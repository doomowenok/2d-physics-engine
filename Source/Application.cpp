#include "Application.h"
#include "Physics/Constants.h"
#include "Graphics.h"
#include "Physics/Force.h"

bool Application::IsRunning() const
{
    return running;
}

void Application::Setup()
{
    running = Graphics::OpenWindow();
    world = new World(-9.8f);

    Body* floor = new Body(
        BoxShape(Graphics::Width() - 50, 50),
        Graphics::Width() / 2.0,
        Graphics::Height() - 50,
        0.0f,
        1.0f,
        0.7f);
    world->AddBody(floor);

    Body* leftWall = new Body(
        BoxShape(50, Graphics::Height() - 100),
        50, Graphics::Height() / 2.0 - 25,
        0.0f,
        0.2f,
        0.7f);
    world->AddBody(leftWall);

    Body* rightWall = new Body(
        BoxShape(50, Graphics::Height() - 100),
        Graphics::Width() - 50,
        Graphics::Height() / 2.0 - 25,
        0.0f,
        0.2f,
        0.7f);
    world->AddBody(rightWall);

    Body* bigBox = new Body(
        BoxShape(200, 200),
        Graphics::Width() / 2.0,
        Graphics::Height() / 2.0,
        0.0f,
        0.2f,
        0.7f);
    bigBox->SetTexture("../Assets/crate.png");
    world->AddBody(bigBox);

    Body* ball = new Body(
        CircleShape(100),
        Graphics::Width() / 2.0 + 400,
        Graphics::Height() / 2.0 + 100,
        0.0f,
        0.2f,
        0.7f);
    ball->SetTexture("../Assets/basketball.png");
    world->AddBody(ball);
}

void Application::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                if (event.key.keysym.sym == SDLK_d)
                {
                    debug = !debug;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                std::vector<Vec2> vertices =
                {
                    Vec2(20, 60),
                    Vec2(-40, 20),
                    Vec2(-20, -60),
                    Vec2(20, -60),
                    Vec2(40, 20),
                };
                Body* polygon = new Body(PolygonShape(vertices), x, y, 2.0f, 0.1f, 0.7f);
                world->AddBody(polygon);
                break;
        }
    }
}

void Application::Update()
{
    Graphics::ClearScreen(0xFF056263);

    static int timePreviousFrame;

    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);

    if (timeToWait > 0)
    {
        SDL_Delay(timeToWait);
    }

    float deltaTime = (float) (SDL_GetTicks() - timePreviousFrame) / MILLISECONDS_IN_SECOND;

    if (deltaTime > 0.016)
    {
        deltaTime = 0.016;
    }

    timePreviousFrame = SDL_GetTicks();

    world->Update(deltaTime);
}

void Application::Render()
{
    for (const Body* body: world->GetBodies())
    {
        if (body->shape->GetType() == CIRCLE)
        {
            const CircleShape* circleShape = static_cast<CircleShape*>(body->shape);

            if(!debug && body->texture)
            {
                Graphics::DrawTexture(body->position.x, body->position.y, circleShape->radius * 2, circleShape->radius * 2, body->rotation, body->texture);
            }
            else
            {
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFF00FF00);
            }
        }

        if (body->shape->GetType() == BOX)
        {
            const BoxShape* boxShape = static_cast<BoxShape*>(body->shape);

            if(!debug && body->texture)
            {
                Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
            }
            else
            {
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, 0xFF00FF00);
            }
        }

        if (body->shape->GetType() == POLYGON)
        {
            const PolygonShape* polygonShape = static_cast<PolygonShape*>(body->shape);

            if (!debug)
            {
                Graphics::DrawFillPolygon(body->position.x, body->position.y, polygonShape->worldVertices, 0xFF444444);
            }
            else
            {
                Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, 0xFF00FF00);
            }
        }
    }

    Graphics::RenderFrame();
}

void Application::Destroy()
{
    Graphics::CloseWindow();

    delete world;
}
