#include "Renderer.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif


#include "../Input.hpp"
#include "../Game.hpp"

void Renderer::Init(SDL_Window *pWindow)
{
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    #if __EMSCRIPTEN__
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #else
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    #endif

    SDL_GLContext context = SDL_GL_CreateContext(pWindow);

    if (!context)
    {
        SDL_Log("Failed to create OpenGL context!\n");
        exit(0);
    }

    if (!gladLoadGLES2Loader(SDL_GL_GetProcAddress))
    {
        SDL_Log("Failed to Load glad!\n");
        exit(0);
    }

    m_pWindow = pWindow;
    m_OpenGLContext = context;

    CreateQuadBuffer(MAX_QUADS);

    m_2DShader = std::make_unique<Shader>("asset/shader/color_vert.glsl", "asset/shader/color_frag.glsl");
    m_2DShader->Bind();
    int textures[MAX_TEXTURE_IMAGE_UNITS] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    m_2DShader->SetIntArray("u_Textures", MAX_TEXTURE_IMAGE_UNITS, textures);
    m_iDrawCalls = 0;

    m_TextureSlots.fill(~0u);

    int width, height;
    SDL_GetWindowSize(m_pWindow, &width, &height);
    glViewport(0, 0, width, height);
    m_GameSize = glm::vec2(width, height);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_SCISSOR_TEST);
}

void Renderer::RenderBegin()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int w, h;
    SDL_GetWindowSize(m_pWindow, &w, &h);

    float aspect = (float)w / (float)h;
    float targetAspect = m_GameSize.x / m_GameSize.y;

    int left = 0, bottom = 0, width = w, height = h;

    float aspectDiff = targetAspect / aspect;
    if(aspectDiff < 1.0) {
        left = (int)(w * (1.0f - aspectDiff) / 2.0f );
        width = (int)(w * aspectDiff);
    }
    if(aspectDiff > 1.0)
    {
        bottom = (int)(h * (1.0f - (aspect / targetAspect)) / 2.0f );
        height = (int)(h / aspectDiff);
    }
    
    glScissor(left , bottom, width, height);
}

void Renderer::RenderTexturedQuad(std::shared_ptr<Texture> sprite, const glm::mat4 &transform)
{
    int slot = GetBufferTextureSlot(sprite->GetTextureID());

    glm::vec4 bottom_left  = transform * glm::vec4(-sprite->GetWidth() / 2.0f, -sprite->GetHeight() / 2.0f, 0.0f, 1.0f);
    glm::vec4 bottom_right = transform * glm::vec4( sprite->GetWidth() / 2.0f, -sprite->GetHeight() / 2.0f, 0.0f, 1.0f);
    glm::vec4 top_right    = transform * glm::vec4( sprite->GetWidth() / 2.0f,  sprite->GetHeight() / 2.0f, 0.0f, 1.0f);
    glm::vec4 top_left     = transform * glm::vec4(-sprite->GetWidth() / 2.0f,  sprite->GetHeight() / 2.0f, 0.0f, 1.0f);

    const Texture::TextureUV& uv = sprite->GetUV();
    const glm::vec2& bottomLeftUV = uv.bottomLeft;
    const glm::vec2& topRightUV = uv.topRight;

    m_Vertices[0 + m_QuadCount * 4] = Vertex{ glm::vec2(bottom_left .x, bottom_left .y), glm::vec2(bottomLeftUV.x,     bottomLeftUV.y    ), glm::vec4(1.0f), (float)slot };
    m_Vertices[1 + m_QuadCount * 4] = Vertex{ glm::vec2(bottom_right.x, bottom_right.y), glm::vec2(topRightUV.x, bottomLeftUV.y    ), glm::vec4(1.0f), (float)slot };
    m_Vertices[2 + m_QuadCount * 4] = Vertex{ glm::vec2(top_right   .x, top_right   .y), glm::vec2(topRightUV.x, topRightUV.y), glm::vec4(1.0f), (float)slot };
    m_Vertices[3 + m_QuadCount * 4] = Vertex{ glm::vec2(top_left    .x, top_left    .y), glm::vec2(bottomLeftUV.x,     topRightUV.y), glm::vec4(1.0f), (float)slot };
    m_QuadCount++;

    if(m_QuadCount == MAX_QUADS)
        DrawQuadBuffer();
}

void Renderer::RenderQuad(const glm::mat4 &transform, const glm::vec4 &color)
{
    glm::vec4 bottom_left  = transform * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
    glm::vec4 bottom_right = transform * glm::vec4( 1.0f, -1.0f, 0.0f, 1.0f);
    glm::vec4 top_right    = transform * glm::vec4( 1.0f,  1.0f, 0.0f, 1.0f);
    glm::vec4 top_left     = transform * glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f);

    m_Vertices[0 + m_QuadCount * 4] = Vertex{ glm::vec2(bottom_left .x, bottom_left .y), glm::vec2(0.0f, 0.0f), color, -1.0f };
    m_Vertices[1 + m_QuadCount * 4] = Vertex{ glm::vec2(bottom_right.x, bottom_right.y), glm::vec2(1.0f, 0.0f), color, -1.0f };
    m_Vertices[2 + m_QuadCount * 4] = Vertex{ glm::vec2(top_right   .x, top_right   .y), glm::vec2(1.0f, 1.0f), color, -1.0f };
    m_Vertices[3 + m_QuadCount * 4] = Vertex{ glm::vec2(top_left    .x, top_left    .y), glm::vec2(0.0f, 1.0f), color, -1.0f };
    m_QuadCount++;

    if(m_QuadCount == MAX_QUADS)
        DrawQuadBuffer();
}

void Renderer::RenderText(const glm::ivec2 &position, std::shared_ptr<Font> font, const std::string &text, const glm::vec4 &color, TextHAlign halign, TextVAlign valign)
{
    int slot = GetBufferTextureSlot(font->GetTexture()->GetTextureID());

    unsigned int x_pos = position.x;
    unsigned int y_pos = (int)Renderer::Get().GetGameSize().y - position.y;

    float y_offset = 0.0f;
    switch(valign)
    {
        case TextVAlign::Top:
            y_offset = -(float)font->GetFontSize();
            break;
        case TextVAlign::Center:
            y_offset = -(float)font->GetFontSize() / 2.0f;
            break;
        case TextVAlign::Bottom:
            y_offset = 0.0f;
            break;
    }

    float x_offset = 0.0f;
    switch (halign)
    {
    case TextHAlign::Left:
        x_offset = 0.0f;
        break;
    case TextHAlign::Center:
        x_offset = -CalculateTextSize(font, text).x / 2.0f;
        break;
    case TextHAlign::Right:
        x_offset = (float)-CalculateTextSize(font, text).x;
        break;
    }
    

    for(char c : text)
    {
        const Font::FontCharacter &character = font->GetCharacter(c);

        m_Vertices[0 + m_QuadCount * 4] = Vertex{ glm::vec2(x_pos                    + x_offset, y_pos + y_offset - (character.size.y - character.bearing.y)), glm::vec2(character.bottomLeftUV.x, character.topRightUV.y), color, (float)slot };
        m_Vertices[1 + m_QuadCount * 4] = Vertex{ glm::vec2(x_pos + character.size.x + x_offset, y_pos + y_offset - (character.size.y - character.bearing.y)), glm::vec2(character.topRightUV.x,   character.topRightUV.y), color, (float)slot };
        m_Vertices[2 + m_QuadCount * 4] = Vertex{ glm::vec2(x_pos + character.size.x + x_offset, y_pos + y_offset - (character.size.y - character.bearing.y) + character.size.y), glm::vec2(character.topRightUV.x,   character.bottomLeftUV.y), color, (float)slot };
        m_Vertices[3 + m_QuadCount * 4] = Vertex{ glm::vec2(x_pos                    + x_offset, y_pos + y_offset - (character.size.y - character.bearing.y) + character.size.y), glm::vec2(character.bottomLeftUV.x, character.bottomLeftUV.y), color, (float)slot };
        m_QuadCount++;

        x_pos += character.advance >> 6;

        if(m_QuadCount == MAX_QUADS) {
            DrawQuadBuffer();
            slot = GetBufferTextureSlot(font->GetTexture()->GetTextureID());
        }
    }
}

glm::ivec2 Renderer::CalculateTextSize(std::shared_ptr<Font> font, const std::string &text)
{
    unsigned int x_size = 0;
    for(int i = 0; i < text.length() - 1; i++)
    {
        const Font::FontCharacter &character = font->GetCharacter((unsigned char)text.at(i));
        x_size += character.advance >> 6;
    }
    x_size += font->GetCharacter((unsigned int)text.at(text.length() - 1)).size.x;
    return glm::ivec2(x_size, font->GetFontSize());
}

void Renderer::RenderEnd()
{
    DrawQuadBuffer();
    //SDL_Log("Draw calls: %d\n", m_iDrawCalls);
    m_iDrawCalls = 0;
    
    SDL_GL_SwapWindow(m_pWindow);

}

void Renderer::CreateQuadBuffer(int max_count)
{
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    unsigned int vb, ib;
    glGenBuffers(1, &vb);
    glGenBuffers(1, &ib);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * max_count, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

    std::unique_ptr<unsigned int[]> indices(new unsigned int[6 * max_count]);

    for(int i = 0; i < max_count; i++)
    {
        indices[i * 6 + 0] = 0 + i * 4;
        indices[i * 6 + 1] = 1 + i * 4;
        indices[i * 6 + 2] = 2 + i * 4;
        indices[i * 6 + 3] = 2 + i * 4;
        indices[i * 6 + 4] = 3 + i * 4;
        indices[i * 6 + 5] = 0 + i * 4;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6 * max_count, indices.get(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    
    m_QuadBufferVetexArrayObject = vao;
    m_QuadBufferVertexBuffer = vb;
    m_QuadBufferIndexBuffer = ib;
}

void Renderer::DrawQuadBuffer()
{
    if(!m_QuadCount) return;

    int width, height;
    SDL_GetWindowSize(m_pWindow, &width, &height);

    glm::mat4 projection = glm::ortho(0.0f, m_GameSize.x, 0.0f, m_GameSize.y, -1.0f, 1.0f);

    m_2DShader->Bind();
    m_2DShader->SetMat4("u_MVP", projection);
    m_2DShader->SetFloat("u_Aspect", (float)width / (float)height);
    m_2DShader->SetFloat("u_TargetAspect", m_GameSize.x / m_GameSize.y);

    for(int i = 0; i < 32; i++)
    {
        if(m_TextureSlots[i] != ~0u)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]);
            m_TextureSlots[i] = ~0u;
        } else break;
    }

    glBindVertexArray(m_QuadBufferVetexArrayObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4 * m_QuadCount, m_Vertices.data());
    glDrawElements(GL_TRIANGLES, m_QuadCount * 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    
    m_QuadCount = 0;
    m_iDrawCalls++;
}

int Renderer::GetBufferTextureSlot(unsigned int textureID)
{
    int slot = -1;
    for(int i = 0; i < 32; i++)
    {
        if(m_TextureSlots[i] == ~0u)
        {
            m_TextureSlots[i] = textureID;
            return i;
        }
        if(m_TextureSlots[i] == textureID)
        {
            return i;
        }
    }
    DrawQuadBuffer();
    return GetBufferTextureSlot(textureID);
}

void Renderer::OnResize(int width, int height)
{
    glViewport(0, 0, width, height);
    // m_GameSize = glm::vec2(width, height);
}
