#include "D3D11Texture2D.h"
#include "D3D11Device.h"
#include <cassert>
using namespace deadrop::render;
using namespace deadrop::memory;


bool D3D11Texture2D::Create(const Texture2DDesc& desc, const MemoryBlock& data,
    const std::vector<MemoryBlock>& dataArray)
{
    m_desc = desc;

    D3D11_TEXTURE2D_DESC D3Ddesc{ 0 };
    D3Ddesc.Width = desc.width;
    D3Ddesc.Height = desc.height;
    D3Ddesc.MipLevels = desc.mips;
    D3Ddesc.ArraySize = desc.count;
    D3Ddesc.SampleDesc.Count = 1;
    // TODO: add support for Sampling Quality later inside Texture2DDescriptor
    D3Ddesc.SampleDesc.Quality = 0;
    if (desc.type == TEXTURE2D_TYPE::TEXTURE2D_TYPE_CUBEMAP)
    {
        // make sure we have at least 6 textures for the cubemap
        if (desc.count == 6)
        {
            D3Ddesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
        }
        else
        {
            // error, a cubemap requires exactly 6 textures
            return false;
        }
    }
    else
    {
        D3Ddesc.MiscFlags = 0;
    }

    m_d3dFormat = D3D11Common::ResolveType(desc.format);
    D3Ddesc.Format = m_d3dFormat;
    D3Ddesc.CPUAccessFlags = D3D11Common::ResolveType(desc.cpuAccess);
    D3Ddesc.Usage = D3D11Common::ResolveType(desc.usage);
    D3Ddesc.BindFlags = D3D11Common::ResolveType(desc.bindFlags);

    auto device = D3D11Device::GetDevice();
    HRESULT hr = E_FAIL;

    // handle single texture
    if (desc.count == 1 && desc.type == TEXTURE2D_TYPE::TEXTURE2D_TYPE_REGULAR)
    {
        // if initial data is specified
        if (data.ptr != nullptr && data.size > 0)
        {
            D3D11_SUBRESOURCE_DATA srd{ 0 };
            srd.pSysMem = data.ptr;
            srd.SysMemPitch = D3D11Common::SizeOf(desc.format) * desc.width;
            srd.SysMemSlicePitch = 0;
            hr = device->CreateTexture2D(&D3Ddesc, &srd, &m_texture);
        }
        else
        {
            // if no initial data is specified, create texture without any data
            // use SetData() to set the texture data before using it
            hr = device->CreateTexture2D(&D3Ddesc, nullptr, &m_texture);
        }
    }

    // handle texture arrays like cubemaps
    if (desc.count > 1 && desc.type == TEXTURE2D_TYPE::TEXTURE2D_TYPE_CUBEMAP)
    {
        // make sure that we were given enough data for what has been requested
        if (dataArray.size() != desc.count)
        {
            // error, cannot create a texture array from a single texture data, use the single data overload instead
            return false;
        }

        std::vector<D3D11_SUBRESOURCE_DATA> srd(dataArray.size());
        for (unsigned int i = 0; i < dataArray.size(); i++)
        {
            // check if the data array element contains a valid memory
            if (dataArray[i].size > 0 && dataArray[i].ptr != nullptr)
            {
                srd[i].pSysMem = dataArray[i].ptr;
                srd[i].SysMemPitch = D3D11Common::SizeOf(desc.format) * desc.width;
                srd[i].SysMemSlicePitch = 0;
            }
            else
            {
                // error, texture array element must not be nullptr or size is zero
                return false;
            }
        }

        // create the texture2D object
        hr = device->CreateTexture2D(&D3Ddesc, &srd[0], &m_texture);
    }

    if (FAILED(hr))
    {
        // error, failed to create a texture 2d
        return false;
    }

    // create a resource view if the texture will be accessed by a shader
    if (desc.bindFlags == BIND_FLAG::BIND_FLAG_SHADER_RESOURCE)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
        std::memset(&srDesc, 0, sizeof(srDesc));
        srDesc.Format = m_d3dFormat;
        srDesc.ViewDimension = D3D11Common::ResolveType(desc.type);
        srDesc.Texture2D.MostDetailedMip = 0;
        srDesc.Texture2D.MipLevels = desc.mips;

        HRESULT hr2 = device->CreateShaderResourceView(m_texture.Get(), &srDesc, &m_shaderResourceView);
        if (FAILED(hr2))
        {
            // error, failed to create a shader resource view for a texture 2d
            return false;
        }
    }

    return true;
}

const Texture2DDesc& D3D11Texture2D::GetDesc()
{
    return m_desc;
}

bool D3D11Texture2D::SetData(const MemoryBlock& data)
{
    if (data.size == 0) return false;
    if (m_desc.bindFlags != BIND_FLAG::BIND_FLAG_SHADER_RESOURCE) return false;

    auto context = D3D11Device::GetDeviceContext();
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = context->Map(m_texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr))
    {
        // error, failed to map texture 2d resource memory
        return false;
    }

    memcpy(mappedResource.pData, data.ptr, data.size);
    context->Unmap(m_texture.Get(), 0);
    return true;
}

unsigned int D3D11Texture2D::GetWidth()
{
    return m_desc.width;
}

unsigned int D3D11Texture2D::GetHeight()
{
    return m_desc.height;
}

bool D3D11Depth2D::Create(const Texture2DDesc& desc)
{
    m_desc = desc;

    D3D11_TEXTURE2D_DESC D3Ddesc{ 0 };
    D3Ddesc.Width = desc.width;
    D3Ddesc.Height = desc.height;
    D3Ddesc.MipLevels = desc.mips;
    D3Ddesc.ArraySize = desc.count;
    D3Ddesc.SampleDesc.Count = 1;
    // TODO: add support for sampling quality inside Texture2DDesc
    D3Ddesc.SampleDesc.Quality = 0;
    D3Ddesc.MiscFlags = 0;
    m_d3dFormat = D3D11Common::ResolveType(desc.format);
    D3Ddesc.Format = m_d3dFormat;
    D3Ddesc.Usage = D3D11_USAGE_DEFAULT;
    D3Ddesc.CPUAccessFlags = 0;
    D3Ddesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    auto device = D3D11Device::GetDevice();
    HRESULT hr = device->CreateTexture2D(&D3Ddesc, nullptr, &m_texture);
    if (FAILED(hr))
    {
        // error, failed to create depth 2d texture
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    descDSV.Format = D3Ddesc.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    descDSV.Flags = 0;

    // create the depth stencil view
    HRESULT hrDepthStencilView = device->CreateDepthStencilView(m_texture.Get(), &descDSV, m_depthStencilView.GetAddressOf());
    if (FAILED(hrDepthStencilView))
    {
        // error, failed to create the depth stencil view
        return false;
    }
    return true;
}

const Texture2DDesc& D3D11Depth2D::GetDesc()
{
    return m_desc;
}

bool D3D11Depth2D::SetData(const MemoryBlock& data)
{
    // error, cannot use setdata() on a depth texture
    assert("cannot use setdata() on a depth texture!");
    return false;
}

unsigned int D3D11Depth2D::GetWidth()
{
    return m_desc.width;
}

unsigned int D3D11Depth2D::GetHeight()
{
    return m_desc.height;
}
