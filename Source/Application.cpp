#include "Application.h"
#include "Physics/Constants.h"
#include "Graphics.h"

bool Application::IsRunning()
{
    return running;
}

void Application::Setup()
{
    running = Graphics::OpenWindow();

    particle = new Particle(50, 100, 1.0f, 4);
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

    float deltaTime = (float)(SDL_GetTicks() - timePreviousFrame) / MILLISECONDS_IN_SECOND;

    if(deltaTime > 0.016)
    {
        deltaTime = 0.016;
    }

    timePreviousFrame = SDL_GetTicks();

    particle->Acceleration = Vec2(2.0f, 9.8f) * PIXELS_PER_METER;

    particle->Velocity += particle->Acceleration * deltaTime;
    particle->Position += particle->Velocity * deltaTime;

    if(particle->Position.x - particle->Radius <= 0)
    {
        particle->Position.x = particle->Radius;
        particle->Velocity.x *= -1.0f;
    }
    else if(particle->Position.x + particle->Radius >= Graphics::Width())
    {
        particle->Position.x = Graphics::Width() - particle->Radius;
        particle->Velocity.x *= -1.0f;
    }

    if(particle->Position.y - particle->Radius <= 0)
    {
        particle->Position.y = particle->Radius;
        particle->Velocity.y *= -1.0f;
    }
    else if(particle->Position.y + particle->Radius >= Graphics::Height())
    {
        particle->Position.y = Graphics::Height() - particle->Radius;
        particle->Velocity.y *= -1.0f;
    }
}

void Application::Render()
{
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle((int)particle->Position.x, (int)particle->Position.y, particle->Radius, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

void Application::Destroy()
{
    delete particle;
    Graphics::CloseWindow();
}