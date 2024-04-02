#pragma once
#include "engine/runtime/systems/core/ISystem.h"
#include "engine/core/math/math.h"
#include "engine/core/types.h"
#include "engine/core/pair.h"
#include <array>
#include <tuple>

namespace deadrop
{
    namespace systems
    {
        // reference: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
        enum VIRTUAL_KEY : u8
        {
            // NOTE: this key is not for use, only internally (as a default value)
            KEY_EMPTY = 0x00,
            // keyboard numbers:
            KEY_0 = 0x30,
            KEY_1 = 0x31,
            KEY_2 = 0x32,
            KEY_3 = 0x33,
            KEY_4 = 0x34,
            KEY_5 = 0x35,
            KEY_6 = 0x36,
            KEY_7 = 0x37,
            KEY_8 = 0x38,
            KEY_9 = 0x39,
            // keypad numbers:
            KEY_NUMPAD_0 = 0x60,
            KEY_NUMPAD_1 = 0x61,
            KEY_NUMPAD_2 = 0x62,
            KEY_NUMPAD_3 = 0x63,
            KEY_NUMPAD_4 = 0x64,
            KEY_NUMPAD_5 = 0x65,
            KEY_NUMPAD_6 = 0x66,
            KEY_NUMPAD_7 = 0x67,
            KEY_NUMPAD_8 = 0x68,
            KEY_NUMPAD_9 = 0x69,
            // keyboard alphabet:
            KEY_A = 0x41,
            KEY_B = 0x42,
            KEY_C = 0x43,
            KEY_D = 0x44,
            KEY_E = 0x45,
            KEY_F = 0x46,
            KEY_G = 0x47,
            KEY_H = 0x48,
            KEY_I = 0x49,
            KEY_J = 0x4A,
            KEY_K = 0x4B,
            KEY_L = 0x4C,
            KEY_M = 0x4D,
            KEY_N = 0x4E,
            KEY_O = 0x4F,
            KEY_P = 0x50,
            KEY_Q = 0x51,
            KEY_R = 0x52,
            KEY_S = 0x53,
            KEY_T = 0x54,
            KEY_U = 0x55,
            KEY_V = 0x56,
            KEY_W = 0x57,
            KEY_X = 0x58,
            KEY_Y = 0x59,
            KEY_Z = 0x5A,
            // shift, ctrl, alt (regular)
            KEY_SHIFT = 0x10,
            KEY_CONTROL = 0x11,
            KEY_ALT = 0x12,
            // shift, ctrl, alt (extended)
            KEY_LSHIFT = 0xA0,
            KEY_RSHIFT = 0xA1,
            KEY_LCONTROL = 0xA2,
            KEY_RCONTROL = 0xA3,
            KEY_LALT = 0xA4,
            KEY_RALT = 0xA5,
            // keyboard movement keys
            KEY_SPACE = 0x20,
            KEY_BACKSPACE = 0x08,
            KEY_TAB = 0x09,
            KEY_ENTER = 0x0D,
            KEY_ESCAPE = 0x1B,
            // keyboard math operations keys
            KEY_MULTIPLY = 0x6A,
            KEY_ADD = 0x6B,
            KEY_SUBTRACT = 0x6D,
            KEY_DECIMAL = 0x6E,
            KEY_DIVIDE = 0x6F,
            // mouse buttons
            KEY_MOUSE_LEFT = 0x01,
            KEY_MOUSE_RIGHT = 0x02,
            KEY_MOUSE_MIDDLE = 0x04,
            KEY_MOUSE_FOUR = 0x05,
            KEY_MOUSE_FIVE = 0x06,
        };

        enum MOUSE_WHEEL_DIRECTION
        {
            MOUSE_WHEEL_FORWARD,
            MOUSE_WHEEL_BACKWARD
        };

        class InputSystem : public ISystem
        {
        public:
            void Destroy() override {}

            // Update must be called each frame for per-frame functionality like
            // IsKeyPressed() to function correctly
            void Update();

            // HandlerOnKeyDown A handler that must be called for InputModule to function
            // NOTE: must be called each time the Window Module receives a KeyDown event
            void Handler_OnKeyDown(u8 key);

            // HandlerOnKeyUp A handler that must be called for InputModule to function!
            // NOTE: must be called each time the Window Module receives a KeyUp event
            void Handler_OnKeyUp(u8 key);

            // HandlerOnLostKeyboardFocus A handler that must be called for InputModule to function!
            // NOTE: this is used to trigger a key up for all the key when the focus is lost
            void Handler_OnLostKeyboardFocus();

            // HandlerOnMouseRelativeMovement A handler that must called for InputModule to function
            // NOTE: this is used to make relative mouse movement (delta) functions work, must be called by Window Module
            void Handler_OnMouseRelativeMovement(i64 x, i64 y);

            // HandlerOnMouseMovement A handler that must called for InputModule to function
            // NOTE: this is used to make mouse movement functions work, must be called by Window Module
            void Handler_OnMouseMovement(i64 x, i64 y);

            // HandlerOnMouseKeyDown A handler that must be called for InputModule to function
            // NOTE: must be called each time the Window module receives a mouse button down event
            void Handler_OnMouseKeyDown(u8 key);

            // HandlerOnMouseKeyUp A handler that must be called for InputModule to function
            // NOTE: must be called each time the Window module receives a mouse button up event
            void Handler_OnMouseKeyUp(u8 key);

            // HandlerOnMouseWheel A handler that must be called for InputModule to function
            // NOTE: must be called each time the Window module receives a mouse wheel rotation event
            void Handler_OnMouseWheel(bool direction);

            // IMPORTANT NOTE:
            // IsKey...(key) functions return true for regular keys when their extended-key counterpart is set,
            // but only when the regular key is passed, not vise-versa,
            // for example: KEY_SHIFT will return true when either KEY_LSHIFT or KET_RSHFIT is set,
            // and KEY_0 will return true when KEY_NUMPAD_0 is set and so on.

            // Return true while the key is pressed (frame-based)
            // NOTE: keys that have extended counterparts return true when their extended key is pressed.
            bool IsKeyDown(VIRTUAL_KEY key);

            // Return true while the key is not pressed (frame-based)
            // NOTE: keys that have extended counterparts return true when their extended key is pressed.
            bool IsKeyUp(VIRTUAL_KEY key);

            // Return true only when the the key is pressed for the first time (first frame), (frame-based)
            // NOTE: keys that have extended counterparts return true when their extended key is pressed.
            // NOTE: don't use this overload for check for a combinations of key, use the other overload instead
            bool IsKeyPressed(VIRTUAL_KEY key);

            // NOTE: use this function for key-combinations instead of the regular IsKeyPressed() because you can't
            // check if two keys are pressed at the same time with that function, even if you use an if(pressed && pressed)
            // NOTE: all keys must be pressed in the following order: first_key then first_key only then second_key!
            // NOTE: if you pass the function the following parameters for example:
            // (KEY_CONTROL, KEY_D, KEY_S) another call with the parameters (KEY_CONTROL, KEY_D) will also return true right after
            // the second_key is pressed, so it's better to give three-key combination their unique second_key to avoid collision
            bool IsKeyPressed(VIRTUAL_KEY first_key, VIRTUAL_KEY second_key, VIRTUAL_KEY third_key = KEY_EMPTY);

            // Return true only when the the key is released for the first time (frame-based)
            bool IsKeyReleased(VIRTUAL_KEY key);

            // Return the delta of the mouse position, the amount of movement since last mouse position (frame-based)
            // NOTE: this function is meant to have high-resolution (raw-input) and no acceleration or cursor ballistics.
            // NOTE: do not attempt to accumulate the results returned by this function hoping to get absolute mouse position!
            // NOTE: this function is only useful in things such as controling a game camera, not to be used for user-interfaces.
            math::vec2f GetMouseRelativeMovement();

            // Return the mouse position relative to the window (top-left is the 0,0 coordinate)
            // NOTE: this function returns mouse position that is relative the to the window, use it for UI,
            // instead of GetMouseRelativeMovement()
            math::vec2f GetMousePosition();

            // Return true when the mouse wheel is rotated in the specified direction
            // NOTE: this function acts like IsKeyPressed, but it's more frequent because of the nature of the mouse wheel
            // that acts in an incremental way (each sroll makes this function return true for one frame)
            bool IsMouseWheelRotated(MOUSE_WHEEL_DIRECTION direction);

        private:
            enum KEY_STATE
            {
                KEY_STATE_NOT_PRESSED,
                KEY_STATE_PRESSED,
            };

            // this array is used as a buffer for keeping keys (mouse and keyboard) state as we receive it
            std::array<KEY_STATE, 256> m_keyStateBuffer{ KEY_STATE_NOT_PRESSED };
            // this array is used by frame-based functions, these are updated per-frame
            std::array<KEY_STATE, 256> m_perframe_currentKeyState{ KEY_STATE_NOT_PRESSED };
            std::array<KEY_STATE, 256> m_perframe_previousKeyState{ KEY_STATE_NOT_PRESSED };

            // reset all key states to KEY_STATE_NOT_PRESSED
            void clear();

            // update the current key state (buffer)
            inline void updateKeyStateBuffer(u8 key, KEY_STATE state);

            // return the current key state (buffer)
            inline KEY_STATE checkCurrentKeyStateBuffer(u8 key);

            // NOTE: these functions requires the Update() function to be
            // called regularly, for per-frame functionality to function
            inline KEY_STATE checkPerframePreviousKeyState(u8 key);
            inline KEY_STATE checkPerframeCurrentKeyState(u8 key);

            // this function maps extended-keys to their regular keys,
            // for example: KEY_LSHIFT and KEY_RSHIFT are mapped to KEY_SHIFT,
            // and KEY_NUMPAD_0 is mapped to KEY_0 and so on
            inline u8 mapExtendedToRegularKeys(u8 vk);

            // variables for relative mouse movement
            bool m_mouse_moved = false;
            math::vec2f m_mouse_raw_movement{ 0.0f, 0.0f };
            math::vec2f m_mouse_perframe_delta{ 0.0f, 0.0f };
            // variables for regular mouse movement
            math::vec2f m_mouse_position = { 0.0f, 0.0f };
            math::vec2f m_mouse_perframe_position{ 0.0f, 0.0f };
            pair<bool, MOUSE_WHEEL_DIRECTION> m_mouse_wheel;
            pair<bool, MOUSE_WHEEL_DIRECTION> m_mouse_wheel_last;
        };
    }
}
