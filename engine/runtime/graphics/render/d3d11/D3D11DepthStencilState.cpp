#include "D3D11DepthStencilState.h"
#include "D3D11Device.h"
#include "engine/core/debug.h"
using namespace deadrop::render;


bool D3D11DepthStencilState::Create(const DepthStencilStateDesc& desc)
{
    m_desc = desc;

    // fill d3d11 depth stencil desc
    D3D11_DEPTH_STENCIL_DESC d3d_desc;

    // depth test parameters
    d3d_desc.DepthEnable = desc.EnableDepth;
    d3d_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    d3d_desc.DepthFunc = D3D11_COMPARISON_LESS;

    // stencil test parameters
    d3d_desc.StencilEnable = desc.EnableStencil;
    d3d_desc.StencilReadMask = 0xFF;
    d3d_desc.StencilWriteMask = 0xFF;

    // stencil operations if pixel is front-facing
    d3d_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    d3d_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    d3d_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    d3d_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // stencil operations if pixel is back-facing
    d3d_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    d3d_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    d3d_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    d3d_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    auto device = D3D11Device::GetDevice();
    HRESULT hrDepthStencilState = device->CreateDepthStencilState(&d3d_desc, &m_depthStencilState);
    if (SUCCEEDED(hrDepthStencilState))
    {
        // error, failed to create a depth stencil state
        return true;
    }

    return false;
}

const DepthStencilStateDesc& D3D11DepthStencilState::GetDesc()
{
    return m_desc;
}
