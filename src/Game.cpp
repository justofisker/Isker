#include "Game.hpp"

#include "Renderer.hpp"

#include "Input.hpp"
#include "Sprite.hpp"
#include <math.h>

#include <glm/gtc/constants.hpp>

void Game::Init(SDL_Window *pWindow)
{
    m_pWindow = pWindow;
}

void Game::Frame(float delta)
{
    static std::shared_ptr<Sprite> GodSprite =   std::make_shared<Sprite>("asset/image/340.png");
    static std::shared_ptr<Sprite> MarioSprite = std::make_shared<Sprite>("asset/image/gansta_mario.jpg");

    auto [width, height] = GetGameWindowSize();

    Renderer::Get().RenderBegin();

    static float theta = 0.0f;
    theta += delta;

    for(int x = 0; x < 30; x++)
    {
        for(int y = 0; y < 30; y++)
        {
            Renderer::Get().RenderSprite(MarioSprite, glm::vec2(width * x / 30.0f, height * y / 30.0f), glm::vec2(0.4f));
        }
    }

    Renderer::Get().RenderQuad(glm::vec2(width / 2 + 250, height / 2), glm::vec2(200.0f, 200.0f), glm::pi<float>() / 4.0f, glm::vec4(0.4f, 0.7f, 0.3f, 1.0f));

    Renderer::Get().RenderSprite(GodSprite, glm::vec2(width / 2 + sinf(theta) * 150, height / 2), glm::vec2(0.4f), theta);

    Renderer::Get().RenderEnd();
}

std::pair<int, int> Game::GetGameWindowSize()
{
    int w, h;
    SDL_GetWindowSize(m_pWindow, &w, &h);
    return std::make_pair(w, h);
}