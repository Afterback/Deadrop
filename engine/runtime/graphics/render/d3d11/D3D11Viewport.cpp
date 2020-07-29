#include "D3D11Viewport.h"
using namespace deadrop::render;


D3D11Viewport::D3D11Viewport(const ViewportDesc& viewportDesc)
{
    m_viewport.Width = viewportDesc.width;
    m_viewport.Height = viewportDesc.height;
    m_viewport.MinDepth = viewportDesc.minDepth;
    m_viewport.MaxDepth = viewportDesc.maxDepth;
}

void D3D11Viewport::SetSize(float width, float height)
{
    m_viewport.Width = width;
    m_viewport.Height = height;
}

void D3D11Viewport::SetDepth(float minDepth, float maxDepth)
{
    m_viewport.MinDepth = minDepth;
    m_viewport.MaxDepth = maxDepth;
}
