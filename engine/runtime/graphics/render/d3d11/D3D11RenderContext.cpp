#include "engine/runtime/graphics/render/d3d11/D3D11RenderContext.h"
#include "engine/runtime/graphics/render/d3d11/D3D11Texture2D.h"
#include "engine/runtime/graphics/render/d3d11/D3D11Device.h"
#include "engine/runtime/graphics/render/d3d11/D3D11RenderTarget.h"
#include "engine/runtime/graphics/render/d3d11/D3D11Viewport.h"
#include "engine/runtime/graphics/render/d3d11/D3D11Shader.h"
#include "engine/runtime/graphics/render/d3d11/D3D11RasterizerState.h"
#include "engine/runtime/graphics/render/d3d11/D3D11Buffer.h"
#include "engine/runtime/graphics/render/d3d11/D3D11PipelineState.h"
#include "engine/runtime/graphics/render/d3d11/D3D11UniformBuffer.h"
#include "engine/runtime/graphics/render/d3d11/D3D11DepthStencilState.h"
#include "engine/core/debug.h"
using namespace deadrop;
using namespace deadrop::render;
using namespace deadrop::memory;

#include <vector>
//#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#if defined(PROJECT_BUILD_DEBUG) || defined(PROJECT_BUILD_RELEASE)
#include <d3d11sdklayers.h>
#endif

bool D3D11RenderContext::CreateDevice(const DeviceDesc& deviceDesc)
{
    m_device_desc = deviceDesc;

    if (m_initialized)
    {
        // error, device already created
        return false;
    }

    // create a d3d11 device using the first Adapter
    // NOTE: must specify D3D_DRIVER_TYPE_UNKNOWN when providing the adapter to use
    UINT deviceFlags = 0;
#ifdef PROJECT_BUILD_DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    HRESULT hrCreateDevice = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags,
        D3D11FeatureLevels, D3D11FeatureLevelsNum, D3D11_SDK_VERSION,
        m_device.GetAddressOf(),
        &m_featureLevel,
        m_deviceContext.GetAddressOf());

    if (FAILED(hrCreateDevice))
    {
        // error, failed to create a d3d11 device
        return false;
    }

    // Get the deferred context
    m_device->GetImmediateContext(m_deviceContext.GetAddressOf());
    HRESULT hrDeferredContext = m_device->CreateDeferredContext(0, m_deviceContextDeferred.GetAddressOf());
    if (FAILED(hrDeferredContext))
    {
        // error, failed to create a d3d11 deferred context
        return false;
    }

    // store the created device, context, deferred context for other classes to use
    D3D11Device::Init(m_device, m_deviceContext, m_deviceContextDeferred);

    // get a d3d11.1 Device and device context
    // NOTE: available on Windows 7 with platform update and up
    HRESULT hrDevice1 = m_device.As(&m_device1);
    if (SUCCEEDED(hrDevice1))
    {
        m_device1->GetImmediateContext1(&m_deviceContext1);
    }

    m_initialized = true;

    // check device features
    // TODO: continue the implementation
    D3D11_FEATURE_DATA_ARCHITECTURE_INFO data;
    D3D11_FEATURE_DATA_D3D11_OPTIONS data2;
    D3D11_FEATURE_DATA_D3D11_OPTIONS1 data3;
    D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS data4;
    D3D11_FEATURE_DATA_D3D9_OPTIONS data5;
    HRESULT hr10 = m_device->CheckFeatureSupport(D3D11_FEATURE::D3D11_FEATURE_ARCHITECTURE_INFO, &data, sizeof(data));
    HRESULT hr11 = m_device->CheckFeatureSupport(D3D11_FEATURE::D3D11_FEATURE_D3D11_OPTIONS, &data2, sizeof(data2));
    HRESULT hr12 = m_device->CheckFeatureSupport(D3D11_FEATURE::D3D11_FEATURE_D3D11_OPTIONS1, &data3, sizeof(data3));
    HRESULT hr13 = m_device->CheckFeatureSupport(D3D11_FEATURE::D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &data4, sizeof(data4));
    HRESULT hr14 = m_device->CheckFeatureSupport(D3D11_FEATURE::D3D11_FEATURE_D3D9_OPTIONS, &data5, sizeof(data5));

    return true;
}

bool D3D11RenderContext::CreateSwapchain(const SwapchainDesc& swapchainDesc)
{
    if (m_device == nullptr)
    {
        // error, invalid device, either previous device creation failed to was not done
        return false;
    }

    // create the swapchain

    // fill in the d3d11 swapchain description
    DXGI_FORMAT D3DswapChainFormat = D3D11Common::ResolveType(swapchainDesc.format);
    DXGI_SWAP_CHAIN_DESC D3DswapchainDesc;
    std::memset(&D3DswapchainDesc, 0, sizeof(D3DswapchainDesc));
    D3DswapchainDesc.BufferCount = 1;
    D3DswapchainDesc.BufferDesc.Width = swapchainDesc.width;
    D3DswapchainDesc.BufferDesc.Height = swapchainDesc.height;
    D3DswapchainDesc.BufferDesc.Format = D3DswapChainFormat;
    D3DswapchainDesc.BufferDesc.RefreshRate.Numerator = swapchainDesc.refreshRateNom;
    D3DswapchainDesc.BufferDesc.RefreshRate.Denominator = swapchainDesc.refreshRateDenom;
    D3DswapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    D3DswapchainDesc.OutputWindow = static_cast<HWND>(swapchainDesc.windowHandle);
    D3DswapchainDesc.SampleDesc.Count = 1;
    D3DswapchainDesc.SampleDesc.Quality = 0;
    D3DswapchainDesc.Windowed = swapchainDesc.windowed;

    IDXGIFactory* factory = nullptr;
    HRESULT hrFactory = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
    if (FAILED(hrFactory))
    {
        // error, failed to create a dxgi factory
        return false;
    }

    // create the actual swapchain
    HRESULT hrSwapChain = factory->CreateSwapChain(m_device.Get(), &D3DswapchainDesc, &m_swapChain);
    if (FAILED(hrSwapChain))
    {
        // error, failed to create the swapchain
        return false;
    }

    // get the back-buffer from the swapchain
    HRESULT hrBackBuffer = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &m_backBuffer);
    if (FAILED(hrBackBuffer))
    {
        // error, failed to get the back buffer from the swapchain
        return false;
    }

    // create an ITexture object from the actual back buffer texture
    auto tempTexture = std::make_shared<D3D11Texture2D>();
    Texture2DDesc backBufferTextureDesc;
    backBufferTextureDesc.width = swapchainDesc.width;
    backBufferTextureDesc.height = swapchainDesc.height;
    backBufferTextureDesc.format = swapchainDesc.format;
    tempTexture->m_desc = backBufferTextureDesc;
    tempTexture->m_d3dFormat = D3DswapChainFormat;
    tempTexture->m_texture = m_backBuffer;
    m_backBufferTexture = std::move(tempTexture);

    return true;
}


uptr<ITexture2D> D3D11RenderContext::CreateTexture2D(const Texture2DDesc& desc,
    const MemoryBlock& data,
    const std::vector<MemoryBlock>& dataArray)
{
    // forward the call to the appropriate object
    auto ptr = std::make_unique<D3D11Texture2D>();
    return ptr->Create(desc, data, dataArray) ? std::move(ptr) : nullptr;
}

uptr<IBuffer> D3D11RenderContext::CreateBuffer(BufferDesc& desc, const MemoryBlock& data)
{
    // forward the call to the appropriate object
    auto ptr = std::make_unique<D3D11Buffer>();
    return ptr->Create(desc, data) ? std::move(ptr) : nullptr;
}

uptr<IShader> D3D11RenderContext::CreateShaderFromFile(const ShaderDesc& desc, const std::wstring& filePath)
{
    // forward the call to the appropriate object
    auto ptr = std::make_unique<D3D11Shader>(desc);
    // compile the shader
    bool compiled = ptr->Compile(filePath);
    if (compiled)
    {
        // compiled successfully
        return std::move(ptr);
    }
    else
    {
        // error, failed to compile
        return nullptr;
    }
}

uptr<ITexture2D> D3D11RenderContext::CreateDepth2D(const Texture2DDesc& desc)
{
    // forward the call to the appropriate object
    auto ptr = std::make_unique<D3D11Depth2D>();
    return ptr->Create(desc) ? std::move(ptr) : nullptr;
}

uptr<IViewport> D3D11RenderContext::CreateViewport(const ViewportDesc& desc)
{
    // forward the call to the appropriate object
    return std::make_unique<D3D11Viewport>(desc);
}

uptr<IRenderTarget> D3D11RenderContext::CreateRenderTarget(ITexture2D* texture)
{
    // forward the call to the appropriate object
    auto ptr = std::make_unique<D3D11RenderTarget>();
    return ptr->Create(texture) ? std::move(ptr) : nullptr;
}
uptr<IPipelineState> D3D11RenderContext::CreatePipelineState(const PipelineStateDesc& pipelineState)
{
    // forward the call to the appropriate object
    return std::make_unique<D3D11PipelineState>(pipelineState);
}

uptr<IRasterizerState> D3D11RenderContext::CreateRasterizerState(const RasterizerStateDesc& desc)
{
    // forward the call to the appropriate object
    auto ptr = std::make_unique<D3D11RasterizerState>();
    return ptr->Create(desc) ? std::move(ptr) : nullptr;
}

uptr<IDepthStencilState> D3D11RenderContext::CreateDepthStencilState(const DepthStencilStateDesc& desc)
{
    // forward the call to the appropriate object
    auto ptr = std::make_unique<D3D11DepthStencilState>();
    return ptr->Create(desc) ? std::move(ptr) : nullptr;
}

void D3D11RenderContext::BindShader(IShader* shader)
{
    // retrieve the actual d3d11 implementation object
    auto pShader = static_cast<D3D11Shader*>(shader);
    switch (pShader->GetType())
    {
    case SHADER_TYPE::SHADER_TYPE_VERTEX:
    {
        m_deviceContext->IASetInputLayout(pShader->m_inputLayout.Get());
        m_deviceContext->VSSetShader(pShader->m_vertexShader.Get(), nullptr, NULL);
        return;
    }
    case SHADER_TYPE::SHADER_TYPE_PIXEL:
    {
        m_deviceContext->PSSetShader(pShader->m_pixelShader.Get(), nullptr, NULL);
        return;
    }
    case SHADER_TYPE::SHADER_TYPE_GEOMETRY:
    {
        m_deviceContext->GSSetShader(pShader->m_geometryShader.Get(), nullptr, NULL);
        return;
    }
    case SHADER_TYPE::SHADER_TYPE_COMPUTE:
    {
        m_deviceContext->CSSetShader(pShader->m_computeShader.Get(), nullptr, NULL);
        return;
    }
    default:
    {
        // error, invalid shader type
        return;
    }
    }
}

void D3D11RenderContext::BindState(IRasterizerState* state)
{
    // retrieve the actual d3d11 implementation object
    auto pRasterizerState = static_cast<D3D11RasterizerState*>(state);
    m_deviceContext->RSSetState(pRasterizerState->m_rasterizerState.Get());
}

void D3D11RenderContext::BindState(IDepthStencilState* state)
{
    // retrieve the actual d3d11 implementation object
    auto pDepthStencilState = static_cast<D3D11DepthStencilState*>(state);
    m_deviceContext->OMSetDepthStencilState(pDepthStencilState->m_depthStencilState.Get(), 1);
}

void D3D11RenderContext::BindState(IPipelineState* pipelineState)
{
    // casting is not needed since IPipelineState and this function 
    // are implemented using existing engine functionality
    auto desc = pipelineState->GetDesc();
    if (desc.RasterizerState)	{ BindState(desc.RasterizerState.get()); }
    if (desc.VertexShader)		{ BindShader(desc.VertexShader.get()); }
    if (desc.PixelShader)		{ BindShader(desc.PixelShader.get()); }
    if (desc.GeometryShader)	{ BindShader(desc.GeometryShader.get()); }
    if (desc.ComputeShader)		{ BindShader(desc.ComputeShader.get()); }
}

void D3D11RenderContext::BindTexture(unsigned int slot, ITexture2D* texture)
{
    if (texture == nullptr)
    {
        // unbind the texture at the specified slot
        ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
        m_deviceContext->PSSetShaderResources(slot, 1, nullSRV);
        return;
    }
    // retrieve the actual d3d11 implementation object
    auto pTexture = static_cast<D3D11Texture2D*>(texture);
    m_deviceContext->PSSetShaderResources(slot, 1, pTexture->m_shaderResourceView.GetAddressOf());
}

void D3D11RenderContext::BindBuffer(IBuffer* buffer, unsigned int slot, const BIND_STAGE& stage)
{
    // retrieve the actual d3d11 implementation object
    auto pBuffer = static_cast<D3D11Buffer*>(buffer);
    auto context = D3D11Device::GetDeviceContext();

    // bind the buffer based on its type
    switch (pBuffer->m_desc.type)
    {
    case BUFFER_TYPE::BUFFER_TYPE_VERTEX:
    {
        UINT offset = 0;
        UINT stride = pBuffer->m_desc.stride;
        context->IASetVertexBuffers(slot, 1, pBuffer->m_buffer.GetAddressOf(), &stride, &offset);
        break;
    }
    case BUFFER_TYPE::BUFFER_TYPE_INDEX:
    {
        context->IASetIndexBuffer(pBuffer->m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        break;
    }
    default:
        break;
    }
}

void D3D11RenderContext::BindUniformBuffer(IUniformBuffer* uniformBuffer, const render::BIND_STAGE& stage)
{
    // retrieve the actual d3d11 implementation object
    auto pUniformBuffer = static_cast<D3D11UniformBuffer*>(uniformBuffer);

    // bind the uniform buffer to a specific stage
    switch (stage)
    {
    case BIND_STAGE::BIND_STAGE_VERTEX:
    {
        m_deviceContext->VSSetConstantBuffers(pUniformBuffer->m_desc.bindSlot, 1, pUniformBuffer->m_buffer.GetAddressOf());
        break;
    }
    case BIND_STAGE::BIND_STAGE_PIXEL:
    {
        m_deviceContext->PSSetConstantBuffers(pUniformBuffer->m_desc.bindSlot, 1, pUniformBuffer->m_buffer.GetAddressOf());
        break;
    }
    case BIND_STAGE::BIND_STAGE_GEOMETRY:
    {
        break;
    }
    }
}

void D3D11RenderContext::BindRenderTarget(IRenderTarget* renderTarget, ITexture2D* depthStencil)
{
    // retrieve the actual d3d11 implementation object
    auto pRenderTargetView = static_cast<D3D11RenderTarget*>(renderTarget)->m_renderTargetView;
    if (depthStencil == nullptr)
    {
        // set the render target without depth stencil
        m_deviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);
    }
    else
    {
        // retrieve the actual d3d11 implementation object
        auto pDepthStencilView = static_cast<D3D11Depth2D*>(depthStencil)->m_depthStencilView;
        // set the render target with the provided depth stencil
        m_deviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
    }
}

void D3D11RenderContext::Clear(IRenderTarget* renderTarget, const float color[4])
{
    // retrieve the actual d3d11 implementation object
    auto pRenderTarget = static_cast<D3D11RenderTarget*>(renderTarget);
    // clear the render target with the provided color
    m_deviceContext->ClearRenderTargetView(pRenderTarget->m_renderTargetView.Get(), color);
}

void D3D11RenderContext::Clear(ITexture2D* depthTexture)
{
    // retrieve the actual d3d11 implementation object
    auto pDepthTexture = static_cast<D3D11Depth2D*>(depthTexture);
    // clear the depth stencil texture
    m_deviceContext->ClearDepthStencilView(pDepthTexture->m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f, 0);
}

sptr<ITexture2D> D3D11RenderContext::GetBackBuffer()
{
    // return the back-buffer texture
    return m_backBufferTexture;
}

void D3D11RenderContext::Draw(unsigned int vertexCount, unsigned int startOffset)
{
    // set the primitive topology that will be used
    // TODO: topology should not be set here
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // issue a draw
    m_deviceContext->Draw(vertexCount, startOffset);
}

void D3D11RenderContext::DrawIndexed(unsigned int indexCount, unsigned int startOffset)
{
    // set the primitive topology that will be used
    // TODO: topology should not be set here
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // TODO: topology shouldn't be set here
    // issue an indexed draw
    m_deviceContext->DrawIndexed(indexCount, startOffset, 0);
}

void D3D11RenderContext::SetViewport(IViewport* viewport)
{
    // retrieve the actual d3d11 implementation object
    auto pViewport = static_cast<D3D11Viewport*>(viewport)->m_viewport;
    // set the viewport for the rasterizer
    m_deviceContext->RSSetViewports(1, &pViewport);
}

void D3D11RenderContext::SetScissor(const Rect& rect)
{
    D3D11_RECT d3dRect{ 0 };
    d3dRect.left = rect.left;
    d3dRect.right = rect.right;
    d3dRect.top = rect.top;
    d3dRect.bottom = rect.bottom;
    // set the scissor of the rasterizer 
    m_deviceContext->RSSetScissorRects(1, &d3dRect);
}

const DeviceDesc& D3D11RenderContext::GetDeviceDesc()
{
    return m_device_desc;
}

void D3D11RenderContext::Present(bool vsync)
{
    // swap the buffers of the swapchain in order to present
    // the texture that was drawn into it, to show it on the screen
    m_swapChain->Present(vsync, 0);
}
