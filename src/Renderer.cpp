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

// Placeholder until I implement batching
Mesh *Quad;
Shader *ColorShader;
Shader *SpriteShader;
Sprite *sprite;

static void CreateQuad()
{
    struct Vertex
    {
        float x, y;
        float uv[2];
    };

    float vertices[4 * (2 + 2)] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
    };

    unsigned char indices[3 * 2] = {
        0, 1, 2,
        2, 3, 0,
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    unsigned int vb, ib;
    glGenBuffers(1, &vb);
    glGenBuffers(1, &ib);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
    Quad = new Mesh(vao, vb, ib);
}

void Renderer::Init(SDL_Window *pWindow)
{
    SDL_GLContext context = SDL_GL_CreateContext(pWindow);

    SDL_assert(context);

    SDL_assert(gladLoadGL());

    m_pWindow = pWindow;
    m_OpenGLContext = context;

    CreateQuad();
    ColorShader = new Shader("asset/shader/color_vert.glsl", "asset/shader/color_frag.glsl");
    SpriteShader = new Shader("asset/shader/sprite_vert.glsl", "asset/shader/sprite_frag.glsl");
    sprite = new Sprite("asset/image/god.png");
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

void Renderer::RenderSprite(const Sprite &sprite)
{
    int width, height;
    SDL_GetWindowSize(m_pWindow, &width, &height);
    glm::mat4 projection = glm::ortho(0, width, 0, height);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 mvp = projection * view * model;
}


void Renderer::RenderQuad(const glm::vec2 &translation, const glm::vec4 &color)
{
    //ColorShader->Bind();
    //ColorShader->SetUniform4f("u_Color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    SpriteShader->Bind();
    SpriteShader->SetUniform1i("u_Texture", 0);
    sprite->Bind(0);
    glBindVertexArray(Quad->m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
    glBindVertexArray(0);
}

void Renderer::RenderEnd()
{
    SDL_GL_SwapWindow(m_pWindow);
}