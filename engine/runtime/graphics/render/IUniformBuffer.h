#pragma once
#include "engine/core/memory/memory.h"
#include <string>

namespace deadrop
{
    namespace render
    {
        // defines the uniform buffer type
        enum UNIFORM_BUFFER_TYPE
        {
            UNIFORM_BUFFER_TYPE_SCALAR,
            UNIFORM_BUFFER_TYPE_TEXTURE,
        };

        // a descriptor for uniform buffers
        struct UniformBufferDesc
        {
            std::string name;
            size_t size;
            unsigned int bindSlot;
            unsigned int variableNum;
            UNIFORM_BUFFER_TYPE type;
            unsigned int flags;
        };

        // an interface to expose uniform buffers functionalities (constant-buffer in d3d terminology)
        class IUniformBuffer
        {
        public:

            // commits the data returned by IShader::GetUniformBufferData<T>() to the gpu
            // NOTE: this function does a copy from the cpu to the gpu by overriding the existing buffer
            virtual bool Commit() = 0;

            // returns the descriptor of the uniform buffer
            virtual const UniformBufferDesc& GetDesc() = 0;

            // virtual destructor
            virtual ~IUniformBuffer() {}

        protected:
            // required to provide the IShader::GetUniformBufferData<T> functionality without exposing implementation details
            friend class IShader;

            // the raw uniform buffer data
            // TODO: inspect to make sure it gets deleted
            sptr<void> data;
        };
    }
}
