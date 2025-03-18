#include "Particle.h"

Particle::Particle(float x, float y, float mass, float radius)
{
    this->Position = Vec2(x, y);
    this->Mass = mass;
    this->Radius = radius;
}

Particle::~Particle() = default;