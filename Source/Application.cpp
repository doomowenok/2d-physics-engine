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

    Particle* smallPlanet = new Particle(100, 100, 1.0f, 6);
    particles.push_back(smallPlanet);

    Particle* bigPlanet = new Particle(500, 500, 100000000.0f, 20);
    particles.push_back(bigPlanet);
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
                    Vec2 impulseDirection = (particles[0]->Position - mouseCursor).UnitVector();
                    float impulseMagnitude = (particles[0]->Position - mouseCursor).Magnitude() * 5.0;
                    particles[0]->Velocity = impulseDirection * impulseMagnitude;
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

//    Vec2 weight = Vec2(0.0f, 9.8f) * PIXELS_PER_METER;

    for (Particle* particle: particles)
    {
//        particle->AddForce(weight * particle->Mass);
        particle->AddForce(pushForce);

        Vec2 friction = Force::GenerateFrictionForce(*particle, 10.0f * PIXELS_PER_METER);
        particle->AddForce(friction);
    }

    Vec2 attraction = Force::GenerateGravitationalForce(*particles[0], *particles[1], 1.0f);
    particles[0]->AddForce(attraction);
    particles[1]->AddForce(-attraction);

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

    if (leftMouseButtonDown)
    {
        Graphics::DrawLine(particles[0]->Position.x, particles[0]->Position.y, mouseCursor.x, mouseCursor.y,0xFF0000FF);
    }

    int count = 0;
    for (Particle* particle: particles)
    {
        Graphics::DrawFillCircle((int) particle->Position.x, (int) particle->Position.y, particle->Radius, count % 2 == 0 ? 0xFFAA3300 : 0xFF00FFFF);
        count++;
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