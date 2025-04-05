#include "CollisionDetection.h"
#include "Contact.h"
#include <limits>

#include "../Graphics.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact &contact)
{
    const bool aIsCircle = a->shape->GetType() == CIRCLE;
    const bool bIsCircle = b->shape->GetType() == CIRCLE;

    const bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    const bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;

    if (aIsCircle && bIsCircle)
    {
        return IsCollidingCircleCircle(a, b, contact);
    }

    if (aIsPolygon && bIsPolygon)
    {
        return IsCollidingPolygonPolygon(a, b, contact);
    }

    if(aIsPolygon && bIsCircle)
    {
        return IsCollidingPolygonCircle(a, b, contact);
    }

    if(aIsCircle && bIsPolygon)
    {
        return IsCollidingPolygonCircle(b, a, contact);
    }

    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact &contact)
{
    const CircleShape* aCircleShape = (CircleShape *) a->shape;
    const CircleShape* bCircleShape = (CircleShape *) b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= radiusSum * radiusSum;

    if (!isColliding)
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

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact &contact)
{
    const PolygonShape* aPolygonShape = (PolygonShape *) a->shape;
    const PolygonShape* bPolygonShape = (PolygonShape *) b->shape;

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

    contact.a = a;
    contact.b = b;

    if (abSeparation > baSeparation)
    {
        contact.depth = -abSeparation;
        contact.normal = aAxis.Normal();
        contact.start = aPoint;
        contact.end = aPoint + contact.normal * contact.depth;
    }
    else
    {
        contact.depth = -baSeparation;
        contact.normal = -bAxis.Normal();
        contact.start = bPoint - contact.normal * contact.depth;
        contact.end = bPoint;
    }

    return true;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* polygon, Body* circle, Contact &contact)
{
    const PolygonShape* polygonShape = (PolygonShape *) polygon->shape;
    const CircleShape* circleShape = (CircleShape *) circle->shape;

    const std::vector<Vec2>& polygonVertices = polygonShape->worldVertices;

    Vec2 minCurrentVertex;
    Vec2 minNextVertex;

    for (int i = 0; i < polygonVertices.size(); i++)
    {
        int currentVertex = i;
        int nextVertex = (i + 1) % polygonVertices.size();

        Vec2 edge = polygonShape->EdgeAt(currentVertex);
        Vec2 normal = edge.Normal();

        Vec2 circleCenter = circle->position - polygonVertices[currentVertex];

        const  float projection = circleCenter.Dot(normal);

        if(projection > 0)
        {
            minCurrentVertex = polygonShape->worldVertices[currentVertex];
            minNextVertex = polygonShape->worldVertices[nextVertex];
            break;
        }
    }

    Graphics::DrawFillCircle(minCurrentVertex.x, minCurrentVertex.y, 5, 0xFF00FFFF);
    Graphics::DrawFillCircle(minNextVertex.x, minNextVertex.y, 5, 0xFF00FFFF);

    return false;
}
