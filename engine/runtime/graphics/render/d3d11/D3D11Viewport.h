#pragma once
#include "engine/runtime/graphics/render/IViewport.h"
#include "D3D11Common.h"

namespace deadrop
{
    namespace render
    {
        class D3D11Viewport : public IViewport
        {
        public:
            D3D11Viewport(const ViewportDesc& viewportDesc);
            void SetSize(float width, float height) override;
            void SetDepth(float minDepth, float maxDepth) override;

        private:
            friend class D3D11RenderContext;
            D3D11_VIEWPORT m_viewport{ 0 };
        };
    }
}
