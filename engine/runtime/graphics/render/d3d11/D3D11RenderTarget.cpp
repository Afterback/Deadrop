#include "D3D11RenderTarget.h"
#include "D3D11Texture2D.h"
#include "D3D11Device.h"
#include "engine/core/debug.h"
using namespace deadrop::render;


bool D3D11RenderTarget::Create(ITexture2D* texture)
{
    // retrieve the actual d3d11 implementation object
    auto pTexture = static_cast<D3D11Texture2D*>(texture);

    D3D11_RENDER_TARGET_VIEW_DESC rtvdec;
    rtvdec.Format = pTexture->m_d3dFormat;
    rtvdec.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvdec.Texture2D.MipSlice = 0;

    // retrieve the actual d3d11 device
    auto device = D3D11Device::GetDevice();
    HRESULT hr = device->CreateRenderTargetView(pTexture->m_texture.Get(), &rtvdec, m_renderTargetView.GetAddressOf());
    if (SUCCEEDED(hr))
    {
        // error, failed to create a render target view from a texture
        return true;
    }

    return false;
}

ID3D11RenderTargetView* D3D11RenderTarget::Get()
{
    return m_renderTargetView.Get();
}
