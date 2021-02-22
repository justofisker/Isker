#pragma once

#include <SDL2/SDL.h>
#include "Singleton.hpp"
#include "Sprite.hpp"

/*
 * The whole point of this is so that I wont have to write OpenGL
 * code outside of this class. Future me, please dont write ANY
 * OpenGL calls or anything outside of here. So that in the future
 * I might be able to change to Vulkan and for my sanity of not
 * having to deal with raw OpenGL
 */

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

class Renderer {
    SINGLETON(Renderer);
private:
    SDL_Window *m_pWindow;
    SDL_GLContext m_OpenGLContext;    
public:
    void Init(SDL_Window *pWindow);

    void RenderBegin();
    void RenderSprite(const Sprite &sprite, const glm::vec2 &translation);
    void RenderQuad(const glm::vec2 &translation, const glm::vec4 &color);
    void RenderEnd();
};