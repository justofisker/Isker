#pragma once

#include "Singleton.hpp"

#include <SDL2/SDL.h>

#include <utility>

class Input
{
    SINGLETON(Input);
private:
    typedef unsigned char ButtonState;
    enum ButtonStateFlags {
        BUTTONSTATEFLAG_PRESSED = 1 << 0,
        BUTTONSTATEFLAG_JUST_PRESSED = 1 << 1,
    };
private:
    int m_MouseMotion[2];
    int m_MousePosition[2];
    int m_MouseWheelDirection;
    ButtonState m_KeyState[SDL_NUM_SCANCODES];
    ButtonState m_MouseButtonState[5];
public:
    void Init();
    void Frame();

    void HandleKeyboard(int key, bool state);
    void HandleMouseMovement(int x, int y, int relx, int rely);
    void HandleMouseButton(int button, bool state);
    void HandleMouseWheel(int direction);

    bool IsKeyJustPressed(int key);
    bool IsKeyPressed(int key);
    bool IsMouseButtonJustPressed(int button);
    bool IsMouseButtonPressed(int button);
    std::pair<int, int> GetMousePosition();
    std::pair<int, int> GetMouseMovement();
    int GetMouseWheelDirection();
    
};
