#pragma once

#include "Graphics.h"
#include "Physics/Body.h"
#include <vector>

class Application
{
private:
    bool running = false;
    std::vector<Body*> bodies;
    Vec2 pushForce = Vec2(0.0f, 0.0f);
    Vec2 mouseCursor = Vec2(0.0f, 0.0f);
    bool leftMouseButtonDown = false;

    Vec2 anchor;
    float k = 1500.0f;
    float restLength = 200.0f;
    const int NUM_PARTICLES = 15;

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