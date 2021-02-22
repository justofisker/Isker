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

#include "Shader.hpp"

#include <memory>

class Renderer {
    SINGLETON(Renderer);
private:
    const int MAX_QUADS = 1024;
    SDL_Window *m_pWindow;
    SDL_GLContext m_OpenGLContext;
    std::unique_ptr<Shader> m_2DShader;
    unsigned int m_QuadBufferVetexArrayObject,
                 m_QuadBufferVertexBuffer,
                 m_QuadBufferIndexBuffer;
    struct Vertex
    {
        glm::vec2 position;
        glm::vec2 uv;
        glm::vec4 color;
        float texture;
    };
    Vertex m_Vertices[1024 * 4];
    int m_QuadCount;
    std::weak_ptr<Sprite> m_TextureSlots[32];
public:
    void Init(SDL_Window *pWindow);

    void RenderBegin();
    void RenderSprite(std::shared_ptr<Sprite> sprite, const glm::vec2 &translation, const glm::vec2 &scale = glm::vec2(1.0f), float rotation = 0.0f);
    void RenderQuad(const glm::vec2 &translation, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1.0f));
    void RenderEnd();
private:
    void CreateQuadBuffer(int max_count);
};