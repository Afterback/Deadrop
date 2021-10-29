#pragma once

namespace Sandbox
{
    class Application
    {
    public:
        // initialization function for the application, it gets called by the application entry point (function)
        bool Init();
        // destroys all the application related resources causing it to stop
        void Destroy();
    };
}