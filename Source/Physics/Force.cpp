#include "Force.h"

Vec2 Force::GenerateDragForce(const Particle& particle, float k)
{
    Vec2 dragForce = Vec2(0.0f, 0.0f);

    if(particle.Velocity.MagnitudeSquared() > 0.0f)
    {
        Vec2 dragDirection = particle.Velocity.UnitVector() * -1.0f;
        float dragMagnitude = k * particle.Velocity.MagnitudeSquared();
        dragForce = dragDirection * dragMagnitude;
    }

    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle& particle, float k)
{
    Vec2 frictionForce = Vec2(0.0f, 0.0f);
    Vec2 frictionDirection = particle.Velocity.UnitVector() * -1.0f;
    float frictionMagnitude = k;
    frictionForce = frictionDirection * frictionMagnitude;
    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Particle& a, const Particle& b, float g)
{
    Vec2 direction = b.Position - a.Position;
    float distanceSquared = direction.MagnitudeSquared();

    Vec2 attractionDirection = direction.UnitVector();
    float attractionMagnitude = g * (a.Mass * b.Mass) / distanceSquared;

    Vec2 attractionForce = attractionDirection * attractionMagnitude;
    return attractionForce;
}
