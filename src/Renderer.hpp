#pragma once

#include <memory>
#include <array>

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

#include "Singleton.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

#define MAX_QUADS 1024

class Renderer {
    SINGLETON(Renderer);
private:
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
    std::array<Vertex, MAX_QUADS * 4> m_Vertices;
    int m_QuadCount;
    std::weak_ptr<Texture> m_TextureSlots[32];
    unsigned int m_iDrawCalls;
    glm::vec2 m_RenderWindowSize;
public:
    void Init(SDL_Window *pWindow);
    void RenderBegin();
    void RenderTexturedQuad(std::shared_ptr<Texture> sprite, const glm::vec2 &translation, const glm::vec2 &scale = glm::vec2(1.0f), float rotation = 0.0f);
    void RenderQuad(const glm::vec2 &translation, const glm::vec2 &size, float rotation = 0.0f, const glm::vec4 &color = glm::vec4(1.0f));
    void RenderEnd();
    void OnResize(int width, int height);
private:
    void CreateQuadBuffer(int max_count);
    void DrawQuadBuffer();
};