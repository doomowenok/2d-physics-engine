#pragma once

#include "Graphics.h"
#include "Physics/Particle.h"
#include <vector>

class Application
{
    private:
        bool running = false;

        std::vector<Particle*> particles;
        Vec2 pushForce = Vec2(0.0f, 0.0f);
        SDL_Rect liquid;

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};