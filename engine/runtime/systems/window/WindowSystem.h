#pragma once
#include "engine/runtime/systems/core/ISystem.h"
#include "engine/core/types.h"
#include "engine/core/pair.h"
#include <string>
#include <vector>
#include <functional>

#ifdef PROJECT_PLATFORM_WIN
#include <Windows.h>
#endif

namespace deadrop
{
    namespace systems
    {
        struct WindowDesc
        {
            unsigned int width;
            unsigned int height;
            std::string name;
            std::string title;
        };

        class WindowSystem : public ISystem
        {
        public:
            
            // default constructor
            WindowSystem() = default;

            // overrides
            void Destroy() override;

            // creates and initializes the window but does not show it
            bool Init(WindowDesc& windowDesc);

            // set the callbacks that will handle keyboard events
            void SetKeyboardCallbacks(
                std::function<void(u8)> fOnKeyDown,
                std::function<void(u8)> fOnKeyUp,
                std::function<void(u64)> fOnLostKeyboardFocus)
            {
                m_fOnKeyDown = fOnKeyDown;
                m_fOnKeyUp = fOnKeyUp;
                m_fOnLostKeyboardFocus = fOnLostKeyboardFocus;
            }

            // set the callbacks that will handle mouse events
            void SetMouseCallbacks(
                std::function<void(i64, i64)> fOnMouseRelativeMovement,
                std::function<void(i64, i64)> fOnMouseMovement,
                std::function<void(u8)> fOnMouseKeyDown,
                std::function<void(u8)> fOnMouseKeyUp,
                std::function<void(bool)> fOnMouseWheel)
            {
                m_fOnMouseRelativeMovement = fOnMouseRelativeMovement;
                m_fOnMouseMovement = fOnMouseMovement;
                m_fOnMouseKeyDown = fOnMouseKeyDown;
                m_fOnMouseKeyUp = fOnMouseKeyUp;
                m_fOnMouseWheel = fOnMouseWheel;
            }

            // updates the window
            bool Update();

            // shows the window
            // returns true if the window was previously visible,
            // and false if it was not previously visible
            bool Show();

            // check the visibility state of the window
            // returns true if the window is visible, and false if it is not visible
            // NOTE: this function might return true even if the window is totatlly obscured
            // by other windows, since it just checks for a visibility flag and not coverage
            bool IsWindowVisible();

            // hides the window
            bool Hide();

            // returns the window handle
            // NOTE: returns the window handle HWND on Windows
            void* GetHandle();

            // confines the cursor to the window, if the window is exiting, it restores the cursor to its previous state
            // NOTE: this function does not confine the cursor if the window is exiting in order to release it for other apps
            void ConfineCursor(bool confine);

            // returns the current client area available for drawing
            // NOTE: returns {0, 0} if it fails 
            pair<unsigned int, unsigned int> GetClientSize();

            // returns the current actual window size used to fit the client area inside
            // NOTE: returns {0, 0} if it fails
            pair<unsigned int, unsigned int> GetWindowSize();

            // polls and processes the input messages directly from the input devices (raw input data)
            // NOTE: this function must be called frequently for it to process the input data, call it on per-frame basis
            // NOTE: this function can process multiple input messages at once (currently the max count is 16 messages per-call)
            void ProcessRawInput();

            // message loop
            // NOTE: on Windows, this is a redirected WndProc
            // NOTE: it must be public so that the actual WndProc can call it
#ifdef PROJECT_PLATFORM_WIN
            LRESULT message_loop(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

        private:
            WindowDesc m_window_desc;
            void* m_window_handle = nullptr;
            bool m_window_exiting = false;
            pair<unsigned int, unsigned int> m_window_size;

            // to keep track of the cursor
            bool m_cursor_confined;
#ifdef PROJECT_PLATFORM_WIN
            RECT m_old_cursor_clip;
#endif

            // keyboard callback functions
            std::function<void(u8)> m_fOnKeyDown;
            std::function<void(u8)> m_fOnKeyUp;
            std::function<void(u64)> m_fOnLostKeyboardFocus;
            // mouse callback functions
            std::function<void(i64, i64)> m_fOnMouseRelativeMovement;
            std::function<void(i64, i64)> m_fOnMouseMovement;
            std::function<void(u8)> m_fOnMouseKeyDown;
            std::function<void(u8)> m_fOnMouseKeyUp;
            std::function<void(bool)> m_fOnMouseWheel;
        };
    }
}