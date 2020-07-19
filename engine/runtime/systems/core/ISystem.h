#pragma once

namespace deadrop
{
    namespace systems
    {
        class ISystem
        {
        public:
            virtual ~ISystem(){}
            // this function should destroy the instances that were created
            // by the system and also releasing their allocated memory
            virtual void Destroy() = 0;

            //virtual bool Init() = 0;
            //virtual void Pause() = 0;
            //virtual void Stop() = 0;
            //virtual void Update()	= 0;
        };
    }
}