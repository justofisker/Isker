#include "Game.hpp"

#include "Renderer.hpp"

#include "Input.hpp"

void Game::Init()
{
    
}

void Game::Frame()
{
    // Process

    if(Input::Get().IsKeyJustPressed(SDLK_m))
        SDL_Log("M Pressed!\n");
    if(Input::Get().IsKeyPressed(SDLK_m))
        SDL_Log("M Just Pressed!\n");

    Renderer::Get().RenderBegin();

    Renderer::Get().RenderEnd();
}