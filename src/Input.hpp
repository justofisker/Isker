#pragma once

#include <array>

#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

#include "Singleton.hpp"

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
    std::array<int, 2> m_MouseMotion;
    std::array<int, 2> m_MousePosition;
    int m_MouseWheelDirection;
    std::array<ButtonState, SDL_NUM_SCANCODES> m_KeyState;
    std::array<ButtonState, 5> m_MouseButtonState;
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
    glm::vec2 GetMousePosition();
    glm::vec2 GetMouseMovement();
    int GetMouseWheelDirection();
    
};
