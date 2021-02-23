#include "Game.hpp"

#include "Renderer.hpp"
#include <glm/gtc/constants.hpp>

#include "Input.hpp"
#include "Texture.hpp"

void Game::Init(SDL_Window *pWindow)
{
    m_pWindow = pWindow;
}

void Game::Frame(float delta)
{
    static std::shared_ptr<Texture> GodSprite =   std::make_shared<Texture>("asset/image/340.png");
    static std::shared_ptr<Texture> MarioSprite = std::make_shared<Texture>("asset/image/gansta_mario.jpg");

    glm::vec2 WindowSize = GetGameWindowSize();

    Renderer::Get().RenderBegin();

    static float theta = 0.0f;
    theta = fmodf(theta + delta, glm::pi<float>() * 2.0f);

    {
        const int size = 5;
        for(int x = 0; x < size; x++)
        {
            for(int y = 0; y < size; y++)
            {
                Renderer::Get().RenderTexturedQuad(MarioSprite, glm::vec2(WindowSize.x * (x + 0.5f) / (float)size, WindowSize.y * (y + 0.5f) / (float)size), glm::vec2(0.4f));
            }
        }
    }

    Renderer::Get().RenderQuad(glm::vec2(WindowSize.x / 2 + 250, WindowSize.y / 2), glm::vec2(200.0f, 200.0f), glm::pi<float>() / 4.0f, glm::vec4(0.4f, 0.7f, 0.3f, 1.0f));

    Renderer::Get().RenderTexturedQuad(GodSprite, glm::vec2(WindowSize.x / 2 + sinf(theta) * 150, WindowSize.y / 2), glm::vec2(0.4f), theta);

    Renderer::Get().RenderQuad(Input::Get().GetMousePosition(), glm::vec2(10.0f), 0.0f);

    Renderer::Get().RenderEnd();
}

glm::vec2 Game::GetGameWindowSize()
{
    int w, h;
    SDL_GetWindowSize(m_pWindow, &w, &h);
    return glm::vec2(w, h);
}