#include "D3D11Common.h"
#include "engine/core/debug.h"
using namespace deadrop::render;


DXGI_FORMAT D3D11Common::ResolveType(const TEXTURE2D_FORMAT& format)
{
    switch (format)
    {
    case RGBA_8U:
        return DXGI_FORMAT_R8G8B8A8_UINT;
    case RGBA_8UN:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    case SRGBA_8UN:
        return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    case RGBA_8S:
        return DXGI_FORMAT_R8G8B8A8_SINT;
    case RGBA_8SN:
        return DXGI_FORMAT_R8G8B8A8_SNORM;
    case RGBA_16U:
        return DXGI_FORMAT_R16G16B16A16_UINT;
    case RGBA_16UN:
        return DXGI_FORMAT_R16G16B16A16_UNORM;
    case RGBA_16S:
        return DXGI_FORMAT_R16G16B16A16_SINT;
    case RGBA_16SN:
        return DXGI_FORMAT_R16G16B16A16_SNORM;
    case RGBA_16F:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case RGBA_32U:
        return DXGI_FORMAT_R32G32B32A32_UINT;
    case RGBA_32S:
        return DXGI_FORMAT_R32G32B32A32_SINT;
    case RGBA_32F:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case RGB_32U:
        return DXGI_FORMAT_R32G32B32_UINT;
    case RGB_32S:
        return DXGI_FORMAT_R32G32B32_SINT;
    case RGB_32F:
        return DXGI_FORMAT_R32G32B32_FLOAT;
    case D24S8:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    default:
        // error, invalid texture format, defaulting to RGB_8UN!
        return DXGI_FORMAT_B8G8R8A8_UNORM;
    }
}

D3D11_FILL_MODE D3D11Common::ResolveType(const FILL_MODE& fillMode)
{
    switch (fillMode)
    {
    case FILL_MODE::FILL_MODE_SOLID:
    {
        return D3D11_FILL_MODE::D3D11_FILL_SOLID;
    }
    case FILL_MODE::FILL_MODE_WIREFRAME:
    {
        return D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    }
    default:
    {
        // error, incorrect filling mode, defaulting to FILL_SOLID!
        return D3D11_FILL_MODE::D3D11_FILL_SOLID;
    }
    }
}

D3D11_CULL_MODE D3D11Common::ResolveType(const CULL_MODE & cullMode)
{
    switch (cullMode)
    {
    case CULL_MODE::CULL_MODE_NONE:
    {
        return D3D11_CULL_MODE::D3D11_CULL_NONE;
    }
    case CULL_MODE::CULL_MODE_FRONT:
    {
        return D3D11_CULL_MODE::D3D11_CULL_FRONT;
    }
    case CULL_MODE::CULL_MODE_BACK:
    {
        return D3D11_CULL_MODE::D3D11_CULL_BACK;
    }
    default:
    {
        // error, incorrect culling mode, defaulting to CULL_NONE!
        return D3D11_CULL_MODE::D3D11_CULL_NONE;
    }
    }

}

unsigned int D3D11Common::ResolveType(const CPU_ACCESS & cpuAccess)
{
    unsigned int D3D11CPUAccessFlag = 0;
    D3D11CPUAccessFlag |= (cpuAccess & CPU_ACCESS::CPU_ACCESS_READ) ? D3D11_CPU_ACCESS_READ : 0;
    D3D11CPUAccessFlag |= (cpuAccess & CPU_ACCESS::CPU_ACCESS_WRITE) ? D3D11_CPU_ACCESS_WRITE : 0;
    return D3D11CPUAccessFlag;
}

D3D11_USAGE D3D11Common::ResolveType(const USAGE & usage)
{
    switch (usage)
    {
    case USAGE::USAGE_DEFAULT: return D3D11_USAGE::D3D11_USAGE_DEFAULT;
    case USAGE::USAGE_DYNAMIC: return D3D11_USAGE::D3D11_USAGE_DYNAMIC;
    case USAGE::USAGE_STAGING: return D3D11_USAGE::D3D11_USAGE_STAGING;
    case USAGE::USAGE_STATIC: return D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
    default:
        // error, incorrect usage, defaulting to USAGE_DEFAULT!
        return D3D11_USAGE::D3D11_USAGE_DEFAULT;
    }
}

D3D11_BIND_FLAG D3D11Common::ResolveType(const BIND_FLAG & bindFlag)
{
    switch (bindFlag)
    {
    case BIND_FLAG::BIND_FLAG_RENDER_TARGET: return D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
    case BIND_FLAG::BIND_FLAG_SHADER_RESOURCE: return D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
    case BIND_FLAG::BIND_FLAG_UNIFORM_BUFFER: return D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
    default:
        // error, incorrect bind flag, defaulting to BIND_SHADER_RESOURCE!
        return D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
    }
}

D3D11_SRV_DIMENSION D3D11Common::ResolveType(const TEXTURE2D_TYPE& type)
{
    switch (type)
    {
    case TEXTURE2D_TYPE::TEXTURE2D_TYPE_REGULAR: { return D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D; }
    case TEXTURE2D_TYPE::TEXTURE2D_TYPE_CUBEMAP: { return D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURECUBE; }
    default:
        // error, incorrect texture 2d type, defaulting to SRV_DIMENSION_TEXTURE2D!
        return D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;;
    }
}

unsigned int D3D11Common::SizeOf(const TEXTURE2D_FORMAT& format)
{
    // formula is component size * number of components
    switch (format)
    {
    case RGBA_8U:
        return sizeof(uint8_t) * 4;
    case RGBA_8UN:
        return sizeof(uint8_t) * 4;
    case SRGBA_8UN:
        return sizeof(uint8_t) * 4;
    case RGBA_8S:
        return sizeof(int8_t) * 4;
    case RGBA_8SN:
        return sizeof(int8_t) * 4;
    case RGBA_16U:
        return sizeof(uint16_t) * 4;
    case RGBA_16UN:
        return sizeof(uint8_t) * 4;
    case RGBA_16S:
        return sizeof(int8_t) * 4;
    case RGBA_16SN:
        return sizeof(int8_t) * 4;
    case RGBA_16F:
        return sizeof(float) * 4;
    case RGBA_32U:
        return sizeof(uint32_t) * 4;
    case RGBA_32S:
        return sizeof(int32_t) * 4;
    case RGBA_32F:
        return sizeof(float) * 4;
    case RGB_32U:
        return sizeof(uint32_t) * 3;
    case RGB_32S:
        return sizeof(int32_t) * 3;
    case RGB_32F:
        return sizeof(float) * 3;
    case D24S8:
        return 32;
    default:
        // error, invalid texture format, cannot determin the size, returning 0!
        return 0;
    }
}

UNIFORM_BUFFER_TYPE D3D11Common::ResolveType(const D3D_CBUFFER_TYPE & type)
{
    switch (type)
    {
    case D3D_CBUFFER_TYPE::D3D_CT_CBUFFER: return UNIFORM_BUFFER_TYPE::UNIFORM_BUFFER_TYPE_SCALAR;
    case D3D_CBUFFER_TYPE::D3D_CT_TBUFFER: return UNIFORM_BUFFER_TYPE::UNIFORM_BUFFER_TYPE_TEXTURE;
    default:
    {
        // error, incorrect or not yet supported D3D_CBUFFER_TYPE, returning UNIFORM_BUFFER_TYPE_SCALAR!
        return UNIFORM_BUFFER_TYPE::UNIFORM_BUFFER_TYPE_SCALAR;
    }
    }
}
