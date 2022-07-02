#pragma once
#include "engine/core/memory/memory.h"
#include "engine/runtime/graphics/render/IRenderContext.h"
#include "engine/runtime/graphics/render/IRenderTarget.h"
#include "engine/runtime/graphics/render/IViewport.h"
#include "engine/runtime/graphics/render/IShader.h"
#include <d3d11.h>
#include <d3d11_1.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace deadrop
{
    namespace render
    {
        class D3D11RenderContext : public IRenderContext
        {
        public:
            // graphics pipleline object creation functions
            virtual bool CreateDevice(const DeviceDesc& deviceDesc) override;
            virtual bool CreateSwapchain(const SwapchainDesc& swapchain) override;
            virtual uptr<ITexture2D>	CreateTexture2D(
                const Texture2DDesc& desc,
                const memory::MemoryBlock& data,
                const std::vector<memory::MemoryBlock>& dataArray) override;

            virtual uptr<IBuffer> CreateBuffer(BufferDesc& desc, const memory::MemoryBlock& data) override;
            virtual uptr<IShader> CreateShaderFromFile(const ShaderDesc& desc, const std::wstring& filePath) override;
            virtual uptr<ITexture2D> CreateDepth2D(const Texture2DDesc& desc) override;
            virtual uptr<IViewport> CreateViewport(const ViewportDesc& desc) override;
            virtual uptr<IRenderTarget> CreateRenderTarget(ITexture2D* texture) override;
            virtual uptr<IPipelineState> CreatePipelineState(const PipelineStateDesc& pipelineState) override;
            virtual uptr<IRasterizerState> CreateRasterizerState(const RasterizerStateDesc& desc) override;
            virtual uptr<IDepthStencilState> CreateDepthStencilState(const DepthStencilStateDesc& desc) override;

            // graphics pipeline changing functions
            virtual sptr<ITexture2D> GetBackBuffer() override;
            virtual void BindShader(IShader* shader) override;
            virtual void BindState(IRasterizerState* state) override;
            virtual void BindState(IDepthStencilState* state) override;
            virtual void BindState(IPipelineState* pipelineState) override;
            virtual void BindTexture(unsigned int slot, ITexture2D* texture) override;
            virtual void BindBuffer(IBuffer* buffer, unsigned int slot, const BIND_STAGE& stage) override;
            virtual void BindUniformBuffer(IUniformBuffer* uniformBuffer, const BIND_STAGE& stage) override;
            virtual void BindRenderTarget(IRenderTarget* renderTarget, ITexture2D* depthStencil)override;
            virtual void Clear(IRenderTarget* renderTarget, const float color[4]) override;
            virtual void Clear(ITexture2D* depthTexture) override;
            virtual void Draw(unsigned int vertexCount, unsigned int startOffset) override;
            virtual void DrawIndexed(unsigned int vertexCount, unsigned int startOffset = 0) override;
            virtual void Present(bool vsync) override;

            virtual void SetViewport(IViewport* viewport) override;
            virtual void SetScissor(const Rect& rect) override;

            const DeviceDesc& GetDeviceDesc() override;

        private:
            ComPtr<ID3D11Device>            m_device = nullptr;
            ComPtr<ID3D11Device1>           m_device1 = nullptr;
            ComPtr<ID3D11DeviceContext>     m_deviceContext = nullptr;
            ComPtr<ID3D11DeviceContext1>    m_deviceContext1 = nullptr;
            ComPtr<ID3D11DeviceContext>     m_deviceContextDeferred = nullptr;
            ComPtr<IDXGISwapChain>          m_swapChain = nullptr;
            ComPtr<ID3D11Texture2D>         m_backBuffer = nullptr;
            DeviceDesc m_device_desc;
            bool m_initialized = false;
            sptr<ITexture2D> m_backBufferTexture = nullptr;

            D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
            const unsigned int D3D11FeatureLevelsNum = 6;
            D3D_FEATURE_LEVEL D3D11FeatureLevels[6] =
            {
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1,
            };
        };
    }
}
