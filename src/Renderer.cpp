#include "Renderer.hpp"

#include <glad/glad.h>

#include "Input.hpp"

void Renderer::Init(SDL_Window *pWindow)
{
    SDL_GLContext context = SDL_GL_CreateContext(pWindow);

    SDL_assert(context);

    SDL_assert(gladLoadGL());

    m_pWindow = pWindow;
    m_OpenGLContext = context;
}

void Renderer::RenderBegin()
{
    if(Input::Get().IsKeyPressed(SDLK_l))
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    else
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderSprite(const Sprite &sprite)
{

}

void Renderer::RenderEnd()
{
    SDL_GL_SwapWindow(m_pWindow);
}