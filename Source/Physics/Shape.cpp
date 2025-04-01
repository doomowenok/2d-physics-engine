#include "Shape.h"

CircleShape::CircleShape(const float radius)
{
    this->radius = radius;
}

CircleShape::~CircleShape()
{

}

ShapeType CircleShape::GetType() const
{
    return ShapeType::CIRCLE;
}

Shape* CircleShape::Clone() const
{
    return new CircleShape(this->radius);
}

float CircleShape::GetMomentOfInertia() const
{
    return radius * radius * 0.5f;
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices)
{

}

PolygonShape::~PolygonShape()
{

}

ShapeType PolygonShape::GetType() const
{
    return ShapeType::POLYGON;
}

Shape* PolygonShape::Clone() const
{
    return new PolygonShape(this->localVertices);
}

float PolygonShape::GetMomentOfInertia() const
{
    return 0.0f;
}

Vec2 PolygonShape::EdgeAt(int index) const
{
    int currentVertex = index;
    int nextVertex = (index + 1) % worldVertices.size();
    return worldVertices[nextVertex] - worldVertices[currentVertex];
}

float PolygonShape::FindMinimimSeparation(const PolygonShape* other, Vec2& axis, Vec2& point) const
{
    float separation = std::numeric_limits<float>::lowest();

    for (int i = 0; i < worldVertices.size(); i++)
    {
        Vec2 va = worldVertices[i];
        Vec2 normal = EdgeAt(i).Normal();

        Vec2 minimumVertex;
        float minimumSeparation = std::numeric_limits<float>::max();

        for (int j = 0; j < other->worldVertices.size(); j++)
        {
            Vec2 vb = other->worldVertices[j];
            float projection = (vb - va).Dot(normal);

            if (projection < minimumSeparation)
            {
                minimumSeparation = projection;
                minimumVertex = vb;
            }

            minimumSeparation = std::min(minimumSeparation, projection);
        }

        if (minimumSeparation > separation)
        {
            separation = minimumSeparation;
            axis = EdgeAt(i);
            point = minimumVertex;
        }

        separation = std::max(separation, minimumSeparation);
    }

    return separation;
}

void PolygonShape::UpdateVertices(float angle, const Vec2& position)
{
    for(int i = 0; i < localVertices.size(); i++)
    {
        worldVertices[i] = localVertices[i].Rotate(angle);
        worldVertices[i] += position;
    }
}

BoxShape::BoxShape(float width, float height)
{
    this->width = width;
    this->height = height;

    localVertices.push_back(Vec2(-(width / 2.0f), -(height / 2.0f)));
    localVertices.push_back(Vec2(+(width / 2.0f), -(height / 2.0f)));
    localVertices.push_back(Vec2(+(width / 2.0f), +(height / 2.0f)));
    localVertices.push_back(Vec2(-(width / 2.0f), +(height / 2.0f)));

    worldVertices.push_back(Vec2(-(width / 2.0f), -(height / 2.0f)));
    worldVertices.push_back(Vec2(+(width / 2.0f), -(height / 2.0f)));
    worldVertices.push_back(Vec2(+(width / 2.0f), +(height / 2.0f)));
    worldVertices.push_back(Vec2(-(width / 2.0f), +(height / 2.0f)));
}

BoxShape::~BoxShape()
{

}

ShapeType BoxShape::GetType() const
{
    return ShapeType::BOX;
}

Shape* BoxShape::Clone() const
{
    return new BoxShape(this->width, this->height);
}

float BoxShape::GetMomentOfInertia() const
{
    return (width * width + height * height) * 0.83333;
}
