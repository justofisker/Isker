#include "Game.hpp"

#include <glm/gtc/constants.hpp>
#include <box2d/box2d.h>

#include "Input.hpp"
#include "Render/Renderer.hpp"
#include "Render/Texture.hpp"
#include "Component/Transform2D.hpp"

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
    static std::shared_ptr<Texture> rotatingTexture    = std::make_shared<Texture>("asset/image/rotating.png");
    static std::shared_ptr<Texture> backgroundTexture  = std::make_shared<Texture>("asset/image/background.png");
    static std::shared_ptr<Texture> subTextureTest0    = std::make_shared<Texture>("asset/image/subtexturetest.png");
    static std::shared_ptr<SubTexture> subTextureTest1 = std::make_shared<SubTexture>(subTextureTest0, 100, 100, 900, 900);
    static std::shared_ptr<SubTexture> subTextureTest2 = std::make_shared<SubTexture>(subTextureTest1, 100, 100, 800, 800);
    static std::shared_ptr<Font> robotoFont = std::make_shared<Font>(FontBuilder(), "asset/font/Roboto/Roboto-Regular.ttf", 34); 

    auto& RenderSize = Renderer::Get().GetRenderSize();

    Renderer::Get().RenderBegin();

    static float theta = 0.0f;
    theta = fmodf(theta + delta, glm::pi<float>() * 2.0f);

    {
        const int size = 5;
        for(int x = 0; x < size; x++)
        {
            for(int y = 0; y < size; y++)
            {
                Renderer::Get().RenderTexturedQuad(backgroundTexture, Transform2D(glm::vec2(RenderSize.x * (x + 0.5f) / (float)size, RenderSize.y * (y + 0.5f) / (float)size), glm::vec2(0.4f)));
            }
        }
    }
    
    Renderer::Get().RenderTexturedQuad(subTextureTest0, Transform2D(glm::vec2(200.0f, 200.0f), glm::vec2(0.2f)));
    Renderer::Get().RenderTexturedQuad(subTextureTest1, Transform2D(glm::vec2(400.0f, 200.0f), glm::vec2(0.2f)));
    Renderer::Get().RenderTexturedQuad(subTextureTest2, Transform2D(glm::vec2(600.0f, 200.0f), glm::vec2(0.2f)));

    Renderer::Get().RenderQuad(Transform2D(glm::vec2(RenderSize.x / 2 + 250, RenderSize.y / 2), glm::vec2(100.0f, 100.0f), glm::pi<float>() / 4.0f), glm::vec4(0.4f, 0.7f, 0.3f, 1.0f));

    Renderer::Get().RenderTexturedQuad(rotatingTexture, Transform2D(glm::vec2(RenderSize.x / 2 + sinf(theta) * 150, RenderSize.y / 2), glm::vec2(0.4f), theta));

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
        
        Renderer::Get().RenderQuad(Transform2D(glm::vec2(scale * bodyPos.x   + RenderSize.x / 2.0f, RenderSize.y - scale * bodyPos.y   - 100), glm::vec2(1.0f) * scale         , bodyRotation), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
        Renderer::Get().RenderQuad(Transform2D(glm::vec2(scale * groundPos.x + RenderSize.x / 2.0f, RenderSize.y - scale * groundPos.y - 100), glm::vec2(50.0f, 10.0f) * scale, groundRotation));
    }

    {
        const static unsigned int fps_history_count = 15;
        static int fps_pos = 0;
        static std::array<float, fps_history_count> fps_history = { 0.0f };
        fps_history[fps_pos] = delta;
        fps_pos = (fps_pos + 1) % fps_history_count;
        float fps = 0.0f;
        for(auto f : fps_history)
            fps += f;
        fps /= fps_history_count;
        fps = 1.0f / fps;
        Renderer::Get().RenderText((glm::ivec2)RenderSize - glm::ivec2(20), robotoFont, std::string("FPS: ").append(std::to_string((int)ceilf(fps))), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), Renderer::TextHAlign::Right, Renderer::TextVAlign::Bottom);
    }

    Renderer::Get().RenderEnd();
}