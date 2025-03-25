#include "Force.h"
#include <algorithm>

Vec2 Force::GenerateDragForce(const Body& particle, float k)
{
    Vec2 dragForce = Vec2(0.0f, 0.0f);

    if(particle.velocity.MagnitudeSquared() > 0.0f)
    {
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0f;
        float dragMagnitude = k * particle.velocity.MagnitudeSquared();
        dragForce = dragDirection * dragMagnitude;
    }

    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Body& particle, float k)
{
    Vec2 frictionForce = Vec2(0.0f, 0.0f);
    Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0f;
    float frictionMagnitude = k;
    frictionForce = frictionDirection * frictionMagnitude;
    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Body& a, const Body& b, float g, float minDistance, float maxDistance)
{
    Vec2 direction = b.position - a.position;
    float distanceSquared = direction.MagnitudeSquared();

    distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);

    Vec2 attractionDirection = direction.UnitVector();
    float attractionMagnitude = g * (a.mass * b.mass) / distanceSquared;

    Vec2 attractionForce = attractionDirection * attractionMagnitude;
    return attractionForce;
}

Vec2 Force::GenerateSpringForce(const Body& particle, Vec2 anchor, float restLength, float k)
{
    Vec2 direction = particle.position - anchor;
    float displacement = direction.Magnitude() - restLength;

    Vec2 springDirection = direction.UnitVector();
    float springMagnitude = -k * displacement;

    Vec2 springForce = springDirection * springMagnitude;
    return springForce;
}

Vec2 Force::GenerateSpringForce(const Body& a, const Body& b, float restLength, float k)
{
    return GenerateSpringForce(a, b.position, restLength, k);
}
