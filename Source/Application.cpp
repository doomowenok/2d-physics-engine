#include "Application.h"
#include "Physics/Constants.h"
#include "Graphics.h"
#include "Physics/Force.h"

bool Application::IsRunning()
{
    return running;
}

void Application::Setup()
{
    running = Graphics::OpenWindow();

    anchor = Vec2(Graphics::Width() / 2.0f, 30.0f);

    Body* box = new Body(BoxShape(200, 100), Graphics::Width() / 2, Graphics::Height() / 2, 1.0f);
    bodies.push_back(box);
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
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
                {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
                {
                    leftMouseButtonDown = false;
                    int particleIndex = 0;
                    // int particleIndex = NUM_PARTICLES - 1;
                    Vec2 impulseDirection = (bodies[particleIndex]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (bodies[particleIndex]->position - mouseCursor).Magnitude() * 5.0;
                    bodies[particleIndex]->velocity = impulseDirection * impulseMagnitude;
                }
                break;
        }
    }
}

void Application::Update()
{
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
        // Vec2 weight = Vec2(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
        // body->AddForce(weight);

         float torque = 2000.0f;
         body->AddTorque(torque);
    }

    for(Body* body: bodies)
    {
        body->Update(deltaTime);
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
    Graphics::ClearScreen(0xFF056263);

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
        if(body->shape->GetType() == CIRCLE)
        {
            CircleShape* circleShape = (CircleShape*) body->shape;
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFFFFFFFF);
        }
        else if (body->shape->GetType() == BOX)
        {
            BoxShape* boxShape = (BoxShape*) body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, 0xFFFFFFFF);
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