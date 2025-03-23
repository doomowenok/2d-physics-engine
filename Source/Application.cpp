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

    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;
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
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int x;
                    int y;
                    SDL_GetMouseState(&x, &y);
                    Particle* particle = new Particle(x, y, 1.0f, 5);
                    particles.push_back(particle);
                }
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

    // Vec2 wind = Vec2(1.0f, 0.0f) * PIXELS_PER_METER;
    Vec2 weight = Vec2(0.0f, 9.8f) * PIXELS_PER_METER;

    for (Particle* particle: particles)
    {
        // particle->AddForce(wind);
        particle->AddForce(weight * particle->Mass);
        particle->AddForce(pushForce);

        if (particle->Position.y >= liquid.y)
        {
            Vec2 drag = Force::GenerateDragForce(*particle, 0.01);
            particle->AddForce(drag);
        }
    }

    for (Particle* particle: particles)
    {
        particle->Integrate(deltaTime);
    }

    for (Particle* particle: particles)
    {
        if (particle->Position.x - particle->Radius <= 0)
        {
            particle->Position.x = particle->Radius;
            particle->Velocity.x *= -0.9f;
        } else if (particle->Position.x + particle->Radius >= Graphics::Width())
        {
            particle->Position.x = Graphics::Width() - particle->Radius;
            particle->Velocity.x *= -0.9f;
        }

        if (particle->Position.y - particle->Radius <= 0)
        {
            particle->Position.y = particle->Radius;
            particle->Velocity.y *= -0.9f;
        } else if (particle->Position.y + particle->Radius >= Graphics::Height())
        {
            particle->Position.y = Graphics::Height() - particle->Radius;
            particle->Velocity.y *= -0.9f;
        }
    }
}

void Application::Render()
{
    Graphics::ClearScreen(0xFF056263);

    Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF6E3713);

    for (Particle* particle: particles)
    {
        Graphics::DrawFillCircle((int) particle->Position.x, (int) particle->Position.y, particle->Radius, 0xFFFFFFFF);
    }

    Graphics::RenderFrame();
}

void Application::Destroy()
{
    for (Particle* particle: particles)
    {
        delete particle;
    }

    Graphics::CloseWindow();
}