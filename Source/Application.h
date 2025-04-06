#pragma once

#include "Graphics.h"
#include "Physics/Body.h"
#include "Physics/World.h"
#include <vector>

class Application
{
private:
    bool debug = false;
    bool running = false;
    World* world;

public:
    Application() = default;

    ~Application() = default;

    bool IsRunning() const;

    void Setup();

    void Input();

    void Update();

    void Render();

    void Destroy();
};
