#pragma once

#include "Vec2.h"

struct Particle
{
    Vec2 Position;
    Vec2 Velocity;
    Vec2 Acceleration;
    Vec2 SumForces;

    int Radius;

    float Mass;
    float InverseMass;

    Particle(float x, float y, float mass, int radius);
    ~Particle();

    void AddForce(const Vec2& force);
    void ClearForces();
    void Integrate(float deltaTime);
};