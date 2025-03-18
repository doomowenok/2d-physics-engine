#include "Application.h"

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
    particle->Velocity = Vec2(2.0f, 0.0f);

    particle->Position += particle->Velocity;
}

void Application::Render()
{
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle(particle->Position.x, particle->Position.y, 4, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

void Application::Destroy()
{
    // TODO: destroy all objects in the scene

    delete particle;
    Graphics::CloseWindow();
}