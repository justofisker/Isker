#include "Game.hpp"

#include "Renderer.hpp"
#include <glm/gtc/constants.hpp>
#include <box2d/box2d.h>

#include "Input.hpp"
#include "Texture.hpp"

b2World *world;
b2Body *groundBody;
b2Body *body;
int32 velocityIterations = 8;
int32 positionIterations = 3;

void Game::Init(SDL_Window *pWindow)
{
    m_pWindow = pWindow;

    b2Vec2 gravity(0.0f, -10.0f);
    world = new b2World(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 7.0f);
    bodyDef.angle = glm::pi<float>() / 3.0f;
    body = world->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
}

void Game::Frame(float delta)
{
    static std::shared_ptr<Texture> GodSprite =   std::make_shared<Texture>("asset/image/340.png");
    static std::shared_ptr<Texture> MarioSprite = std::make_shared<Texture>("asset/image/gansta_mario.jpg"); 

    glm::vec2 WindowSize = GetGameWindowSize();

    Renderer::Get().RenderBegin();

    static float theta = 0.0f;
    theta = fmodf(theta + delta, glm::pi<float>() * 2.0f);

    {
        const int size = 5;
        for(int x = 0; x < size; x++)
        {
            for(int y = 0; y < size; y++)
            {
                Renderer::Get().RenderTexturedQuad(MarioSprite, Transform2D(glm::vec2(WindowSize.x * (x + 0.5f) / (float)size, WindowSize.y * (y + 0.5f) / (float)size), glm::vec2(0.4f)));
            }
        }
    }

    Renderer::Get().RenderQuad(Transform2D(glm::vec2(WindowSize.x / 2 + 250, WindowSize.y / 2), glm::vec2(100.0f, 100.0f), glm::pi<float>() / 4.0f), glm::vec4(0.4f, 0.7f, 0.3f, 1.0f));

    Renderer::Get().RenderTexturedQuad(GodSprite, Transform2D(glm::vec2(WindowSize.x / 2 + sinf(theta) * 150, WindowSize.y / 2), glm::vec2(0.4f), theta));

    {
        if(Input::Get().IsKeyJustPressed(SDLK_SPACE))
            body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, 30.0f), true);

        float direction = 0.0f;

        if(Input::Get().IsKeyPressed(SDLK_LEFT) || Input::Get().IsKeyPressed(SDLK_a))
            direction -= 1.0f;
        if(Input::Get().IsKeyPressed(SDLK_RIGHT) || Input::Get().IsKeyPressed(SDLK_d))
            direction += 1.0f;  

        body->ApplyForceToCenter(b2Vec2(direction * 50.0f, 0.0f), true); 
        
        float scale = 30.0f;

        world->Step(delta, velocityIterations, positionIterations);
        b2Vec2 bodyPos = body->GetPosition();
        float bodyRotation = body->GetAngle();
        b2Vec2 groundPos = groundBody->GetPosition();
        float groundRotation = groundBody->GetAngle();
        
        Renderer::Get().RenderQuad(Transform2D(glm::vec2(scale * bodyPos.x   + WindowSize.x / 2.0f, WindowSize.y - scale * bodyPos.y   - 100), glm::vec2(1.0f) * scale         , bodyRotation), glm::vec4(0.3f, 0.4f, 0.5f, 1.0f));
        Renderer::Get().RenderQuad(Transform2D(glm::vec2(scale * groundPos.x + WindowSize.x / 2.0f, WindowSize.y - scale * groundPos.y - 100), glm::vec2(50.0f, 10.0f) * scale, groundRotation));
    }

    Renderer::Get().RenderQuad(Transform2D(Input::Get().GetMousePosition(), glm::vec2(10.0f)));

    Renderer::Get().RenderEnd();
}

glm::vec2 Game::GetGameWindowSize()
{
    int w, h;
    SDL_GetWindowSize(m_pWindow, &w, &h);
    return glm::vec2(w, h);
}