#include "Application.h"
#include "Physics/Constants.h"

bool Application::IsRunning()
{
    return running;
}

void Application::Setup()
{
    running = Graphics::OpenWindow();

    particle = new Particle(50, 100, 1.0f);
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

    particle->Velocity = Vec2(50.0f, 10.0f) * deltaTime;

    particle->Position += particle->Velocity;
}

void Application::Render()
{
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle((int)particle->Position.x, (int)particle->Position.y, 4, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

void Application::Destroy()
{
    delete particle;
    Graphics::CloseWindow();
}