#pragma once
#include "engine/runtime/graphics/render/ITexture2D.h"
#include "D3D11Common.h"
#include <vector>

namespace deadrop
{
    namespace render
    {
        class D3D11Texture2D : public ITexture2D
        {
        public:
            D3D11Texture2D() = default;
            virtual ~D3D11Texture2D() {}

            bool SetData(const memory::MemoryBlock& data) override;
            unsigned int GetWidth() override;
            unsigned int GetHeight() override;
            const Texture2DDesc& GetDesc() override;

            // returns a pointer to the actual resource
            ID3D11Texture2D* Get();

        private:
            bool Create(const Texture2DDesc& desc, const memory::MemoryBlock& data,
                const std::vector<memory::MemoryBlock>& dataArray);

        protected:
            friend class D3D11RenderTarget;
            friend class D3D11RenderContext;

            Texture2DDesc m_desc;
            DXGI_FORMAT m_d3dFormat{};
            ComPtr<ID3D11Texture2D> m_texture = nullptr;
            ComPtr<ID3D11ShaderResourceView> m_shaderResourceView = nullptr;
        };

        class D3D11Depth2D : public ITexture2D
        {
        public:
            D3D11Depth2D() = default;
            virtual ~D3D11Depth2D() {}

            bool SetData(const memory::MemoryBlock& data) override;
            unsigned int GetWidth() override;
            unsigned int GetHeight() override;
            const Texture2DDesc& GetDesc() override;

        protected:
            bool Create(const Texture2DDesc& desc);
            friend class D3D11RenderContext;

            Texture2DDesc m_desc;
            DXGI_FORMAT m_d3dFormat{};
            ComPtr<ID3D11Texture2D> m_texture = nullptr;
            ComPtr<ID3D11DepthStencilView> m_depthStencilView = nullptr;
        };
    }
}
