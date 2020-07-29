#include "D3D11RasterizerState.h"
#include "D3D11Device.h"
#include "engine/core/debug.h"
using namespace deadrop::render;


bool D3D11RasterizerState::Create(const RasterizerStateDesc& desc)
{
    m_desc = desc;

    m_d3dRasterizerState.FillMode = D3D11Common::ResolveType(desc.FillMode);
    m_d3dRasterizerState.CullMode = D3D11Common::ResolveType(desc.CullMode);
    m_d3dRasterizerState.FrontCounterClockwise = !desc.FrontFaceIsClockwise;
    m_d3dRasterizerState.DepthBias = desc.DepthBias;
    m_d3dRasterizerState.DepthBiasClamp = 0.0f;		  // currently not supported by the engine
    m_d3dRasterizerState.SlopeScaledDepthBias = 0.0f; // currently not supported by the engine
    m_d3dRasterizerState.DepthClipEnable = desc.EnableDepthClip;
    m_d3dRasterizerState.ScissorEnable = desc.EnableScissor;
    m_d3dRasterizerState.MultisampleEnable = desc.EnableMultisampling;
    m_d3dRasterizerState.AntialiasedLineEnable = desc.EnableLineAntialiasing;

    auto device = D3D11Device::GetDevice();
    HRESULT hr = device->CreateRasterizerState(&m_d3dRasterizerState, m_rasterizerState.GetAddressOf());
    if (SUCCEEDED(hr))
    {
        return true;
    }

    // error, failed to create rasterizer state
    return false;
}

const RasterizerStateDesc& D3D11RasterizerState::GetDesc()
{
    return m_desc;
}
