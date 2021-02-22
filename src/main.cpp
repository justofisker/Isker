#include <SDL2/SDL.h>
#include <iostream>

#include "Renderer.hpp"
#include "Game.hpp"
#include "Input.hpp"

int main()
{
    SDL_assert(SDL_Init(SDL_INIT_VIDEO) == 0);

    SDL_Window *pWindow = SDL_CreateWindow("Ikser",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    
    SDL_assert(pWindow);

    Renderer::Get().Init(pWindow);
    Game::Get().Init();
    
    bool bRunning = 1;
    SDL_Event event;

    while(bRunning)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                bRunning = false;
                break;
            case SDL_WINDOWEVENT:
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if(!event.key.repeat)
                    Input::Get().HandleKeyboard(event.key.keysym.scancode, event.key.state);
                break;
            case SDL_MOUSEMOTION:
                Input::Get().HandleMouseMovement(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                Input::Get().HandleMouseButton(event.button.button, event.button.state);
                break;
            case SDL_MOUSEWHEEL:
                Input::Get().HandleMouseWheel(event.wheel.y);
                break;
            }
        }

        Game::Get().Frame();
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}