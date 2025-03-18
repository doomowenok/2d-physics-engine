#pragma once

#include "Vec2.h"

struct Particle
{
    Vec2 Position;
    Vec2 Velocity;
    Vec2 Acceleration;

    float Mass;

    Particle(float x, float y, float mass);
    ~Particle();
};