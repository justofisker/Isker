#pragma once

#include <memory>
#include <array>

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <SDL2/SDL.h>

#include "../Singleton.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Font.hpp"

#define MAX_QUADS 1024
#define MAX_TEXTURE_IMAGE_UNITS 16

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
    std::array<unsigned int, MAX_TEXTURE_IMAGE_UNITS> m_TextureSlots;
    unsigned int m_iDrawCalls;
    glm::vec2 m_GameSize;
public:
    enum class TextVAlign
    {
        Top, Center, Bottom
    };
    enum class TextHAlign
    {
        Left, Center, Right
    };
public:
    void Init(SDL_Window *pWindow);
    void RenderBegin();
    void RenderTexturedQuad(std::shared_ptr<Texture> texture, const glm::mat4 &transform);
    void RenderQuad(const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1.0f));
    void RenderText(const glm::ivec2 &position, std::shared_ptr<Font> font, const std::string &text, const glm::vec4 &color = glm::vec4(1.0f), TextHAlign halign = TextHAlign::Left, TextVAlign valign = TextVAlign::Top);
    glm::ivec2 CalculateTextSize(std::shared_ptr<Font> font, const std::string &text);
    void RenderEnd();
    void OnResize(int width, int height);
    inline const glm::vec2 &GetGameSize() { return m_GameSize; }
private:
    void CreateQuadBuffer(int max_count);
    void DrawQuadBuffer();
    int GetBufferTextureSlot(unsigned int textureID);
};
