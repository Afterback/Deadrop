#pragma once
#include "engine/runtime/graphics/render/IPipelineState.h"

namespace deadrop
{
    namespace render
    {
        class D3D11PipelineState : public IPipelineState
        {
        public:
            D3D11PipelineState(const PipelineStateDesc& desc) : m_desc(desc) {}
            PipelineStateDesc& GetDesc() override;

        private:
            PipelineStateDesc m_desc;
        };
    }
}
