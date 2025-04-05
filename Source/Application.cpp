#include "Application.h"
#include "Physics/Constants.h"
#include "Graphics.h"
#include "Physics/Force.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Contact.h"

bool Application::IsRunning() const
{
    return running;
}

void Application::Setup()
{
    running = Graphics::OpenWindow();

    Body* floor = new Body(
        BoxShape(Graphics::Width() - 50, 50),
        Graphics::Width() / 2.0,
        Graphics::Height() - 50,
        0.0f,
        1.0f,
        0.7f);
    Body* leftWall = new Body(
        BoxShape(50, Graphics::Height() - 100),
        50, Graphics::Height() / 2.0 - 25,
        0.0f,
        0.2f,
        0.7f);
    Body* rightWall = new Body(
        BoxShape(50, Graphics::Height() - 100),
        Graphics::Width() - 50,
        Graphics::Height() / 2.0 - 25,
        0.0f,
        0.2f,
        0.7f);

    bodies.push_back(floor);
    bodies.push_back(leftWall);
    bodies.push_back(rightWall);

    Body* bigBox = new Body(
        BoxShape(200, 200),
        Graphics::Width() / 2.0,
        Graphics::Height() / 2.0,
        0.0f,
        0.2f,
        0.7f);
    bodies.push_back(bigBox);
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
                bodies.push_back(polygon);
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

    for (Body* body: bodies)
    {
        Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
        body->AddForce(weight);

        // Vec2 wind = Vec2(2.0f, 0.0f) * PIXELS_PER_METER;
        // body->AddForce(wind);

        // float torque = 0.0f;
        // body->AddTorque(torque);
    }

    for (Body* body: bodies)
    {
        body->Update(deltaTime);
    }

    for (Body* body: bodies)
    {
        body->isColliding = false;
    }

    for (int i = 0; i < bodies.size() - 1; i++)
    {
        for (int j = i + 1; j < bodies.size(); j++)
        {
            Body* a = bodies[i];
            Body* b = bodies[j];

            Contact contact;

            if (CollisionDetection::IsColliding(a, b, contact))
            {
                contact.ResolveCollision();

                a->isColliding = true;
                b->isColliding = true;

                if (debug)
                {
                    Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                    Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                    Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15,
                                       contact.start.y + contact.normal.y * 15, 0xFFFF00FF);
                }
            }
        }
    }
}

void Application::Render()
{
    for (const Body* body: bodies)
    {
        if (body->shape->GetType() == CIRCLE)
        {
            const CircleShape* circleShape = static_cast<CircleShape*>(body->shape);
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFF00FF00);
        }

        if (body->shape->GetType() == BOX)
        {
            const BoxShape* boxShape = static_cast<BoxShape*>(body->shape);
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, 0xFF00FF00);
        }

        if (body->shape->GetType() == POLYGON)
        {
            const PolygonShape* polygonShape = static_cast<PolygonShape*>(body->shape);
            Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, 0xFF00FF00);
        }
    }

    Graphics::RenderFrame();
}

void Application::Destroy()
{
    for (Body* particle: bodies)
    {
        delete particle;
    }

    Graphics::CloseWindow();
}
