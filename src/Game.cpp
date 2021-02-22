#include "Game.hpp"

#include "Renderer.hpp"

#include "Input.hpp"
#include "Sprite.hpp"

void Game::Init(SDL_Window *pWindow)
{
    m_pWindow = pWindow;
}

void Game::Frame(float delta)
{
    static std::shared_ptr<Sprite> GodSprite =   std::make_shared<Sprite>("asset/image/god.jpg");
    static std::shared_ptr<Sprite> MarioSprite = std::make_shared<Sprite>("asset/image/gansta_mario.jpg");

    auto [width, height] = GetGameWindowSize();

    Renderer::Get().RenderBegin();

    Renderer::Get().RenderQuad(glm::vec2(width / 2 + 250, height / 2), glm::vec4(0.4f, 0.7f, 0.3f, 1.0f));

    Renderer::Get().RenderSprite(MarioSprite, glm::vec2(width / 2 - 500, height / 2 - 100), glm::vec2(0.4f));

    Renderer::Get().RenderSprite(GodSprite, glm::vec2(width / 2, height / 2), glm::vec2(0.4f));

    Renderer::Get().RenderEnd();
}

std::pair<int, int> Game::GetGameWindowSize()
{
    int w, h;
    SDL_GetWindowSize(m_pWindow, &w, &h);
    return std::make_pair(w, h);
}