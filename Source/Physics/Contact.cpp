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

    Vec2 ra = end - a->position;
    Vec2 rb = start - b->position;

    // Linear + Angular velocities
    Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);

    const Vec2 relativeVelocity = va - vb;

    const float relativeVelocityDotNormal = relativeVelocity.Dot(normal);

    const Vec2 impulseDirection = normal;
    const float impulseMagnitude =
        -(1 + elasticity) * relativeVelocityDotNormal
        / ((a->inverseMass + b->inverseMass) + ra.Cross(normal) * ra.Cross(normal) * a->inverseI + rb.Cross(normal) * rb.Cross(normal) * b->inverseI);

    Vec2 j = impulseDirection * impulseMagnitude;

    a->ApplyImpulse(j, ra);
    b->ApplyImpulse(-j, rb);
}
