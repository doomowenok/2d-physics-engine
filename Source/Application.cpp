#include "Application.h"
#include "Physics/Constants.h"
#include "Graphics.h"
#include "Physics/Force.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Contact.h"

bool Application::IsRunning()
{
    return running;
}

void Application::Setup()
{
    running = Graphics::OpenWindow();

    anchor = Vec2(Graphics::Width() / 2.0f, 30.0f);

    Body* ball0 = new Body(CircleShape(100), 200, 200, 1.0f);
    bodies.push_back(ball0);

    Body* ball1 = new Body(CircleShape(50), 500, 100, 1.0f);
    bodies.push_back(ball1);
}

void Application::Input()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                if(event.key.keysym.sym == SDLK_UP)
                {
                    pushForce.y = 50.0 * PIXELS_PER_METER;
                }
                if(event.key.keysym.sym == SDLK_RIGHT)
                {
                    pushForce.x = 50.0 * PIXELS_PER_METER;
                }
                if(event.key.keysym.sym == SDLK_DOWN)
                {
                    pushForce.y = -50.0 * PIXELS_PER_METER;
                }
                if(event.key.keysym.sym == SDLK_LEFT)
                {
                    pushForce.x = -50.0 * PIXELS_PER_METER;
                }
                break;
            case SDL_KEYUP:
                if(event.key.keysym.sym == SDLK_UP)
                {
                    pushForce.y = 0.0f;
                }
                if(event.key.keysym.sym == SDLK_RIGHT)
                {
                    pushForce.x = 0.0f;
                }
                if(event.key.keysym.sym == SDLK_DOWN)
                {
                    pushForce.y = 0.0f;
                }
                if(event.key.keysym.sym == SDLK_LEFT)
                {
                    pushForce.x = 0.0f;
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

    if(timeToWait > 0)
    {
        SDL_Delay(timeToWait);
    }

    float deltaTime = (float) (SDL_GetTicks() - timePreviousFrame) / MILLISECONDS_IN_SECOND;

    if(deltaTime > 0.016)
    {
        deltaTime = 0.016;
    }

    timePreviousFrame = SDL_GetTicks();

    for(Body* body: bodies)
    {
//         Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
//         body->AddForce(weight);
//
//         Vec2 wind = Vec2(20.0f, 0.0f) * PIXELS_PER_METER;
//         body->AddForce(wind);

         float torque = 0.0f;
         body->AddTorque(torque);
    }

    for(Body* body: bodies)
    {
        body->Update(deltaTime);
    }

    for(Body* body: bodies)
    {
        body->isColliding = false;
    }

    for(int i = 0; i < bodies.size() - 1; i++)
    {
        for(int j = i + 1; j < bodies.size(); j++)
        {
            Body* a = bodies[i];
            Body* b = bodies[j];

            Contact contact;

            if(CollisionDetection::IsColliding(a, b, contact))
            {
                Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * contact.depth, contact.start.y + contact.normal.y * contact.depth, 0xFFFF00FF);
                a->isColliding = true;
                b->isColliding = true;
            }
        }
    }

    for(Body* body: bodies)
    {
        if(body->shape->GetType() == CIRCLE)
        {
            CircleShape* circleShape = (CircleShape*) body->shape;

            if(!circleShape)
            {
                continue;
            }

            if(body->position.x - circleShape->radius <= 0)
            {
                body->position.x = circleShape->radius;
                body->velocity.x *= -0.9f;
            }
            else if(body->position.x + circleShape->radius >= Graphics::Width())
            {
                body->position.x = Graphics::Width() - circleShape->radius;
                body->velocity.x *= -0.9f;
            }

            if(body->position.y - circleShape->radius <= 0)
            {
                body->position.y = circleShape->radius;
                body->velocity.y *= -0.9f;
            }
            else if(body->position.y + circleShape->radius >= Graphics::Height())
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

    if(leftMouseButtonDown)
    {
        Graphics::DrawLine(
                bodies[0]->position.x,
                bodies[0]->position.y,
                mouseCursor.x,
                mouseCursor.y,
                0xFF0000FF);
    }

    for(Body* body: bodies)
    {
        uint32_t color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;

        if(body->shape->GetType() == CIRCLE)
        {
            CircleShape* circleShape = (CircleShape*) body->shape;
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
        }
        else if (body->shape->GetType() == BOX)
        {
            BoxShape* boxShape = (BoxShape*) body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color);
        }
    }

    Graphics::RenderFrame();
}

void Application::Destroy()
{
    for(Body* particle: bodies)
    {
        delete particle;
    }

    Graphics::CloseWindow();
}