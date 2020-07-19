#include "InputSystem.h"
using namespace deadrop;
using namespace deadrop::systems;

void InputSystem::Update()
{
    // make the current key per-frame state the previous for
    // per-frame functionality to work
    m_perframe_previousKeyState = m_perframe_currentKeyState;
    m_perframe_currentKeyState = m_keyStateBuffer;

    // update mouse delta each frame by using the actual data we recieved from the mouse
    if (m_mouse_moved)
    {
        m_mouse_perframe_delta = m_mouse_raw_movement;
        m_mouse_moved = false;
    }
    else
    {
        // reset the mouse per-frame data since we only get events when the mouse moves,
        // so we have to reset it back to zero in the frames that it doesn't move
        m_mouse_perframe_delta.x = 0.0f;
        m_mouse_perframe_delta.y = 0.0f;
    }

    // update mouse position each frame with data we receieved
    m_mouse_perframe_position = m_mouse_position;

    // simulate mouse wheel event since we don't get an event when the wheel stopped moving
    // NOTE: if the mouse wheel was rotated for the first time, then issue a mouse rotated event,
    // else set the state to not being rotated.
    if (m_mouse_wheel.first && !m_mouse_wheel_last.first)
    {
        m_mouse_wheel.first = true;
    }
    else
    {
        m_mouse_wheel.first = false;
    }
    m_mouse_wheel_last = m_mouse_wheel;
}

void InputSystem::Handler_OnKeyDown(u8 key)
{
    // if the keys are similar (like extended-lshift, extended-rshift and regular shift) we set two states,
    // one for the extended key, and one for the regular one
    auto new_key = mapExtendedToRegularKeys(key);
    if (new_key != key)
    {
        updateKeyStateBuffer(new_key, KEY_STATE_PRESSED);
    }
    updateKeyStateBuffer(key, KEY_STATE_PRESSED);
}

void InputSystem::Handler_OnKeyUp(u8 key)
{
    // if the keys are similar (like extended-lshift, extended-rshift and regular shift) we set two states,
    // one for the extended key, and one for the regular one
    auto new_key = mapExtendedToRegularKeys(key);
    if (new_key != key)
    {
        updateKeyStateBuffer(new_key, KEY_STATE_NOT_PRESSED);
    }
    updateKeyStateBuffer(key, KEY_STATE_NOT_PRESSED);
}

void InputSystem::Handler_OnLostKeyboardFocus()
{
    clear();
}

void InputSystem::Handler_OnMouseRelativeMovement(i64 x, i64 y)
{
    m_mouse_moved = true;
    m_mouse_raw_movement.x = static_cast<float>(x);
    m_mouse_raw_movement.y = static_cast<float>(y);
}

void InputSystem::Handler_OnMouseMovement(i64 x, i64 y)
{
    m_mouse_position.x = static_cast<float>(x);
    m_mouse_position.y = static_cast<float>(y);
}

void InputSystem::Handler_OnMouseKeyDown(u8 key)
{
    updateKeyStateBuffer(key, KEY_STATE_PRESSED);
}

void InputSystem::Handler_OnMouseKeyUp(u8 key)
{
    updateKeyStateBuffer(key, KEY_STATE_NOT_PRESSED);
}

void InputSystem::Handler_OnMouseWheel(bool direction)
{
    // mouse wheel is rotated forward
    if (direction)
    {
        m_mouse_wheel.first = true;
        m_mouse_wheel.second = MOUSE_WHEEL_FORWARD;
    }
    // mouse wheel is rotate backward
    else
    {
        m_mouse_wheel.first = true;
        m_mouse_wheel.second = MOUSE_WHEEL_BACKWARD;
    }
}

bool InputSystem::IsKeyDown(VIRTUAL_KEY key)
{
    if (key >= 255 || key < 0)
    {
        return false;
    }

    auto key_state = checkPerframeCurrentKeyState(key);
    if (key_state == KEY_STATE_PRESSED)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool InputSystem::IsKeyUp(VIRTUAL_KEY key)
{
    return !IsKeyDown(key);
}

bool InputSystem::IsKeyPressed(VIRTUAL_KEY key)
{
    if (key >= 255 || key < 0)
    {
        return false;
    }

    auto previous_key_state = checkPerframePreviousKeyState(key);
    auto current_key_state = checkPerframeCurrentKeyState(key);
    if (previous_key_state == KEY_STATE_NOT_PRESSED && current_key_state == KEY_STATE_PRESSED)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool InputSystem::IsKeyPressed(VIRTUAL_KEY first_key, VIRTUAL_KEY second_key, VIRTUAL_KEY third_key)
{
    // NOTE: the keys must be pressed on after the other in the order that
    // was specified in the parameters, first_key then first_key only then second_key
    // else this function returns false, this is mainly because we won't be able to catch all the keys
    // IsKeyPressed() to be true at the same (it returns true only for one frame, so we can't catch all in one frame)
    // instead we used a combination of IsKeyDown which returns true in multiple frames alongside one IsKeyPressed
    // in order for key combinations to work.

    // check the state of first_key early so we don't have to check twice for both combination types (two and three)
    auto first_key_down = IsKeyDown(first_key);

    // handle two keys combination
    if (third_key == KEY_EMPTY)
    {
        if (first_key_down && IsKeyPressed(second_key))
        {
            return true;
        }
    }
    // handle three keys combination
    else
    {
        if (first_key_down && IsKeyDown(second_key))
        {
            if (IsKeyPressed(third_key))
            {
                return true;
            }
        }
    }

    return false;
}

bool InputSystem::IsKeyReleased(VIRTUAL_KEY key)
{
    if (key >= 255 || key < 0)
    {
        return false;
    }

    auto previous_key_state = checkPerframePreviousKeyState(key);
    auto current_key_state = checkPerframeCurrentKeyState(key);
    if (previous_key_state == KEY_STATE_PRESSED && current_key_state == KEY_STATE_NOT_PRESSED)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void InputSystem::clear()
{
    // reset all the key states
    m_keyStateBuffer.fill(KEY_STATE_NOT_PRESSED);
    m_perframe_currentKeyState.fill(KEY_STATE_NOT_PRESSED);
    m_perframe_previousKeyState.fill(KEY_STATE_NOT_PRESSED);
    // reset all mouse states
    m_mouse_perframe_position = { 0.0f, 0.0f };
    m_mouse_position = { 0.0f, 0.0f };
    m_mouse_perframe_delta = { 0.0f, 0.0f };
    m_mouse_raw_movement = { 0.0f, 0.0f };
    m_mouse_moved = false;
}

void InputSystem::updateKeyStateBuffer(u8 key, KEY_STATE state)
{
    m_keyStateBuffer[key] = state;
}

InputSystem::KEY_STATE InputSystem::checkCurrentKeyStateBuffer(u8 key)
{
    return m_keyStateBuffer[key];
}

InputSystem::KEY_STATE InputSystem::checkPerframePreviousKeyState(u8 key)
{
    return m_perframe_previousKeyState[key];
}

InputSystem::KEY_STATE InputSystem::checkPerframeCurrentKeyState(u8 key)
{
    return m_perframe_currentKeyState[key];
}

u8 InputSystem::mapExtendedToRegularKeys(u8 vk)
{
    switch (vk)
    {
    case KEY_LSHIFT:
    case KEY_RSHIFT:
    {
        return KEY_SHIFT;
    }
    case KEY_LCONTROL:
    case KEY_RCONTROL:
    {
        return KEY_CONTROL;
    }
    case KEY_LALT:
    case KEY_RALT:
    {
        return KEY_ALT;
    }
    case KEY_NUMPAD_0:
        return KEY_0;
    case KEY_NUMPAD_1:
        return KEY_1;
    case KEY_NUMPAD_2:
        return KEY_2;
    case KEY_NUMPAD_3:
        return KEY_3;
    case KEY_NUMPAD_4:
        return KEY_4;
    case KEY_NUMPAD_5:
        return KEY_5;
    case KEY_NUMPAD_6:
        return KEY_6;
    case KEY_NUMPAD_7:
        return KEY_7;
    case KEY_NUMPAD_8:
        return KEY_8;
    case KEY_NUMPAD_9:
        return KEY_9;
    default:
        return vk;
    }
}

vec2f InputSystem::GetMouseRelativeMovement()
{
    return m_mouse_perframe_delta;
}

vec2f InputSystem::GetMousePosition()
{
    return m_mouse_perframe_position;
}

bool InputSystem::IsMouseWheelRotated(MOUSE_WHEEL_DIRECTION dir)
{
    // check if the current mouse wheel direction is the same as the user asked for
    if (dir == m_mouse_wheel.second)
    {
        // return the current mouse state
        return m_mouse_wheel.first;
    }
    else
    {
        return false;
    }
}
