#include "Body.h"
#include <cmath>
#include "../Graphics.h"

Body::Body(const Shape& shape, float x, float y, float mass, float restitution, float friction)
{
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->velocity = Vec2(0.0f, 0.0f);
    this->acceleration = Vec2(0.0f, 0.0f);
    this->rotation = 0.0f;
    this->angularVelocity = 0.0f;
    this->angularAcceleration = 0.0f;
    this->sumForces = Vec2(0.0f, 0.0f);
    this->sumTorque = 0.0f;
    this->mass = mass;
    this->inverseMass = mass == 0.0f ? 0.0f : 1.0f / mass;
    this->I = shape.GetMomentOfInertia() * mass;
    this->inverseI = I == 0.0f ? 0.0f : 1.0f / I;
    this->restitution = restitution;
    this->friction = friction;
    this->shape->UpdateVertices(this->rotation, this->position);
}

Body::~Body()
{
    delete shape;
    SDL_DestroyTexture(texture);
}

bool Body::IsStatic() const
{
    const float MASS_EPSILON = 0.005f;
    return fabsf(inverseMass - 0.0f) < MASS_EPSILON;
}

void Body::AddForce(const Vec2& force)
{
    sumForces += force;
}

void Body::AddTorque(float torque)
{
    sumTorque += torque;
}

void Body::ClearForces()
{
    sumForces = Vec2(0.0f, 0.0f);
}

void Body::ClearTorque()
{
    sumTorque = 0.0f;
}

Vec2 Body::WorldToLocalSpace(const Vec2& point) const
{
    const Vec2 translation = point - position;
    const float rotatedX = cos(-rotation) * translation.x - sin(-rotation) * translation.y;
    const float rotatedY = cos(-rotation) * translation.y + sin(-rotation) * translation.x;
    Vec2 result = Vec2(rotatedX, rotatedY);
    return result;
}

Vec2 Body::LocalToWorldSpace(const Vec2& point) const
{
    Vec2 rotated = point.Rotate(rotation);
    Vec2 result = rotated + position;
    return result;
}

// Only linear
void Body::ApplyImpulseLinear(const Vec2& impulse)
{
    if(IsStatic())
    {
        return;
    }

    velocity += impulse * inverseMass;
}

// Only angular
void Body::ApplyImpulseAngular(const float impulse)
{
    if(IsStatic())
    {
        return;
    }

    angularVelocity += impulse * inverseI;
}

// Linear + Angular
void Body::ApplyImpulseAtPoint(const Vec2& impulse, const Vec2& r)
{
    if(IsStatic())
    {
        return;
    }

    velocity += impulse * inverseMass;
    angularVelocity += r.Cross(impulse) * inverseI;
}

void Body::IntegrateForces(const float deltaTime)
{
    if(IsStatic())
    {
        return;
    }

    acceleration = sumForces * inverseMass;
    velocity += acceleration * deltaTime;

    angularAcceleration = sumTorque * inverseI;
    angularVelocity += angularAcceleration * deltaTime;

    ClearForces();
    ClearTorque();
}

void Body::IntegrateVelocities(const float deltaTime)
{
    if(IsStatic())
    {
        return;
    }

    position += velocity * deltaTime;
    rotation += angularVelocity * deltaTime;

    shape->UpdateVertices(rotation, position);
}