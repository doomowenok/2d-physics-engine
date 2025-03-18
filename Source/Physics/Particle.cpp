#include "Particle.h"

Particle::Particle(float x, float y, float mass)
{
    this->Position = Vec2(x, y);
    this->Mass = mass;
}

Particle::~Particle() = default;