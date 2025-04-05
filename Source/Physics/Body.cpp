#include "Body.h"
#include <cmath>

Body::Body(const Shape& shape, float x, float y, float mass, float restitution, float friction)
{
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->velocity = Vec2(0.0f, 0.0f);
    this->acceleration = Vec2(0.0f, 0.0f);
    this->rotation = 0.0f;
    this->angularVelocity = 0.0f;
    this->angularAcceleration = 0.0f;
    this->sumForces = Vec2(0.0f, 0.0f);
    this->sumTorque = 0.0f;
    this->mass = mass;
    this->inverseMass = mass == 0.0f ? 0.0f : 1.0f / mass;
    this->I = shape.GetMomentOfInertia() * mass;
    this->inverseI = I == 0.0f ? 0.0f : 1.0f / I;
    this->restitution = restitution;
    this->friction = friction;
}

Body::~Body()
{
    delete shape;
}

bool Body::IsStatic() const
{
    const float MASS_EPSILON = 0.005f;
    return fabsf(inverseMass - 0.0f) < MASS_EPSILON;
}

void Body::AddForce(const Vec2& force)
{
    sumForces += force;
}

void Body::AddTorque(float torque)
{
    sumTorque += torque;
}

void Body::ClearForces()
{
    sumForces = Vec2(0.0f, 0.0f);
}

void Body::ClearTorque()
{
    sumTorque = 0.0f;
}

// Only linear
void Body::ApplyImpulse(const Vec2& impulse)
{
    if(IsStatic())
    {
        return;
    }

    velocity += impulse * inverseMass;
}

// Linear + Angular
void Body::ApplyImpulse(const Vec2& impulse, const Vec2& r)
{
    if(IsStatic())
    {
        return;
    }

    velocity += impulse * inverseMass;
    angularVelocity += r.Cross(impulse) * inverseI;
}

void Body::IntegrateLinear(float deltaTime)
{
    if(IsStatic())
    {
        return;
    }

    // acceleration = sumForces / mass;
    acceleration = sumForces * inverseMass;

    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    ClearForces();
}

void Body::IntegrateAngular(float deltaTime)
{
    // angularAcceleration = sumTorque / I;
    angularAcceleration = sumTorque * inverseI;

    angularVelocity += angularAcceleration * deltaTime;
    rotation += angularVelocity * deltaTime;

    ClearTorque();
}

void Body::Update(float deltaTime)
{
    IntegrateLinear(deltaTime);
    IntegrateAngular(deltaTime);

    ShapeType shapeType = shape->GetType();
    bool isPolygon = shapeType == POLYGON || shapeType == BOX;

    if(isPolygon)
    {
        PolygonShape* polygonShape = (PolygonShape*) shape;
        polygonShape->UpdateVertices(rotation, position);
    }
}
