#pragma once
#include <fstream>
#include <sstream>
#include <cstdio>

#if defined(PROJECT_BUILD_DEBUG) || defined(PROJECT_BUILD_RELEASE)
#ifdef _MSC_VER
#define NATIVE_DEBUG_BREAK __debugbreak;
#else
#define NATIVE_DEBUG_BREAK __noop
#error "DebugBreak not currently supported on this platform or compiler!"
#endif

namespace deadrop 
{
    void DebugBreak()
    {
        NATIVE_DEBUG_BREAK;
    }
}
#endif