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

    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        particles.push_back(new Particle(anchor.x, anchor.y + restLength * i, 2, 6));
    }
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
                    int particleIndex = NUM_PARTICLES - 1;
                    Vec2 impulseDirection = (particles[particleIndex]->Position - mouseCursor).UnitVector();
                    float impulseMagnitude = (particles[particleIndex]->Position - mouseCursor).Magnitude() * 5.0;
                    particles[particleIndex]->Velocity = impulseDirection * impulseMagnitude;
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

    for (Particle* particle: particles)
    {
        particle->AddForce(pushForce);

        Vec2 drag = Force::GenerateDragForce(*particle, 0.001f);
        particle->AddForce(drag);

        Vec2 weight = Vec2(0.0f, particle->Mass * 9.8f * PIXELS_PER_METER);
        particle->AddForce(weight);
    }

    particles[0]->AddForce(Force::GenerateSpringForce(*particles[0], anchor, restLength, k));

    for (int i = 1; i < NUM_PARTICLES; i++)
    {
        Vec2 springForce = Force::GenerateSpringForce(*particles[i], *particles[i - 1], restLength, k);
        particles[i]->AddForce(springForce);
        particles[i - 1]->AddForce(-springForce);
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
        }
        else if (particle->Position.x + particle->Radius >= Graphics::Width())
        {
            particle->Position.x = Graphics::Width() - particle->Radius;
            particle->Velocity.x *= -0.9f;
        }

        if (particle->Position.y - particle->Radius <= 0)
        {
            particle->Position.y = particle->Radius;
            particle->Velocity.y *= -0.9f;
        }
        else if (particle->Position.y + particle->Radius >= Graphics::Height())
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
        Graphics::DrawLine(
                particles[NUM_PARTICLES - 1]->Position.x,
                particles[NUM_PARTICLES - 1]->Position.y,
                mouseCursor.x,
                mouseCursor.y,
                0xFF0000FF);
    }

    Graphics::DrawLine(anchor.x, anchor.y, particles[0]->Position.x, particles[0]->Position.y, 0xFF313131);

    for (int i = 1; i < NUM_PARTICLES; i++)
    {
        Graphics::DrawLine(particles[i]->Position.x, particles[i]->Position.y, particles[i - 1]->Position.x, particles[i - 1]->Position.y, 0xFF313131);
    }

    for (Particle* particle: particles)
    {
        Graphics::DrawFillCircle(particle->Position.x, particle->Position.y, particle->Radius, 0xFFFFFFFF);
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