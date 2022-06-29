#pragma once
#include "engine/runtime/graphics/render/IUniformBuffer.h"
#include "engine/core/memory/memory.h"
#include "D3D11Common.h"
#include <vector>

namespace deadrop
{
    namespace render
    {
        class D3D11UniformBuffer : public IUniformBuffer
        {
        public:
            D3D11UniformBuffer() = default;
            virtual bool Commit() override;
            virtual const UniformBufferDesc& GetDesc() override;

            // returns a pointer to the actual resource
            ID3D11Buffer* Get();

        private:
            bool Create(const UniformBufferDesc& desc, const std::vector<std::string>& variableNames);
            friend class D3D11RenderContext;
            friend class D3D11Shader;

            UniformBufferDesc m_desc;
            std::vector<std::string> m_variableNames;
            ComPtr<ID3D11Buffer> m_buffer;
        };
    }
}
