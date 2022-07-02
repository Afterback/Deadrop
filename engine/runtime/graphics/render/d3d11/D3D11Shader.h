#pragma once
#include "engine/core/debug.h"
#include "engine/core/memory/memory.h"
#include "engine/runtime/graphics/render/IShader.h"
#include "D3D11Common.h"
#include <d3d11shader.h>
#include <unordered_map>
#include <string>

namespace deadrop
{
    namespace render
    {
        class D3D11Shader : public IShader
        {
        public:
            D3D11Shader(const ShaderDesc& desc) : m_desc(desc) {}
            ~D3D11Shader() {}

            virtual IUniformBuffer* GetUniformBufferByName(const std::string& name) override;
            virtual SHADER_TYPE GetType() override { return m_desc.type; }
            virtual ShaderDesc GetDesc() override { return m_desc; }

        private:
            friend class D3D11RenderContext;
            ShaderDesc m_desc;

            // holds a ID3D11VertexShader or ID3D11PixelShader or...
            union
            {
                ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
                ComPtr<ID3D11PixelShader> m_pixelShader;
                ComPtr<ID3D11GeometryShader> m_geometryShader;
                ComPtr<ID3D11ComputeShader> m_computeShader;
            };

            // for internal use
            ComPtr<ID3DBlob> m_shaderBlob;
            ComPtr<ID3D11InputLayout> m_inputLayout;
            ComPtr<ID3D11ShaderReflection> m_shaderReflection;
            std::unordered_map<std::string, sptr<IUniformBuffer>> m_uniformBuffers;

            // for internal use
            HRESULT CreateInputLayoutFromBlob(ID3DBlob* shaderBlob, ID3D11InputLayout** inputLayout);
            HRESULT CompileShaderFromFile(const WCHAR * filename, LPCSTR entryPoint, LPCSTR shaderModel);
            DXGI_FORMAT GetFormatFromComponetType(D3D_REGISTER_COMPONENT_TYPE componentType, unsigned int componentsCount);
            bool Compile(const std::wstring& filePath);
        };
    }
}
