#pragma once

namespace deadrop 
{
    namespace render 
    {
        // reading from a gpu resource is not recommended thus when READ is provided,a copy of the data
        // should be kept in main memory and used when further functions expose a read functionality.
        enum CPU_ACCESS
        {
            CPU_ACCESS_NONE,
            CPU_ACCESS_READ,
            CPU_ACCESS_WRITE,
        };

        enum USAGE
        {
            USAGE_DEFAULT,
            USAGE_DYNAMIC,
            USAGE_STATIC,
            USAGE_STAGING
        };

        enum BIND_FLAG
        {
            BIND_FLAG_SHADER_RESOURCE,
            BIND_FLAG_RENDER_TARGET,
            BIND_FLAG_UNIFORM_BUFFER,
            //DepthStencil
        };
    }
}
