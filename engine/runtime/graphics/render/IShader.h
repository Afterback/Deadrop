#pragma once
#include "engine/core/memory/memory.h"
#include "IUniformBuffer.h"
#include <string>

namespace deadrop
{
    namespace render
    {
        // defines the multiple shader types
        enum SHADER_TYPE
        {
            SHADER_TYPE_VERTEX,
            SHADER_TYPE_PIXEL,
            SHADER_TYPE_GEOMETRY,
            SHADER_TYPE_COMPUTE
        };

        // a descriptor to be used in shader creation
        struct ShaderDesc
        {
            SHADER_TYPE	type;
            std::string entry;
            std::string model;
        };

        // an interface to expose the shader functionality
        class IShader
        {
        public:
            // a template function to enforce the uniform buffer's data structure to a specific type
            template<typename T>
            static T* GetUniformBufferData(IUniformBuffer* base)
            {
                return static_cast<T*>(base->data.get());
            }

            // returns the uniform buffer object by name
            virtual IUniformBuffer* GetUniformBufferByName(const std::string& name) = 0;

            // returns the shader type
            virtual SHADER_TYPE GetType() = 0;

            // returns the descriptor of the shader that was used to create it
            virtual ShaderDesc GetDesc() = 0;

            // virtual destructor
            virtual ~IShader() {};
        };
    }
}
