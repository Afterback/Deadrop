#pragma once
#include "engine/runtime/graphics/render/ITexture2D.h"
#include "engine/runtime/graphics/render/IRasterizerState.h"
#include "engine/runtime/graphics/render/IBuffer.h"
#include "engine/runtime/graphics/render/IUniformBuffer.h"
#include <d3d11.h>

#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace deadrop
{
    namespace render
    {
        class D3D11Common
        {
        public:
            static DXGI_FORMAT ResolveType(const TEXTURE2D_FORMAT& format);
            static D3D11_FILL_MODE ResolveType(const FILL_MODE& fillMode);
            static D3D11_CULL_MODE ResolveType(const CULL_MODE& cullMode);
            static D3D11_USAGE ResolveType(const USAGE& usage);
            static D3D11_BIND_FLAG ResolveType(const BIND_FLAG& bindFlag);
            static D3D11_SRV_DIMENSION ResolveType(const TEXTURE2D_TYPE& type);
            static UNIFORM_BUFFER_TYPE ResolveType(const D3D_CBUFFER_TYPE& type);
            static unsigned int SizeOf(const TEXTURE2D_FORMAT& format);

            // https://msdn.microsoft.com/en-us/library/windows/desktop/ff476259(v=vs.85).aspx
            // Resource Usage	Default	Dynamic	Immutable	Staging
            // GPU - Read 		yes		yes		yes			yes¹
            // GPU - Write		yes							yes¹
            // CPU - Read									yes¹
            // CPU - Write				yes					yes¹
            // using 'unsigned int' instead of 'D3D11_CPU_ACCESS_FLAG' due to OR operation
            static unsigned int ResolveType(const CPU_ACCESS& cpuAccess);
        };
    }
}
