#include "D3D11UniformBuffer.h"
#include "D3D11Device.h"
#include "engine/core/debug.h"
using namespace deadrop::render;


bool D3D11UniformBuffer::Create(const UniformBufferDesc& desc, const std::vector<std::string>& variableNames)
{
    m_desc = desc;
    m_variableNames = variableNames;
    // TODO: use a custom allocator
    data.reset(static_cast<size_t*>(operator new(desc.size)));

    // Create the constant buffer
    D3D11_BUFFER_DESC bufferDesc{ 0 };
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.ByteWidth = (UINT)desc.size;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    auto device = D3D11Device::GetDevice();
    HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, m_buffer.GetAddressOf());
    if (SUCCEEDED(hr))
    {
        return true;
    }

    return false;
}

bool D3D11UniformBuffer::Commit()
{
    D3D11_MAPPED_SUBRESOURCE MAPPED_Subresource;
    std::memset(&MAPPED_Subresource, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));
    // write the new data to the buffer and delete the previous data.
    auto context = D3D11Device::GetDeviceContext();
    HRESULT hr = context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MAPPED_Subresource);
    if (FAILED(hr))
    {
        // error, failed to map the constant buffer
        return false;
    }
    memcpy(MAPPED_Subresource.pData, data.get(), m_desc.size);
    context->Unmap(m_buffer.Get(), 0);

    return true;
}

const UniformBufferDesc& D3D11UniformBuffer::GetDesc()
{
    return m_desc;
}
