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

float FindMinimimSeparation(const PolygonShape& a, const PolygonShape& b)
{
    float separation = std::numeric_limits<float>::lowest();

    for (int i = 0; i < a.worldVertices.size(); i++)
    {
        Vec2 va = a.worldVertices[i];
        Vec2 normal = a.EdgeAt(i).Normal();

        float minimumSeparation = std::numeric_limits<float>::max();

        for (int j = 0; j < b.worldVertices.size(); j++)
        {
            Vec2 vb = b.worldVertices[j];
            float projection = (vb - va).Dot(normal);
            minimumSeparation = std::min(minimumSeparation, projection);
        }

        separation = std::max(separation, minimumSeparation);
    }

    return separation;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact)
{
    const PolygonShape* aPolygonShape = (PolygonShape*) a->shape;
    const PolygonShape* bPolygonShape = (PolygonShape*) b->shape;

    if(FindMinimimSeparation(*aPolygonShape, *bPolygonShape) >= 0)
    {
        return false;
    }

    if(FindMinimimSeparation(*bPolygonShape, *aPolygonShape) >= 0)
    {
        return false;
    }

    return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* a, Body* b, Contact& contact)
{
    return false;
}
