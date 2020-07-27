#pragma once
#include "engine/core/memory/memory.h"
#include "IShader.h"
#include "IRasterizerState.h"

namespace deadrop
{
    namespace render
    {
        // a descriptor for pipeline state, used in creation
        struct PipelineStateDesc
        {
            sptr<IRasterizerState> RasterizerState;
            sptr<IShader> VertexShader;
            sptr<IShader> PixelShader;
            sptr<IShader> GeometryShader;
            sptr<IShader> ComputeShader;
            // TODO: add blend state
        };

        // an interface to expose a unified pipeline state object
        class IPipelineState
        {
        public:
            // destructor
            virtual ~IPipelineState() {}

            // returns the descriptor that was used to create the pipeline state
            virtual PipelineStateDesc& GetDesc() = 0;
        };
    }
}
