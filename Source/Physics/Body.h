#pragma once

#include "Vec2.h"

struct Body
{
    Vec2 Position;
    Vec2 Velocity;
    Vec2 Acceleration;
    Vec2 SumForces;

    int Radius;

    float Mass;
    float InverseMass;

    Body(float x, float y, float mass, int radius);
    ~Body();

    void AddForce(const Vec2& force);
    void ClearForces();
    void Integrate(float deltaTime);
};