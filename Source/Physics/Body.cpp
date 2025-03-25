#include "Body.h"

Body::Body(float x, float y, float mass, int radius)
{
    this->Position = Vec2(x, y);
    this->Mass = mass;
    this->Radius = radius;
    this->InverseMass = mass == 0.0f ? 0.0f : 1.0f / mass;
}

void Body::Integrate(float deltaTime)
{
    // Acceleration = SumForces / Mass;
    Acceleration = SumForces * InverseMass;

    Velocity += Acceleration * deltaTime;
    Position += Velocity * deltaTime;

    ClearForces();
}

void Body::AddForce(const Vec2& force)
{
    SumForces += force;
}

void Body::ClearForces()
{
    SumForces = Vec2(0.0f, 0.0f);
}

Body::~Body() = default;