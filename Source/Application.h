#pragma once

#include "Graphics.h"
#include "Physics/Particle.h"

class Application
{
    private:
        bool running = false;
        Particle* particle = nullptr;

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