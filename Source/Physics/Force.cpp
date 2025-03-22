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
