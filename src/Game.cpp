#include "Game.hpp"

#include "Renderer.hpp"

#include "Input.hpp"

void Game::Init(SDL_Window *pWindow)
{
    m_pWindow = pWindow;
}

void Game::Frame()
{
    // Process

    Renderer::Get().RenderBegin();

    Renderer::Get().RenderQuad(glm::vec2(0.0f, 0.0f), glm::vec4(1.0f));

    Renderer::Get().RenderEnd();
}

std::pair<int, int> Game::GetGameWindowSize()
{
    int w, h;
    SDL_GetWindowSize(m_pWindow, &w, &h);
    return std::make_pair(w, h);
}