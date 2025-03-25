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

BoxShape::BoxShape(float width, float height)
{

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
