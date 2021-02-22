#include "Renderer.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Input.hpp"
#include "Game.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"

void Renderer::Init(SDL_Window *pWindow)
{
    SDL_GLContext context = SDL_GL_CreateContext(pWindow);

    SDL_assert(context);

    SDL_assert(gladLoadGL());

    m_pWindow = pWindow;
    m_OpenGLContext = context;

    CreateQuadBuffer(MAX_QUADS);

    m_2DShader = std::make_unique<Shader>("asset/shader/color_vert.glsl", "asset/shader/color_frag.glsl");
    m_2DShader->Bind();
    int textures[32] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
    m_2DShader->SetUniform1iv("u_Textures", textures, 32);
}

void Renderer::RenderBegin()
{
    int width, height;
    SDL_GetWindowSize(m_pWindow, &width, &height);
    glViewport(0, 0, width, height);
    if(Input::Get().IsKeyPressed(SDLK_l))
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    else
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderSprite(std::shared_ptr<Sprite> sprite, const glm::vec2 &translation, const glm::vec2 &scale, float rotation)
{
    int slot = -1;
    for(int i = 0; i < 32; i++)
    {
        std::shared_ptr<Sprite> curSlot = m_TextureSlots[i].lock();
        if(!curSlot)
        {
            m_TextureSlots[i] = sprite;
            slot = i;
            break;
        }
        if(curSlot == sprite)
        {
            slot = i;
            break;
        }
    }
    if(slot == -1)
    {
        SDL_Log("OUT OF TEXTURE SLOTS!\n");
        return;
    }
    m_Vertices[0 + m_QuadCount * 4] = Vertex{ glm::vec2(-sprite->GetWidth() * scale.x / 2 + translation.x, -sprite->GetHeight() * scale.y / 2 + translation.y), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f), (float)slot };
    m_Vertices[1 + m_QuadCount * 4] = Vertex{ glm::vec2( sprite->GetWidth() * scale.x / 2 + translation.x, -sprite->GetHeight() * scale.y / 2 + translation.y), glm::vec2(1.0f, 0.0f), glm::vec4(1.0f), (float)slot };
    m_Vertices[2 + m_QuadCount * 4] = Vertex{ glm::vec2( sprite->GetWidth() * scale.x / 2 + translation.x,  sprite->GetHeight() * scale.y / 2 + translation.y), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f), (float)slot };
    m_Vertices[3 + m_QuadCount * 4] = Vertex{ glm::vec2(-sprite->GetWidth() * scale.x / 2 + translation.x,  sprite->GetHeight() * scale.y / 2 + translation.y), glm::vec2(0.0f, 1.0f), glm::vec4(1.0f), (float)slot };
    m_QuadCount++;
}


void Renderer::RenderQuad(const glm::vec2 &translation, const glm::vec4 &color)
{
    m_Vertices[0 + m_QuadCount * 4] = Vertex{ glm::vec2(-100.0f + translation.x, -100.0f + translation.y), glm::vec2(0.0f, 0.0f), color, -1 };
    m_Vertices[1 + m_QuadCount * 4] = Vertex{ glm::vec2( 100.0f + translation.x, -100.0f + translation.y), glm::vec2(1.0f, 0.0f), color, -1 };
    m_Vertices[2 + m_QuadCount * 4] = Vertex{ glm::vec2( 100.0f + translation.x,  100.0f + translation.y), glm::vec2(1.0f, 1.0f), color, -1 };
    m_Vertices[3 + m_QuadCount * 4] = Vertex{ glm::vec2(-100.0f + translation.x,  100.0f + translation.y), glm::vec2(0.0f, 1.0f), color, -1 };
    m_QuadCount++;
}

void Renderer::RenderEnd()
{
    if(m_QuadCount)
    {
        int width, height;
        SDL_GetWindowSize(m_pWindow, &width, &height);
        glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);

        m_2DShader->Bind();
        m_2DShader->SetUniformMat4("u_MVP", projection);

        for(int i = 0; i < 32; i++)
        {
            if(std::shared_ptr<Sprite> slot = m_TextureSlots[i].lock())
            {
                slot->Bind(i);
                m_TextureSlots[i] = std::weak_ptr<Sprite>();
            } else break;
        }

        glBindVertexArray(m_QuadBufferVetexArrayObject);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4 * m_QuadCount, m_Vertices);
        glDrawElements(GL_TRIANGLES, m_QuadCount * 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        
        m_QuadCount = 0;
    }
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

    unsigned int *indices = (unsigned int*)malloc(sizeof(unsigned int) * 6 * max_count);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6 * max_count, indices, GL_STATIC_DRAW);

    free((void*)indices);

    glBindVertexArray(0);
    
    m_QuadBufferVetexArrayObject = vao;
    m_QuadBufferVertexBuffer = vb;
    m_QuadBufferIndexBuffer = ib;
}
