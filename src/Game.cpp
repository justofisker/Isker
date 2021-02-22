#include "Game.hpp"

#include "Renderer.hpp"

#include "Input.hpp"

void Game::Init()
{
    
}

void Game::Frame()
{
    // Process

    Renderer::Get().RenderBegin();

    Renderer::Get().RenderEnd();
}