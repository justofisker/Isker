#pragma once

#include <memory>
#include <array>

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <SDL2/SDL.h>

#include "Singleton.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

#define MAX_QUADS 1024

class Transform2D;

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
    void RenderTexturedQuad(std::shared_ptr<Texture> sprite, const glm::mat4 &transform);
    void RenderQuad(const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1.0f));
    void RenderEnd();
    void OnResize(int width, int height);
private:
    void CreateQuadBuffer(int max_count);
    void DrawQuadBuffer();
};

class Transform2D
{
private:
    glm::vec2 m_Translation;
    glm::vec2 m_Scale;
    float m_Rotation;
    glm::mat4 m_Matrix;
    bool m_DoRecalculateMatrix;
public:
    Transform2D(const glm::vec2 &translation = glm::vec2(0.0f), const glm::vec2 &scale = glm::vec2(1.0f), float rotation = 0.0f);
    inline const glm::vec2 &GetTranslation() { return m_Translation; }
    inline const glm::vec2 &GetScale() { return m_Scale; }
    inline float GetRotation() { return m_Rotation; }
    const glm::mat4 &GetMatrix();
    void SetTranslation(const glm::vec2 translation);
    void SetScale(const glm::vec2 scale);
    void SetRotation(float rotation);
    operator const glm::mat4&()
    {
        if(m_DoRecalculateMatrix)
        {
            RecalculateMatrix();
            m_DoRecalculateMatrix = false;
        }
        return m_Matrix;
    }
private:
    void RecalculateMatrix();
};