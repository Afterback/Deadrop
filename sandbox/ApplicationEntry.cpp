#include "Application.h"

#ifdef PROJECT_PLATFORM_WIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif


#ifdef PROJECT_PLATFORM_WIN

// for Windows, we must implement the WinMain entry point
int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    // create a new instace
    Sandbox::Application app{};
    // call the initialization function
    if (!app.Init())
    {
        // error, failed to initialize the app, returned false
        return -1;
    }

    // call the cleanup function
    app.Destroy();

    return 0;
}
#else

// show a compile-time error for other platforms that we didn't not implement an entry point for
#error Sorry, there is no existing implementation of the entry point function for the platform you are currently targeting!

#endif


