#pragma once

namespace deadrop
{
    namespace render
    {
        struct DepthStencilStateDesc
        {
            bool EnableDepth;
            bool EnableStencil;
            // depth operations

            // stencil operations
        };

        // an interface to expose depth stencil state
        class IDepthStencilState
        {
        public:
            // returns the descriptor that was used to create the depth stencil state
            virtual const DepthStencilStateDesc& GetDesc() = 0;
        };
    }
}
