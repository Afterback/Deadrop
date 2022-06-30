#include "WindowSystem.h"
using namespace deadrop::systems;

#include <Windows.h>
// for GET_X_LPARAM and GET_Y_LPARAM
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (LONG_PTR user_data = GetWindowLongPtr(hWnd, GWLP_USERDATA))
    {
        // redirect the call to the non-static memeber function
        WindowSystem* this_window = reinterpret_cast<WindowSystem*>(user_data);
        return this_window->message_loop(hWnd, message, wParam, lParam);
    }

    if (message == WM_NCCREATE)
    {
        // store a pointer to this class in the window's extra storage bytes.
        LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        void* lpCreateParam = create_struct->lpCreateParams;
        WindowSystem* this_window = reinterpret_cast<WindowSystem*>(lpCreateParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_window));
        return this_window->message_loop(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

// based on:
// https://docs.microsoft.com/en-us/windows/win32/dxtecharts/taking-advantage-of-high-dpi-mouse-movement
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

namespace
{
    void registerRawMouseInput(HWND hWnd)
    {
        RAWINPUTDEVICE rid[1];
        rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
        rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
        rid[0].dwFlags = RIDEV_INPUTSINK;
        rid[0].hwndTarget = hWnd;
        RegisterRawInputDevices(rid, 1, sizeof(rid[0]));
    }
}

bool WindowSystem::Init(WindowDesc& windowDesc)
{
    m_window_desc = windowDesc;

    // create a window
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    if (hInstance == nullptr) return false;

    std::wstring windowName = std::wstring(windowDesc.name.begin(), windowDesc.name.end());
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = &WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = windowName.c_str();
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        return false;
    }

    // get the window size that fits our rendering rectangle (client size).
    RECT windowRect = { 0, 0, static_cast<LONG>(windowDesc.width), static_cast<LONG>(windowDesc.height) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, false);

    // calculate the width and height based on what AdjustWindowRect returned (window size).
    m_window_size.first = windowRect.right - windowRect.left;
    m_window_size.second = windowRect.bottom - windowRect.top;

    // create the actual window
    std::wstring windowTitle = std::wstring(windowDesc.title.begin(), windowDesc.title.end());

    m_window_handle = CreateWindow(
        windowName.c_str(),
        windowTitle.c_str(),
        //WS_CLIPCHILDREN to avoid drawing on child windows
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowDesc.width, windowDesc.height,
        NULL,
        NULL,
        hInstance,
        this
    );
    if (m_window_handle == NULL)
    {
        return false;
    }

    // register raw mouse input so we can receives messages about it
    registerRawMouseInput(m_window_handle);
    UpdateWindow(m_window_handle);

    return true;
}

void WindowSystem::Destroy()
{
    // destroy the window
    DestroyWindow(m_window_handle);
}

u32 translateVirtualKey(WPARAM wParam, LPARAM lParam)
{
    // MSDocs WM_KEYDOWN lParam:
    // bits [16-32] is The scan code. The value depends on the OEM.
    u32 scan_code = (lParam & 0x00ff0000) >> 16;
    // MSDocs WM_KEYDOWN lParam: 
    // bit-24 Indicates whether the key is an extended key,
    // such as the right-hand ALT and CTRL keys that appear on
    // an enhanced 101- or 102-key keyboard. The value is 1 if it is an extended key; 
    // otherwise, it is 0.
    bool extended = (lParam & 0x0100000) != 0;

    // wParam is a virtual-key
    WPARAM vk = wParam;
    // NOTE: we don't need to call MapVirtualKey() for VK_CONTROL and VK_MENU because the extended boolean
    // is set for the right-hand ALT and CTRL as MSDocs says.
    switch (wParam)
    {
    // shift
    case VK_SHIFT:
    {
        vk = MapVirtualKey(scan_code, MAPVK_VSC_TO_VK_EX);
        break;
    }
    // ctrl
    case VK_CONTROL:
    {
        vk = extended ? VK_RCONTROL : VK_LCONTROL;
        break;
    }
    // alt
    case VK_MENU:
    {
        vk = extended ? VK_RMENU : VK_LMENU;
        break;
    }
    default:
        break;
    }

    return static_cast<u32>(vk);
}

LRESULT WindowSystem::message_loop(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    // called when the window's X button is pressed
    case WM_CLOSE:          
    { 
        m_window_exiting = true;
        DestroyWindow(hWnd);
        break;
    }
    // called after WM_CLOSE does DestroyWindow()
    case WM_DESTROY:
    {
        // release the cursor
        ConfineCursor(false);
        m_window_exiting = true;
        // queue a WM_QUIT to the next PeekMessage() or GetMessage()
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE:
    {
        auto width = LOWORD(lParam);
        auto height = HIWORD(lParam);
        m_window_size.first = width;
        m_window_size.second = height;
        break;
    }
    case WM_KEYDOWN:
    {
        // NOTE: wParam is a virtual-key code of the nonsystem key (alt isn't pressed)
        auto vk = static_cast<u32>(wParam);
        switch (vk)
        {
        case VK_SHIFT:
        case VK_CONTROL:
        case VK_MENU:
        {
            vk = translateVirtualKey(vk, lParam);
            break;
        }
        default:
            break;
        }
        if (m_fOnKeyDown)
            m_fOnKeyDown(vk);

        return 0;
    }
    case WM_KEYUP:
    {
        // NOTE: this message is not received when the window loses focus,
        // so WM_KILLFOCUS is treated as a WM_KEYUP for all keys.
        auto key = static_cast<u32>(wParam);
        if (m_fOnKeyUp)
            m_fOnKeyUp(key);
        return 0;
    }
    case WM_KILLFOCUS:
    {
        // NOTE: WM_KEYUP message is not received when the window loses focus
        // so when you receive this message, treat it also as a WM_KEYUP for all keys
        // wParam is HWND
        if (m_fOnLostKeyboardFocus)
            m_fOnLostKeyboardFocus(static_cast<u64>(wParam));

         // release the cursor when the window loses focus
        if(!m_window_exiting && m_cursor_confined)
            ConfineCursor(false);
        return 0;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    {
        switch (wParam)
        {
        case MK_LBUTTON:
        {
            if (m_fOnMouseKeyDown)
                m_fOnMouseKeyDown(VK_LBUTTON);
            break;
        }
        case MK_RBUTTON:
        {
            if (m_fOnMouseKeyDown)
                m_fOnMouseKeyDown(VK_RBUTTON);
            break;
        }
        case MK_MBUTTON:
        {
            if (m_fOnMouseKeyDown)
                m_fOnMouseKeyDown(VK_MBUTTON);
            break;
        }
        default:
            break;
        }
        return 0;
    }
    case WM_LBUTTONUP:
    {
        if (m_fOnMouseKeyUp)
            m_fOnMouseKeyUp(VK_LBUTTON);
        return 0;
    }
    case WM_RBUTTONUP:
    {
        if (m_fOnMouseKeyUp)
            m_fOnMouseKeyUp(VK_RBUTTON);
        return 0;
    }
    case WM_MBUTTONUP:
    {
        if (m_fOnMouseKeyUp)
            m_fOnMouseKeyUp(VK_MBUTTON);
        return 0;
    }
    // 5 button mouse support:
    case WM_XBUTTONDOWN:
    {
        auto xbutton = GET_XBUTTON_WPARAM(wParam);
        switch (xbutton)
        {
        case XBUTTON1:
        {
            if (m_fOnMouseKeyDown)
                m_fOnMouseKeyDown(VK_XBUTTON1);
            break;
        }
        case XBUTTON2:
        {
            if (m_fOnMouseKeyDown)
                m_fOnMouseKeyDown(VK_XBUTTON2);
            break;
        }
        default:
            break;
        }
        return 0;
    }
    case WM_XBUTTONUP:
    {
        auto xbutton = GET_XBUTTON_WPARAM(wParam);
        switch (xbutton)
        {
        case XBUTTON1:
        {
            if (m_fOnMouseKeyUp)
                m_fOnMouseKeyUp(VK_XBUTTON1);
            break;
        }
        case XBUTTON2:
        {
            if (m_fOnMouseKeyUp)
                m_fOnMouseKeyUp(VK_XBUTTON2);
            break;
        }
        default:
            break;
        }
        return 0;
    }
    // end 5 mouse button support
    case WM_MOUSEWHEEL:
    {
        // MSDOCS: The high-order word indicates the distance the wheel is rotated,
        // expressed in multiples or divisions of WHEEL_DELTA, which is 120
        // A positive value indicates that the wheel was rotated forward, away from the user;
        // a negative value indicates that the wheel was rotated backward, toward the user.
        auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (zDelta > 0)
        {
            // wheel was rotated forward
            if (m_fOnMouseWheel)
                m_fOnMouseWheel(true);
        }
        else
        {
            // wheel was rotated backward
            if (m_fOnMouseWheel)
                m_fOnMouseWheel(false);
        }
        return 0;
    }
    case WM_INPUT:
    {
        // get the size of the buffer
        RAWINPUT raw;
        UINT rawSize = sizeof(raw);

        // get the actual buffer data
        UINT result = GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &raw, &rawSize, sizeof(RAWINPUTHEADER));
        if (result == -1)
        {
            // failed to retrieve raw input data
            return 0;
        }

        // handle raw mouse input 
        if (raw.header.dwType == RIM_TYPEMOUSE)
        {
            u64 mouse_raw_x = raw.data.mouse.lLastX;
            u64 mouse_raw_y = raw.data.mouse.lLastY;
            if (m_fOnMouseRelativeMovement)
                m_fOnMouseRelativeMovement(mouse_raw_x, mouse_raw_y);
        }
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        auto mouse_pos_x = GET_X_LPARAM(lParam);
        auto mouse_pos_y = GET_Y_LPARAM(lParam);
        if (m_fOnMouseMovement)
            m_fOnMouseMovement(mouse_pos_x, mouse_pos_y);
        return 0;
    }

    case WM_ACTIVATEAPP:
    {
        if (static_cast<bool>(wParam) == true && m_fOnLostKeyboardFocus)
        {
            m_fOnLostKeyboardFocus(static_cast<u64>(0));
        }
        return 0;
    }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

bool WindowSystem::Update()
{
    MSG msg;
    std::memset(&msg, 0, sizeof(MSG));
    //GetMessage(&msg, NULL, NULL, NULL);
    PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
    // check if a WM_QUIT was queued by the WndProc
    bool exit = false;
    if (msg.message == WM_QUIT)
    {
        exit = true;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    return exit;
}

bool WindowSystem::Show()
{
    if (m_window_handle)
    {
        bool window_previously_visible = ShowWindow(m_window_handle, SW_SHOWNORMAL) != 0;
        return window_previously_visible;
    }

    return false;
}

bool WindowSystem::IsWindowVisible()
{
    // NOTE: to avoid name collision with our own function and the Windows api function
    using ::IsWindowVisible;

    if (m_window_handle)
    {
        bool window_visible = IsWindowVisible(m_window_handle) && !IsIconic(m_window_handle);
        return window_visible;
    }

    // return false if the window 
    return false;
}

bool WindowSystem::Hide()
{
    if (m_window_handle)
    {
        bool success = ShowWindow(m_window_handle, SW_HIDE);
        return success;
    }

    return false;
}

void* WindowSystem::GetHandle()
{
    return m_window_handle;
}

void WindowSystem::ConfineCursor(bool confine)
{
    if (confine && !m_window_exiting)
    {
        // save the current cursor clip
        GetClipCursor(&m_old_cursor_clip);

        // confine the cursor to the application's window.
        RECT window_rect{ 0 };
        GetWindowRect(m_window_handle, &window_rect);
        if (ClipCursor(&window_rect) == 0)
        {
            // error, ClipCursor() failed, call GetLastError() for more info
        }
        m_cursor_confined = true;
    }
    else if(!confine && !m_window_exiting)
    {
        // restore the old cursor clip
        ClipCursor(&m_old_cursor_clip);
        m_cursor_confined = false;
    }
}

deadrop::pair<unsigned int, unsigned int> WindowSystem::GetClientSize()
{
    RECT rect{ 0 };
    if (GetClientRect(m_window_handle, &rect))
    {
        return
        {
            static_cast<u32>(rect.right - rect.left),
            static_cast<u32>(rect.bottom - rect.top)
        };
    }
    // GetClientRect() failed, so we return { 0, 0 } instead.
    return { 0, 0 };
}

deadrop::pair<unsigned int, unsigned int> WindowSystem::GetWindowSize()
{
    RECT rect{ 0 };
    if (GetWindowRect(m_window_handle, &rect))
    {
        return
        {
            static_cast<u32>(rect.right - rect.left),
            static_cast<u32>(rect.bottom - rect.top)
        };
    }
    // GetWindowRect() failed, so we return { 0, 0 } instead.
    return { 0, 0 };
}

// NOTE: we will still get WM_INPUT messages in the WndProc from time to time
// so we process those too, but hopefully this function will process most of the messages
// NOTE: WM_INPUT messages will still be received for example when the window title bar is clicked
void WindowSystem::ProcessRawInput()
{
    UINT cbSize = 0;
    if (GetRawInputBuffer(NULL, &cbSize, sizeof(RAWINPUTHEADER)) != 0)
    {
        // error, call to GetRawInputBuffer() failed
        return;
    }

    // process up to 16 messages
    // NOTE: this number must be at least 16 to make sure we process the data in batches
    // (must be at least 8 on WOW64 or it won't work correctly!)
    constexpr UINT k_max_message_count_to_process = 16;
    cbSize *= k_max_message_count_to_process;

    // allocate a buffer large enough
    std::vector<BYTE> buffer(cbSize);
    PRAWINPUT pRawInput = (PRAWINPUT)buffer.data();
    if (pRawInput == NULL)
    {
        // error, failed to allocate enough memory
        return;
    }

    // loop and process the messages
    while (true)
    {
        UINT cbSizeT = cbSize;
        UINT nInput = GetRawInputBuffer(pRawInput, &cbSizeT, sizeof(RAWINPUTHEADER));
        if (nInput == 0)
        {
            break;
        }

        // NOTE: guard against GetRawInputBuffer() returning UINT_MAX when 'cbSize' is not set correctly
        // in order to avoid accidentally allocating a huge amount of memory
        constexpr UINT k_guard_max = k_max_message_count_to_process;
        if (nInput > k_guard_max)
        {
            // error, the returned value from GetRawInputBuffer() is way bigger than we can handle
            return;
        }

        // allocate a buffer big enough to hold the data
        // TODO: use an allocator instead!
        auto buffer_array = std::make_unique<PRAWINPUT[]>(nInput);
        PRAWINPUT* paRawInput = buffer_array.get();
        if (paRawInput == NULL)
        {
            break;
        }

        // defined for NEXTRAWINPUTBLOCK() to compile
        using QWORD = __int64;

        // loop and extract the data
        PRAWINPUT pri = pRawInput;
        for (UINT i = 0; i < nInput; ++i)
        {
            paRawInput[i] = pri;

            // handle raw mouse input 
            if (pri->header.dwType == RIM_TYPEMOUSE)
            {
                // process the mouse data
                i64 mouse_raw_x = pri->data.mouse.lLastX;
                i64 mouse_raw_y = pri->data.mouse.lLastY;

                // call the callback with the data if it was set
                if (m_fOnMouseRelativeMovement)
                    m_fOnMouseRelativeMovement(mouse_raw_x, mouse_raw_y);
            }

            // process the next raw input block
            pri = NEXTRAWINPUTBLOCK(pri);
        }
    }
}
