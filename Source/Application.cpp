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

//    // Chain
//    for (int i = 0; i < NUM_PARTICLES; i++)
//    {
//        bodies.push_back(new Body(anchor.x, anchor.y + restLength * i, 2, 6));
//    }

    // Soft-body
    bodies.push_back(new Body(100, 100, 2.0f, 6));
    bodies.push_back(new Body(100 + restLength, 100, 2.0f, 6));
    bodies.push_back(new Body(100 + restLength, 100 - restLength, 2.0f, 6));
    bodies.push_back(new Body(100, 100 - restLength, 2.0f, 6));
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
                if (event.key.keysym.sym == SDLK_UP)
                {
                    pushForce.y = 50.0 * PIXELS_PER_METER;
                }
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    pushForce.x = 50.0 * PIXELS_PER_METER;
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    pushForce.y = -50.0 * PIXELS_PER_METER;
                }
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    pushForce.x = -50.0 * PIXELS_PER_METER;
                }
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                {
                    pushForce.y = 0.0f;
                }
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    pushForce.x = 0.0f;
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    pushForce.y = 0.0f;
                }
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    pushForce.x = 0.0f;
                }
                break;
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
                {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
                {
                    leftMouseButtonDown = false;
                    int particleIndex = 0;
                    // int particleIndex = NUM_PARTICLES - 1;
                    Vec2 impulseDirection = (bodies[particleIndex]->Position - mouseCursor).UnitVector();
                    float impulseMagnitude = (bodies[particleIndex]->Position - mouseCursor).Magnitude() * 5.0;
                    bodies[particleIndex]->Velocity = impulseDirection * impulseMagnitude;
                }
                break;
        }
    }
}

void Application::Update()
{
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
        // body->AddForce(pushForce);

        Vec2 drag = Force::GenerateDragForce(*body, 0.003f);
        body->AddForce(drag);

        Vec2 weight = Vec2(0.0f, body->Mass * 9.8f * PIXELS_PER_METER);
        body->AddForce(weight);
    }

//    // Chain
//    bodies[0]->AddForce(Force::GenerateSpringForce(*bodies[0], anchor, restLength, k));
//
//    for (int i = 1; i < NUM_PARTICLES; i++)
//    {
//        Vec2 springForce = Force::GenerateSpringForce(*bodies[i], *bodies[i - 1], restLength, k);
//        bodies[i]->AddForce(springForce);
//        bodies[i - 1]->AddForce(-springForce);
//    }

    // Soft-body
    for (Body* a : bodies)
    {
        for (Body* b : bodies)
        {
            if(a == b)
            {
                continue;
            }

            Vec2 springForce = Force::GenerateSpringForce(*a, *b, restLength, k);
            a->AddForce(springForce);
            b->AddForce(-springForce);
        }
    }

    for (Body* body: bodies)
    {
        body->Integrate(deltaTime);
    }

    for (Body* body: bodies)
    {
        if (body->Position.x - body->Radius <= 0)
        {
            body->Position.x = body->Radius;
            body->Velocity.x *= -0.9f;
        } else if (body->Position.x + body->Radius >= Graphics::Width())
        {
            body->Position.x = Graphics::Width() - body->Radius;
            body->Velocity.x *= -0.9f;
        }

        if (body->Position.y - body->Radius <= 0)
        {
            body->Position.y = body->Radius;
            body->Velocity.y *= -0.9f;
        } else if (body->Position.y + body->Radius >= Graphics::Height())
        {
            body->Position.y = Graphics::Height() - body->Radius;
            body->Velocity.y *= -0.9f;
        }
    }
}

void Application::Render()
{
    Graphics::ClearScreen(0xFF056263);

    if (leftMouseButtonDown)
    {
        Graphics::DrawLine(
                bodies[0]->Position.x,
                bodies[0]->Position.y,
                mouseCursor.x,
                mouseCursor.y,
                0xFF0000FF);
    }

//    // Chain
//    Graphics::DrawLine(anchor.x, anchor.y, bodies[0]->Position.x, bodies[0]->Position.y, 0xFF313131);
//
//    for (int i = 1; i < NUM_PARTICLES; i++)
//    {
//        Graphics::DrawLine(bodies[i]->Position.x, bodies[i]->Position.y, bodies[i - 1]->Position.x,
//                           bodies[i - 1]->Position.y, 0xFF313131);
//    }

    // Soft-body
    for (Body* a : bodies)
    {
        for (Body* b : bodies)
        {
            if(a == b)
            {
                continue;
            }

            Vec2 springForce = Force::GenerateSpringForce(*a, *b, restLength, k);
            a->AddForce(springForce);
            b->AddForce(-springForce);

            Graphics::DrawLine(a->Position.x, a->Position.y, b->Position.x, b->Position.y, 0xFF313131);
        }
    }

    for (Body* body: bodies)
    {
        Graphics::DrawFillCircle(body->Position.x, body->Position.y, body->Radius, 0xFFFFFFFF);
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