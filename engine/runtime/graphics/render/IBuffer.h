#pragma once
#include "engine/core/memory/memory.h"
#include "Common.h"

namespace deadrop 
{
    namespace render
    {
        // defines the type of the buffer
        enum BUFFER_TYPE
        {
            BUFFER_TYPE_VERTEX,
            BUFFER_TYPE_INDEX,
            BUFFER_TYPE_CONSTANT
        };

        // descriptor of the buffer, used in creation
        struct BufferDesc
        {
            BUFFER_TYPE type{};
            // NOTE: actual byte size is calculated as 'size * stride'
            unsigned int count = 0;
            USAGE usage{};
            CPU_ACCESS access{};
            unsigned int stride = 1;
        };

        // an interface to expose buffer creation and other functionality
        class IBuffer
        {
        public:
            // creates a gpu buffer using the backend api and set its data
            virtual bool Create(const BufferDesc& desc, const memory::MemoryBlock& data) = 0;

            // sets the data of the buffer by overwriting it
            virtual bool SetData(const memory::MemoryBlock& data) const = 0;

            // sets the data of the buffer at an offset without overwriting it
            virtual bool SetDataAtOffset(unsigned int offset, const memory::MemoryBlock& data) const = 0;

            // returns the actual byte size
            virtual uint32_t GetByteSize() const = 0;

            // returns the description of the buffer
            virtual const BufferDesc& GetDesc() = 0;

            // virtual destructor
            virtual ~IBuffer(){}
        };
    }
}
