#include "CollisionDetection.h"
#include "Contact.h"
#include <limits>

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact)
{
    const bool aIsCircle = a->shape->GetType() == CIRCLE;
    const bool bIsCircle = b->shape->GetType() == CIRCLE;

    const bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    const bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;

    if(aIsCircle && bIsCircle)
    {
        return IsCollidingCircleCircle(a, b, contact);
    }

    if(aIsPolygon && bIsPolygon)
    {
        return IsCollidingPolygonPolygon(a, b, contact);
    }

    if((aIsCircle && bIsPolygon) || (aIsPolygon && bIsCircle))
    {
        return IsCollidingPolygonCircle(a, b, contact);
    }

    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact)
{
    const CircleShape* aCircleShape = (CircleShape*) a->shape;
    const CircleShape* bCircleShape = (CircleShape*) b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= radiusSum * radiusSum;

    if(!isColliding)
    {
        return false;
    }

    contact.a = a;
    contact.b = b;

    contact.normal = ab;
    contact.normal.Normalize();

    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.end = a->position + contact.normal * aCircleShape->radius;

    contact.depth = (contact.end - contact.start).Magnitude();

    return true;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact)
{
    const PolygonShape* aPolygonShape = (PolygonShape*) a->shape;
    const PolygonShape* bPolygonShape = (PolygonShape*) b->shape;

    Vec2 aAxis;
    Vec2 bAxis;
    Vec2 aPoint;
    Vec2 bPoint;

    float abSeparation = aPolygonShape->FindMinimimSeparation(bPolygonShape, aAxis, aPoint);

    if (abSeparation >= 0)
    {
        return false;
    }

    float baSeparation = bPolygonShape->FindMinimimSeparation(aPolygonShape, bAxis, bPoint);

    if (baSeparation >= 0)
    {
        return false;
    }

    return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* a, Body* b, Contact& contact)
{
    return false;
}
