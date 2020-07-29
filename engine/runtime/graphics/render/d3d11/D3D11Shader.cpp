#include "D3D11Shader.h"
#include "D3D11Device.h"
#include "D3D11UniformBuffer.h"
#include "engine/core/debug.h"
#include "engine/core/math/math.h"
using namespace deadrop::render;

#include <d3dcompiler.h>
// requires D3DCompiler_47.DLL to be bundled with the executable
#pragma comment(lib, "D3DCompiler.lib")
// include dxguid.lib else we get a linking error for 'IID_ID3D11ShaderReflection'
#pragma comment(lib, "dxguid.lib")
#include <vector>
#include <fstream>


bool D3D11Shader::Compile()
{
    std::wstring filePath(m_desc.filePath.begin(), m_desc.filePath.end());
    // check if the file exists first
    std::ifstream file(m_desc.filePath);
    if (file.good() != true)
    {
        // error, shader file does not exist in the specified path
        return false;
    }

#ifdef PROJECT_COMPILE_SHADERS_ONLINE
    HRESULT hr = CompileShaderFromFile(filePath.c_str(), m_desc.entry.c_str(), m_desc.model.c_str());
    if (FAILED(hr))
    {
        // error, failed to compile the shader from file
        return false;
    }
#else
#error Offline shader compilation is currently not supported by the engine
#endif

    auto device = D3D11Device::GetDevice();
    HRESULT hrCreate = E_FAIL;

    switch (m_desc.type)
    {
    case SHADER_TYPE::SHADER_TYPE_VERTEX:
        hrCreate = device->CreateVertexShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
        if (SUCCEEDED(hrCreate))
        {
            HRESULT hrInputLayout = CreateInputLayoutFromBlob(m_shaderBlob.Get(), m_inputLayout.GetAddressOf());
            if (FAILED(hrInputLayout))
            {
                // error, failed to create the shader input layout
                return false;
            }
        }
        else
        {
            // error, failed to create a vertex shader
            return false;
        }
        break;
    case SHADER_TYPE::SHADER_TYPE_PIXEL:
        hrCreate = device->CreatePixelShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());
        if (FAILED(hrCreate))
        {
            // error, failed to create a pixel shader
            return false;
        }
        break;
    case SHADER_TYPE::SHADER_TYPE_GEOMETRY:
    {
        hrCreate = device->CreateGeometryShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), nullptr, m_geometryShader.GetAddressOf());
        if (FAILED(hrCreate))
        {
            // error, failed to create a geometry shader
            return false;
        }
        break;
    }
    case SHADER_TYPE::SHADER_TYPE_COMPUTE:
        hrCreate = device->CreateComputeShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(),
            nullptr, m_computeShader.GetAddressOf());
        if (FAILED(hrCreate))
        {
            // error, failed to create a compute shader
            return false;
        }
        break;
    default:
        // error, invalid shader type, creation failed
        return false;
    }

    // do the shader reflection for later use
    HRESULT hrReflection = D3DReflect(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(),
        IID_ID3D11ShaderReflection, reinterpret_cast<void**>(m_shaderReflection.GetAddressOf()));
    if (FAILED(hrReflection))
    {
        // error, failed to reflect the shader
        return false;
    }

    D3D11_SHADER_DESC shaderDesc;
    HRESULT hrShaderDesc = m_shaderReflection->GetDesc(&shaderDesc);
    if (FAILED(hrShaderDesc))
    {
        // error, reflection failed to get the shader descriptor
        return false;
    }

    // retrieve all constant buffers in the shader
    for (unsigned int i = 0; i < shaderDesc.ConstantBuffers; i++)
    {
        ID3D11ShaderReflectionConstantBuffer* constantBuffer = m_shaderReflection->GetConstantBufferByIndex(i);
        D3D11_SHADER_BUFFER_DESC constantBufferDesc;
        HRESULT hrContantBufferDesc = constantBuffer->GetDesc(&constantBufferDesc);
        if (FAILED(hrContantBufferDesc))
        {
            // error, reflection failed to get the constant buffer descriptor, the buffer might not exist
            return false;
        }

        D3D11_SHADER_INPUT_BIND_DESC shaderInputDesc;
        HRESULT hrShaderInputDesc = m_shaderReflection->GetResourceBindingDescByName(constantBufferDesc.Name, &shaderInputDesc);
        if (FAILED(hrShaderInputDesc))
        {
            //error, reflection failed to get the constant buffer bind descriptor
            return false;
        }

        UniformBufferDesc tempDesc;
        tempDesc.name = constantBufferDesc.Name;
        tempDesc.size = constantBufferDesc.Size;
        tempDesc.bindSlot = shaderInputDesc.BindPoint;
        tempDesc.type = D3D11Common::ResolveType(constantBufferDesc.Type);
        tempDesc.variableNum = constantBufferDesc.Variables;
        tempDesc.flags = constantBufferDesc.uFlags;

        // retrieve variables that are inside of the constant buffer
        std::vector<std::string> uniformVariableNames;
        for (unsigned int k = 0; k < constantBufferDesc.Variables; k++)
        {
            ID3D11ShaderReflectionVariable* var = constantBuffer->GetVariableByIndex(k);
            D3D11_SHADER_VARIABLE_DESC varDesc;
            HRESULT hrVarDesc = var->GetDesc(&varDesc);
            if (FAILED(hrVarDesc))
            {
                // error, reflection failed to get shader variable descriptor
            }

            ID3D11ShaderReflectionType* type = var->GetType();
            D3D11_SHADER_TYPE_DESC typeDesc;
            auto hrTypeDesc = type->GetDesc(&typeDesc);
            if (FAILED(hrTypeDesc))
            {
                // error, reflection failed to get shader variable type
            }

            /* CURRENTLY NOT IMPLEMENTED 
            ShaderVariableDesc tempVarDesc;
            tempVarDesc.name = varDesc.Name;
            tempVarDesc.startOffset = varDesc.StartOffset;
            tempVarDesc.size = varDesc.Size;
            tempVarDesc.flags = varDesc.uFlags;
            tempVarDesc.defaultValue = varDesc.DefaultValue;
            tempVarDesc.textureSize = varDesc.TextureSize;
            tempVarDesc.samplerSize = varDesc.SamplerSize;
            */
            uniformVariableNames.push_back(varDesc.Name);
        }

        auto temp = std::make_shared<D3D11UniformBuffer>();
        if (temp->Create(tempDesc, uniformVariableNames))
        {
            m_uniformBuffers.insert({ constantBufferDesc.Name,
                std::move(temp)
                });
        }
        else
        {
            // error, failed to create a UniformBuffer object for this shader constant buffer
            return false;
        }
        
    }

    return true;
}

IUniformBuffer* D3D11Shader::GetUniformBufferByName(const std::string& name)
{
    return m_uniformBuffers[name].get();
}

HRESULT D3D11Shader::CompileShaderFromFile(const WCHAR * filename, LPCSTR entryPoint, LPCSTR shaderModel)
{
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef PROJECT_BUILD_DEBUG
    shaderFlags |= D3DCOMPILE_DEBUG;
    shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    ComPtr<ID3DBlob> pErrorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
        shaderFlags, 0, m_shaderBlob.GetAddressOf(), &pErrorBlob);
    if (FAILED(hr))
    {
        // error, failed to compile the shader from file
#ifdef PROJECT_BUILD_DEBUG
        if (pErrorBlob)
        {
            OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
            auto error_message = reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer());
        }
#endif
    }

    return hr;
}

HRESULT D3D11Shader::CreateInputLayoutFromBlob(ID3DBlob* shaderBlob, ID3D11InputLayout** inputLayout)
{
    ID3D11ShaderReflection* pReflection = nullptr;
    HRESULT hr = D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&pReflection));
    if (FAILED(hr))
    {
        // error, failed to get shader reflection interface
        return hr;
    }

    D3D11_SHADER_DESC shaderDesc{ 0 };
    pReflection->GetDesc(&shaderDesc);

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
    for (unsigned int i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC parameterDesc;
        pReflection->GetInputParameterDesc(i, &parameterDesc);

        DXGI_FORMAT format = GetFormatFromComponetType(parameterDesc.ComponentType, parameterDesc.Mask);
        D3D11_INPUT_ELEMENT_DESC elementDesc =
        {
            parameterDesc.SemanticName,
            parameterDesc.SemanticIndex,
            format,
            0, // InputSlot
            D3D11_APPEND_ALIGNED_ELEMENT, // AlignedByOffset
            D3D11_INPUT_PER_VERTEX_DATA, // InputSlotClass
            0, // InstanceDataStepRate
        };
        inputLayoutDesc.push_back(elementDesc);
    }

    HRESULT hrCreateInputLayout = D3D11Device::GetDevice()->CreateInputLayout(&inputLayoutDesc[0], (UINT)inputLayoutDesc.size(),
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), inputLayout);
    // free shader reflection memory
    pReflection->Release();
    // return the result
    return hrCreateInputLayout;
}


DXGI_FORMAT D3D11Shader::GetFormatFromComponetType(D3D_REGISTER_COMPONENT_TYPE componentType, unsigned int componentsCount)
{
    if (componentsCount == 1)
    {
        switch (componentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32_UINT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32_SINT;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32_FLOAT;
        default:
            break;
        }
    }
    else if (componentsCount <= 3)
    {
        switch (componentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32_UINT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32_SINT;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32_FLOAT;
        default:
            break;
        }
    }
    else if (componentsCount <= 7)
    {
        switch (componentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32B32_UINT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32B32_SINT;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        default:
            break;
        }
    }
    else if (componentsCount <= 15)
    {
        switch (componentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32B32A32_UINT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32B32A32_SINT;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        default:
            break;
        }
    }

    // error, unknown dxgi format
    return DXGI_FORMAT_UNKNOWN;
}
