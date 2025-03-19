#include "Particle.h"

Particle::Particle(float x, float y, float mass, int radius)
{
    this->Position = Vec2(x, y);
    this->Mass = mass;
    this->Radius = radius;
}

void Particle::Integrate(float deltaTime)
{
    Velocity += Acceleration * deltaTime;
    Position += Velocity * deltaTime;
}

Particle::~Particle() = default;