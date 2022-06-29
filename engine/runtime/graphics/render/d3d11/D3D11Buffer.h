#pragma once
#include "engine/runtime/graphics/render/IBuffer.h"
#include "D3D11Common.h"
#include <stdint.h>

namespace deadrop 
{
    namespace render
    {
        class D3D11Buffer : public IBuffer
        {
        public:
            D3D11Buffer() = default;

            bool Create(const BufferDesc& desc, const memory::MemoryBlock& data) override;
            bool SetData(const memory::MemoryBlock& data) const override;
            bool SetDataAtOffset(unsigned int offset, const memory::MemoryBlock& data) const override;
            uint32_t GetByteSize() const override;
            const BufferDesc& GetDesc() override;

            // returns a pointer to the actual resource
            ID3D11Buffer* Get();

        private:
            bool CreateVertexBuffer(const BufferDesc& desc, const memory::MemoryBlock& data);
            bool CreateIndexBuffer(const BufferDesc& desc, const memory::MemoryBlock& data);

            friend class D3D11RenderContext;
            ComPtr<ID3D11Buffer> m_buffer;
            BufferDesc m_desc;
        };
    }
}
