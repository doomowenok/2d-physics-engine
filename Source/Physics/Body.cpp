#include "Body.h"

Body::Body(const Shape& shape, float x, float y, float mass)
{
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->mass = mass;
    this->inverseMass = mass == 0.0f ? 0.0f : 1.0f / mass;
}

Body::~Body()
{
    delete shape;
}

void Body::Integrate(float deltaTime)
{
    // acceleration = sumForces / mass;
    acceleration = sumForces * inverseMass;

    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    ClearForces();
}

void Body::AddForce(const Vec2& force)
{
    sumForces += force;
}

void Body::ClearForces()
{
    sumForces = Vec2(0.0f, 0.0f);
}