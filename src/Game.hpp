#pragma once

#include "Singleton.hpp"

#include <SDL2/SDL.h>

#include <utility>

class Game {
    SINGLETON(Game);
private:
    SDL_Window *m_pWindow;
public:
    void Init(SDL_Window *pWindow);
    void Frame();
    std::pair<int, int> GetGameWindowSize();
};