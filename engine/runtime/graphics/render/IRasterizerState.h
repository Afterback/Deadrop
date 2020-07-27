#pragma once

namespace deadrop
{
    namespace render
    {
        // defines which faces the rasterizer should cull
        enum class CULL_MODE
        {
            CULL_MODE_NONE,
            CULL_MODE_FRONT,
            CULL_MODE_BACK,
        };

        // defines how the rasterizer fill the triangles
        enum class FILL_MODE
        {
            FILL_MODE_SOLID,
            FILL_MODE_WIREFRAME
        };

        // a descriptor for rasterizer state, used in creation
        struct RasterizerStateDesc
        {
            bool EnableDepthClip = true;
            bool EnableScissor = true;
            bool EnableMultisampling = false;
            bool EnableLineAntialiasing = false; // only works when drawing lines
            // NOTE: this is used alongside the cull mode to determin
            // how face culling should be done
            bool FrontFaceIsClockwise = false;
            CULL_MODE CullMode = CULL_MODE::CULL_MODE_NONE;
            // determin how triangles should be filled
            FILL_MODE FillMode = FILL_MODE::FILL_MODE_SOLID;
            int DepthBias = 0;
        };

        // an interface to expose the rasterizer state
        class IRasterizerState
        {
        public:
            // returns the descriptor that was used to create the rasterizer state
            virtual const RasterizerStateDesc& GetDesc() = 0;

            // virtual destructor
            virtual ~IRasterizerState() {}
        };
    }
}
