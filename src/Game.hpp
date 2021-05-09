#pragma once

#include <SDL.h>
#include <glm/vec2.hpp>

#include "Singleton.hpp"

class Game {
    SINGLETON(Game);
private:
    SDL_Window *m_pWindow = nullptr;
public:
    void Init(SDL_Window *pWindow);
    void Frame(float delta);
};