#include "Game.hpp"

#include "Renderer.hpp"

#include "Input.hpp"
#include "Sprite.hpp"

Sprite *sprite;

void Game::Init(SDL_Window *pWindow)
{
    m_pWindow = pWindow;
    
    sprite = new Sprite("asset/image/god.png");
}

void Game::Frame()
{
    // Process

    auto [width, height] = GetGameWindowSize();

    Renderer::Get().RenderBegin();

    Renderer::Get().RenderQuad(glm::vec2(width / 2 + 250, height / 2), glm::vec4(0.4f, 0.7f, 0.3f, 1.0f));

    Renderer::Get().RenderSprite(*sprite, glm::vec2(width / 2, height / 2));

    Renderer::Get().RenderEnd();
}

std::pair<int, int> Game::GetGameWindowSize()
{
    int w, h;
    SDL_GetWindowSize(m_pWindow, &w, &h);
    return std::make_pair(w, h);
}