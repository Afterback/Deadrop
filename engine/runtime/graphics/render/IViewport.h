#pragma once

namespace deadrop
{
    namespace render
    {
        // a descriptor for the viewport
        struct ViewportDesc
        {
            float width;
            float height;
            float minDepth;
            float maxDepth;
        };

        // an interface to expose viewport functionalities
        class IViewport
        {
        public:

            // sets the size of the viewport
            virtual void SetSize(float width, float height) = 0;

            // sets the minimum and maximum depth of the viewport
            virtual void SetDepth(float minDepth, float maxDepth) = 0;

            // virtual destructor
            virtual ~IViewport() {}
        };
    }
}
