#pragma once
#include "engine/runtime/graphics/render/IRasterizerState.h"
#include "D3D11Common.h"

namespace deadrop 
{
    namespace render 
    {
        class D3D11RasterizerState : public IRasterizerState
        {
        public:
            D3D11RasterizerState() = default;
            const RasterizerStateDesc& GetDesc() override;

            // returns a pointer to the actual resource
            ID3D11RasterizerState* Get();

        private:
            bool Create(const RasterizerStateDesc& desc);
            friend class D3D11RenderContext;

            D3D11_RASTERIZER_DESC m_d3dRasterizerState;
            ComPtr<ID3D11RasterizerState> m_rasterizerState;
            RasterizerStateDesc m_desc;
        };
    }
}
