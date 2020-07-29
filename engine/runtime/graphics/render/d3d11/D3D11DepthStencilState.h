#pragma once
#include "engine/runtime/graphics/render/IDepthStencilState.h"
#include "D3D11Common.h"

namespace deadrop
{
    namespace render
    {
        class D3D11DepthStencilState : public IDepthStencilState
        {
        public:
            D3D11DepthStencilState() = default;
            const DepthStencilStateDesc& GetDesc() override;

        private:
            bool Create(const DepthStencilStateDesc& desc);
            friend class D3D11RenderContext;
            ComPtr<ID3D11DepthStencilState> m_depthStencilState;
            DepthStencilStateDesc m_desc;
        };
    }
}
