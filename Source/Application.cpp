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

    Body* boxA = new Body(BoxShape(200, 200), Graphics::Width() / 2, Graphics::Height() / 2, 1.0f, 1.0f);
    boxA->angularVelocity = 0.4f;
    bodies.push_back(boxA);

    Body* boxB = new Body(BoxShape(200, 200), Graphics::Width() / 2, Graphics::Height() / 2, 1.0f, 1.0f);
    boxB->angularVelocity = 0.1f;
    bodies.push_back(boxB);
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
                break;
            case SDL_MOUSEMOTION:
                int x;
                int y;
                SDL_GetMouseState(&x, &y);
                bodies[0]->position.x = x;
                bodies[0]->position.y = y;
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
        // Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
        // body->AddForce(weight);

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
                // contact.ResolveCollision();

                a->isColliding = true;
                b->isColliding = true;
            }
        }
    }

    for (Body* body: bodies)
    {
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape* circleShape = (CircleShape *) body->shape;

            if (!circleShape)
            {
                continue;
            }

            if (body->position.x - circleShape->radius <= 0)
            {
                body->position.x = circleShape->radius;
                body->velocity.x *= -0.9f;
            }
            else if (body->position.x + circleShape->radius >= Graphics::Width())
            {
                body->position.x = Graphics::Width() - circleShape->radius;
                body->velocity.x *= -0.9f;
            }

            if (body->position.y - circleShape->radius <= 0)
            {
                body->position.y = circleShape->radius;
                body->velocity.y *= -0.9f;
            }
            else if (body->position.y + circleShape->radius >= Graphics::Height())
            {
                body->position.y = Graphics::Height() - circleShape->radius;
                body->velocity.y *= -0.9f;
            }
        }
    }
}

void Application::Render()
{
    //    Graphics::ClearScreen(0xFF056263);

    if (leftMouseButtonDown)
    {
        Graphics::DrawLine(
            bodies[0]->position.x,
            bodies[0]->position.y,
            mouseCursor.x,
            mouseCursor.y,
            0xFF0000FF);
    }

    for (Body* body: bodies)
    {
        uint32_t color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;

        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape* circleShape = (CircleShape *) body->shape;
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
        }
        else if (body->shape->GetType() == BOX)
        {
            BoxShape* boxShape = (BoxShape *) body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color);
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
