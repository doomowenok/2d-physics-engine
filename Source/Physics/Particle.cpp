#include "Particle.h"

Particle::Particle(float x, float y, float mass, int radius)
{
    this->Position = Vec2(x, y);
    this->Mass = mass;
    this->Radius = radius;
    this->InverseMass = mass == 0.0f ? 0.0f : 1.0f / mass;
}

void Particle::Integrate(float deltaTime)
{
    // Acceleration = SumForces / Mass;
    Acceleration = SumForces * InverseMass;

    Velocity += Acceleration * deltaTime;
    Position += Velocity * deltaTime;

    ClearForces();
}

void Particle::AddForce(const Vec2& force)
{
    SumForces += force;
}

void Particle::ClearForces()
{
    SumForces = Vec2(0.0f, 0.0f);
}

Particle::~Particle() = default;