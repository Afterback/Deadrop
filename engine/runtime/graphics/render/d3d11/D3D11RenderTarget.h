#pragma once
#include "engine/runtime/graphics/render/IRenderTarget.h"
#include "engine/runtime/graphics/render/ITexture2D.h"
#include "D3D11Common.h"

namespace deadrop
{
    namespace render
    {
        // an interface to expose the render target functionality
        class D3D11RenderTarget : public IRenderTarget
        {
        public:
            // default constructor
            D3D11RenderTarget() = default;

            // returns a pointer to the actual resource
            ID3D11RenderTargetView* Get();

        private:
            // private creation function
            bool Create(ITexture2D* texture);
            friend class D3D11RenderContext;

            ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        };
    }
}
