#pragma once
#include "Common.h"
#include "engine/core/memory/memory.h"

namespace deadrop 
{
    namespace render 
    {
        // defines the 2d texture format
        enum TEXTURE2D_FORMAT
        {
            RGBA_8U,	// 8-bit unsigned integer
            RGBA_8UN,	// 8-bit unsigned normalized
            SRGBA_8UN,	// 8-bit unsigned normalized SRGB
            RGBA_8S,	// 8-bit signed integer
            RGBA_8SN,	// 8-bit signed normalized
            RGBA_16U,	// 16-bit unsgined integer
            RGBA_16UN,	// 16-bit unsigned normalized
            RGBA_16S,	// 16-bit signed integer
            RGBA_16SN,	// 16-bit signed normalized
            RGBA_16F,	// 16-bit float
            RGBA_32U,	// 32-bit unsigned integer
            RGBA_32S,	// 32-bit signed integer
            RGBA_32F,	// 32-bit float
            RGB_32U,	// 32-bit unsigned integer
            RGB_32S,	// 32-bit signed integer
            RGB_32F,	// 32-bit float

            D24S8,		// Depth 24-bit Stencil 8-bit
        };

        // defines the types of 2d textures
        enum class TEXTURE2D_TYPE
        {
            TEXTURE2D_TYPE_REGULAR,
            TEXTURE2D_TYPE_CUBEMAP,
        };

        // a descriptor for 2d textures, used in creation
        struct Texture2DDesc
        {
            TEXTURE2D_FORMAT format = TEXTURE2D_FORMAT::RGBA_8UN;
            unsigned int width = 0;
            unsigned int height = 0;
            unsigned int count = 1;
            unsigned int mips = 1;
            USAGE usage = USAGE::USAGE_DEFAULT;
            CPU_ACCESS cpuAccess = CPU_ACCESS::CPU_ACCESS_NONE;
            BIND_FLAG bindFlags = BIND_FLAG::BIND_FLAG_SHADER_RESOURCE;
            TEXTURE2D_TYPE type = TEXTURE2D_TYPE::TEXTURE2D_TYPE_REGULAR;
        };

        // an interface to expose 2d textures and their functionalities
        class ITexture2D
        {
        public:
            // sets the data by overwriting it
            virtual bool SetData(const memory::MemoryBlock& data) = 0;

            // returns the width of the texture
            virtual unsigned int GetWidth() = 0;

            // returns the height of the texture
            virtual unsigned int GetHeight() = 0;

            // returns the descriptor of the texture
            virtual const Texture2DDesc& GetDesc() = 0;

            // virtual destructor
            virtual ~ITexture2D() {};
        };
    }
}
