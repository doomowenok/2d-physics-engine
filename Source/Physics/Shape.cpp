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
    return new PolygonShape(this->vertices);
}

float PolygonShape::GetMomentOfInertia() const
{
    return 0.0f;
}

BoxShape::BoxShape(float width, float height)
{
    this->width = width;
    this->height = height;

    vertices.push_back(Vec2(-(width / 2.0f), -(height / 2.0f)));
    vertices.push_back(Vec2(+(width / 2.0f), -(height / 2.0f)));
    vertices.push_back(Vec2(+(width / 2.0f), +(height / 2.0f)));
    vertices.push_back(Vec2(-(width / 2.0f), +(height / 2.0f)));
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
