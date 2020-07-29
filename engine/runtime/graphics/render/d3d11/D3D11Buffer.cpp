#include "engine/runtime/graphics/render/d3d11/D3D11Buffer.h"
#include "engine/runtime/graphics/render/d3d11/D3D11Device.h"
#include "engine/core/debug.h"
using namespace deadrop::render;
using namespace deadrop::memory;


bool D3D11Buffer::Create(const BufferDesc& desc, const MemoryBlock& data)
{
    // forward the call to the appropriate function
    // base on the buffer type
    switch (desc.type)
    {
        case BUFFER_TYPE::BUFFER_TYPE_VERTEX:
        {
            return CreateVertexBuffer(desc, data);
        }
        case BUFFER_TYPE::BUFFER_TYPE_INDEX:
        {
            return CreateIndexBuffer(desc, data);
        }
        default:
        {
            return false;
        }
    }
}

bool D3D11Buffer::SetData(const MemoryBlock& data) const
{
    // overwrite the old data with the new data
    if (data.ptr != nullptr && data.size > 0)
    {
        D3D11_MAPPED_SUBRESOURCE mapped_sr{ 0 };
        auto context = D3D11Device::GetDeviceContext();
        // mapping the buffer denies gpu access to the resource
        HRESULT hrMap = context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_sr);
        if (FAILED(hrMap))
        {
            if (hrMap == DXGI_ERROR_WAS_STILL_DRAWING)	return false;
            if (hrMap == DXGI_ERROR_DEVICE_REMOVED)		return false;
            if (hrMap == E_OUTOFMEMORY)					return false;
            return false;
        }
        // copy our data that is in-memory to where the gpu can use it
        memcpy(mapped_sr.pData, data.ptr, this->GetByteSize());

        // allow the gpu to use the resource again
        context->Unmap(m_buffer.Get(), 0);
        return true;
    }
    else
    {
        // error, invalid buffer data pointer or data size
        return false;
    }
}

bool D3D11Buffer::SetDataAtOffset(unsigned int offset, const MemoryBlock & data) const
{
    // overwrite the old data with the new data
    if (data.ptr != nullptr && data.size > 0)
    {
        D3D11_MAPPED_SUBRESOURCE mapped_sr{ 0 };
        auto context = D3D11Device::GetDeviceContext();
        // mapping the buffer denies gpu access to the resource
        HRESULT hrMap = context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_sr);
        if (FAILED(hrMap))
        {
            if (hrMap == DXGI_ERROR_WAS_STILL_DRAWING)	return false;
            if (hrMap == DXGI_ERROR_DEVICE_REMOVED)		return false;
            if (hrMap == E_OUTOFMEMORY)					return false;
            return false;
        }
        // copy our data that is in-memory to where the gpu can use it
        memcpy(static_cast<char*>(mapped_sr.pData) + offset, data.ptr, data.size);
        // allow the gpu to use the resource again
        context->Unmap(m_buffer.Get(), 0);
        return true;
    }
    else
    {
        // error, invalid buffer data pointer or data size
        return false;
    }
}

uint32_t D3D11Buffer::GetByteSize() const
{
    return m_desc.count * m_desc.stride;
}

// NOTE: Map() on D3D11_USAGE_DEFAULT is only supported on feature level 11.0 hardware and up, requires driver support too
// it's recommended to use D3D11_USAGE_DYNAMIC for buffers that you need to Map() else the creation will fail on unsupported platforms
bool D3D11Buffer::CreateVertexBuffer(const BufferDesc& desc, const MemoryBlock& data)
{
    m_desc = desc;

    D3D11_BUFFER_DESC D3Ddesc;
    D3Ddesc.MiscFlags = 0;
    D3Ddesc.ByteWidth = desc.count * desc.stride;
    D3Ddesc.StructureByteStride = desc.stride;
    D3Ddesc.CPUAccessFlags = D3D11Common::ResolveType(desc.access);
    D3Ddesc.Usage = D3D11Common::ResolveType(desc.usage);
    D3Ddesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    auto device = D3D11Device::GetDevice();
    HRESULT hr = E_FAIL;

    D3D11_SUBRESOURCE_DATA srd{ 0 };
    if (data.ptr != nullptr && data.size > 0)
    {
        srd.pSysMem = data.ptr;
        srd.SysMemPitch = 0;
        srd.SysMemSlicePitch = 0;
        hr = device->CreateBuffer(&D3Ddesc, &srd, m_buffer.GetAddressOf());
    }
    else
    {
        hr = device->CreateBuffer(&D3Ddesc, nullptr, m_buffer.GetAddressOf());
    }

    if (FAILED(hr))
    {
        // error, failed to create a vertex buffer
        return false;
    }

    return true;
}

bool D3D11Buffer::CreateIndexBuffer(const BufferDesc& desc, const MemoryBlock& data)
{
    m_desc = desc;
    D3D11_BUFFER_DESC D3Ddesc;
    D3Ddesc.MiscFlags = 0;

    D3Ddesc.ByteWidth = desc.count * desc.stride;
    D3Ddesc.StructureByteStride = desc.stride;
    D3Ddesc.CPUAccessFlags = D3D11Common::ResolveType(desc.access);
    D3Ddesc.Usage = D3D11_USAGE_DEFAULT;
    D3Ddesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    auto device = D3D11Device::GetDevice();
    HRESULT hr = E_FAIL;
    D3D11_SUBRESOURCE_DATA srd{ 0 };
    if (data.ptr != nullptr && data.size > 0)
    {
        srd.pSysMem = data.ptr;
        srd.SysMemPitch = 0;
        srd.SysMemSlicePitch = 0;
        hr = device->CreateBuffer(&D3Ddesc, &srd, m_buffer.GetAddressOf());
    }
    else
    {
        hr = device->CreateBuffer(&D3Ddesc, nullptr, m_buffer.GetAddressOf());
    }

    if (FAILED(hr))
    {
        // error, failed to create an index buffer
        return false;
    }

    return true;
}

const BufferDesc& D3D11Buffer::GetDesc()
{
    return m_desc;
}
