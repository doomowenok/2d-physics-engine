#include "Contact.h"

void Contact::ResolvePenetration() const
{
    if(a->IsStatic() && b->IsStatic())
    {
        return;
    }

    float da = depth / (a->inverseMass + b->inverseMass) * a->inverseMass;
    float db = depth / (a->inverseMass + b->inverseMass) * b->inverseMass;

    a->position -= normal * da;
    b->position += normal * db;
}

void Contact::ResolveCollision() const
{
    ResolvePenetration();

    const float elasticity = std::min(a->restitution, b->restitution);
    const Vec2 relevantVelocity = a->velocity - b->velocity;
    const float relevantVelocityDotNormal = relevantVelocity.Dot(normal);

    const Vec2 impulseDirection = normal;
    const float impulseMagnitude = -(1 + elasticity) * relevantVelocityDotNormal / (a->inverseMass + b->inverseMass);

    Vec2 j = impulseDirection * impulseMagnitude;

    a->ApplyImpulse(j);
    b->ApplyImpulse(-j);
}
