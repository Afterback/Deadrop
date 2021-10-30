#include "Application.h"
using namespace Sandbox;

// used for registering the systems
#include "engine/runtime/systems/core/CoreSystem.h"
// used to create and handle a window and its events
#include "engine/runtime/systems/window/WindowSystem.h"

bool Application::Init()
{
    // TODO: implement application initialization here, anything that is specific to an app
    auto window_system = deadrop::systems::Register<deadrop::systems::WindowSystem>();

    // prepare a description for the window that we will create
    deadrop::systems::WindowDesc window_desc{};
    // this is the name used by Windows to refer to this window
    window_desc.name = "DeadropSandboxGame";
    // this is the actual title that you see in the window
    window_desc.title = "Deadrop Sandbox Game";
    window_desc.width = 1280;
    window_desc.height = 720;

    // initialize the window with the description we filled previously
    bool window_initialized = window_system->Init(window_desc);
    if (!window_initialized)
    {
        // error, failed to initialize the window
        return false;
    }

    // show the actual window
    window_system->Show();

    // prepare the keyboard callback handlers for the events the window can fire
    auto WindowEventHandler_OnKeyDown = [](u8 key) {
        // a key down event was fired
        // the first argument says which key was down
    };

    auto WindowEventHandler_OnKeyUp = [](u8 key) {
        // a key up event was fired
        // the first argument says which key was up
    };

    auto WindowEventHandler_OnLostKeyboardFocus = [](u64 window_handle)
    {
        // the window just lost keyboard focus
        // the first argument is a handle to the window that lost the keyboard focus
        // NOTE: treat this event like an OnKeyUp event for all the down keys (invalidate their pressed state)
        // since the window no longer has keyboard focus
    };

    // register the event handlers that we created previously
    // so they can get called by the window when event happen
    window_system->SetKeyboardCallbacks(WindowEventHandler_OnKeyDown,
        WindowEventHandler_OnKeyUp, WindowEventHandler_OnLostKeyboardFocus);

    // update the window for it to stay responsive and process input
    // when the user closes the window, Update() will return true,
    // which causes this loop to stop looping
    while (!window_system->Update())
    {
        // TODO: replace this loop with an actual game loop
        continue;
    }

    // return success
    return true;
}

void Application::Destroy()
{
    // TODO: release all objects and handles and memory in this function if needed
    // and shutdown everything that is currently running
    return;
}
