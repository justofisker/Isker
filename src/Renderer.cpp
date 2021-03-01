#include "Renderer.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Input.hpp"
#include "Game.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

void Renderer::Init(SDL_Window *pWindow)
{
    SDL_GLContext context = SDL_GL_CreateContext(pWindow);

    if (!context)
    {
        SDL_Log("Failed to create OpenGL context!\n");
        exit(0);
    }
    
    if (!gladLoadGL())
    {
        SDL_Log("Failed to Load glad!\n");
        exit(0);
    }

    m_pWindow = pWindow;
    m_OpenGLContext = context;

    CreateQuadBuffer(MAX_QUADS);

    m_2DShader = std::make_unique<Shader>("asset/shader/color_vert.glsl", "asset/shader/color_frag.glsl");
    m_2DShader->Bind();
    int textures[32] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
    m_2DShader->SetUniform1iv("u_Textures", textures, 32);
    m_iDrawCalls = 0;

    int width, height;
    SDL_GetWindowSize(m_pWindow, &width, &height);
    glViewport(0, 0, width, height);
    m_RenderWindowSize = glm::vec2(width, height);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void Renderer::RenderBegin()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderTexturedQuad(std::shared_ptr<Texture> sprite, const Transform2D &transform)
{
    int slot = -1;
    for(int i = 0; i < 32; i++)
    {
        std::shared_ptr<Texture> curSlot = m_TextureSlots[i].lock();
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

    glm::vec2 bottom_left  = glm::vec2(-sprite->GetWidth(), -sprite->GetHeight());
    glm::vec2 bottom_right = glm::vec2( sprite->GetWidth(), -sprite->GetHeight());
    glm::vec2 top_right    = glm::vec2( sprite->GetWidth(),  sprite->GetHeight());
    glm::vec2 top_left     = glm::vec2(-sprite->GetWidth(),  sprite->GetHeight());

    if(transform.rotation != 0.0f) {
        bottom_left  = glm::rotate(bottom_left , transform.rotation);
        bottom_right = glm::rotate(bottom_right, transform.rotation);
        top_right    = glm::rotate(top_right   , transform.rotation);
        top_left     = glm::rotate(top_left    , transform.rotation);
    }

    m_Vertices[0 + m_QuadCount * 4] = Vertex{ glm::vec2(bottom_left .x * transform.scale.x / 2 + transform.translation.x, bottom_left .y * transform.scale.y / 2 + transform.translation.y), glm::vec2(0.0f, 0.0f), glm::vec4(1.0f), (float)slot };
    m_Vertices[1 + m_QuadCount * 4] = Vertex{ glm::vec2(bottom_right.x * transform.scale.x / 2 + transform.translation.x, bottom_right.y * transform.scale.y / 2 + transform.translation.y), glm::vec2(1.0f, 0.0f), glm::vec4(1.0f), (float)slot };
    m_Vertices[2 + m_QuadCount * 4] = Vertex{ glm::vec2(top_right   .x * transform.scale.x / 2 + transform.translation.x, top_right   .y * transform.scale.y / 2 + transform.translation.y), glm::vec2(1.0f, 1.0f), glm::vec4(1.0f), (float)slot };
    m_Vertices[3 + m_QuadCount * 4] = Vertex{ glm::vec2(top_left    .x * transform.scale.x / 2 + transform.translation.x, top_left    .y * transform.scale.y / 2 + transform.translation.y), glm::vec2(0.0f, 1.0f), glm::vec4(1.0f), (float)slot };
    m_QuadCount++;

    if(m_QuadCount == MAX_QUADS)
        DrawQuadBuffer();
}


void Renderer::RenderQuad(const Transform2D &transform, const glm::vec4 &color)
{
    glm::vec2 bottom_left  = glm::vec2(-transform.scale.x / 2.0f, -transform.scale.y / 2.0f);
    glm::vec2 bottom_right = glm::vec2( transform.scale.x / 2.0f, -transform.scale.y / 2.0f);
    glm::vec2 top_right    = glm::vec2( transform.scale.x / 2.0f,  transform.scale.y / 2.0f);
    glm::vec2 top_left     = glm::vec2(-transform.scale.x / 2.0f,  transform.scale.y / 2.0f);

    if(transform.rotation != 0.0f) {
        bottom_left  = glm::rotate(bottom_left , transform.rotation);
        bottom_right = glm::rotate(bottom_right, transform.rotation);
        top_right    = glm::rotate(top_right   , transform.rotation);
        top_left     = glm::rotate(top_left    , transform.rotation);
    }

    m_Vertices[0 + m_QuadCount * 4] = Vertex{ glm::vec2(bottom_left .x + transform.translation.x, bottom_left .y + m_RenderWindowSize.y - transform.translation.y), glm::vec2(0.0f, 0.0f), color, -1.0f };
    m_Vertices[1 + m_QuadCount * 4] = Vertex{ glm::vec2(bottom_right.x + transform.translation.x, bottom_right.y + m_RenderWindowSize.y - transform.translation.y), glm::vec2(1.0f, 0.0f), color, -1.0f };
    m_Vertices[2 + m_QuadCount * 4] = Vertex{ glm::vec2(top_right   .x + transform.translation.x, top_right   .y + m_RenderWindowSize.y - transform.translation.y), glm::vec2(1.0f, 1.0f), color, -1.0f };
    m_Vertices[3 + m_QuadCount * 4] = Vertex{ glm::vec2(top_left    .x + transform.translation.x, top_left    .y + m_RenderWindowSize.y - transform.translation.y), glm::vec2(0.0f, 1.0f), color, -1.0f };
    m_QuadCount++;

    if(m_QuadCount == MAX_QUADS)
        DrawQuadBuffer();
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

    unsigned int *indices = new unsigned int[6 * max_count];

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

    delete[] indices;

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

    glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);

    m_2DShader->Bind();
    m_2DShader->SetUniformMat4("u_MVP", projection);

    for(int i = 0; i < 32; i++)
    {
        if(std::shared_ptr<Texture> slot = m_TextureSlots[i].lock())
        {
            slot->Bind(i);
            m_TextureSlots[i] = std::weak_ptr<Texture>();
        } else break;
    }

    glBindVertexArray(m_QuadBufferVetexArrayObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4 * m_QuadCount, m_Vertices.data());
    glDrawElements(GL_TRIANGLES, m_QuadCount * 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    
    m_QuadCount = 0;
    m_iDrawCalls++;
}

void Renderer::OnResize(int width, int height)
{
    glViewport(0, 0, width, height);
    m_RenderWindowSize = glm::vec2(width, height);
}
