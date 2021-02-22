#include "Input.hpp"

void Input::Init()
{
    m_MousePosition[0] = 0;
    m_MousePosition[1] = 0;
    m_MouseMotion[0] = 0;
    m_MouseMotion[1] = 0;
    m_MouseWheelDirection = 0;
    memset(m_KeyState, 0, sizeof(m_KeyState));
    memset(m_MouseButtonState, 0, sizeof(m_MouseButtonState));
}

void Input::Frame()
{
    m_MouseMotion[0] = 0;
    m_MouseMotion[1] = 0;
    m_MouseWheelDirection = 0;

    for(int i = 0; i < SDL_NUM_SCANCODES; i++)
        m_KeyState[i] = m_KeyState[i] & ~BUTTONSTATEFLAG_JUST_PRESSED;

    for(int i = 0; i < 5; i++)
        m_MouseButtonState[i] = m_MouseButtonState[i] & ~BUTTONSTATEFLAG_JUST_PRESSED;
}

void Input::HandleKeyboard(int key, bool state)
{
    if(key < 0 || key >= SDL_NUM_SCANCODES) return;
    m_KeyState[key] = state * (BUTTONSTATEFLAG_PRESSED | BUTTONSTATEFLAG_JUST_PRESSED);
}

void Input::HandleMouseMovement(int x, int y, int relx, int rely)
{
    m_MousePosition[0] = x;
    m_MousePosition[1] = y;
    m_MouseMotion[0] += relx;
    m_MouseMotion[1] += rely;
}

void Input::HandleMouseButton(int button, bool state)
{
    if(button < 0 || button >= 5) return;
    m_MouseButtonState[button] = (unsigned char)state * (BUTTONSTATEFLAG_PRESSED | BUTTONSTATEFLAG_JUST_PRESSED);
}

void Input::HandleMouseWheel(int direction)
{
    m_MouseWheelDirection += direction;
}

bool Input::IsKeyJustPressed(int key)
{
    key = SDL_GetScancodeFromKey(key);
    if(key < 0 || key >= SDL_NUM_SCANCODES) return false;
    return m_KeyState[key] & BUTTONSTATEFLAG_JUST_PRESSED;
}

bool Input::IsKeyPressed(int key)
{
    key = SDL_GetScancodeFromKey(key);
    if(key < 0 || key >= SDL_NUM_SCANCODES) return false;
    return m_KeyState[key] & BUTTONSTATEFLAG_PRESSED;
}

bool Input::IsMouseButtonJustPressed(int button)
{
    if(button < 0 || button >= SDL_NUM_SCANCODES) return false;
    return m_MouseButtonState[button] & BUTTONSTATEFLAG_JUST_PRESSED;
}

bool Input::IsMouseButtonPressed(int button)
{
    if(button < 0 || button >= SDL_NUM_SCANCODES) return false;
    return m_MouseButtonState[button] & BUTTONSTATEFLAG_PRESSED;
}

std::pair<int, int> Input::GetMousePosition()
{
    return std::make_pair(m_MousePosition[0], m_MousePosition[1]);
}

std::pair<int, int> Input::GetMouseMovement()
{
    return std::make_pair(m_MouseMotion[0], m_MouseMotion[1]);
}

int Input::GetMouseWheelDirection()
{
    return m_MouseWheelDirection;
}