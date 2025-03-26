#include "CollisionDetection.h"

bool CollisionDetection::IsColliding(Body* a, Body* b)
{
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;

    if(aIsCircle && bIsCircle)
    {
        return IsCollidingCircleCircle(a, b);
    }

    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b)
{
    CircleShape* aCircleShape = (CircleShape*) a->shape;
    CircleShape* bCircleShape = (CircleShape*) b->shape;

    float distance = (b->position - a->position).MagnitudeSquared();
    float radiusSumSquared = aCircleShape->radius * aCircleShape->radius + bCircleShape->radius * bCircleShape->radius;

    return distance <= radiusSumSquared;
}
