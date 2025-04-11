#pragma once

#include "SDL.h"
#include "Vec2.h"
#include "Shape.h"


struct Body
{
    bool isColliding = false;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float rotation;
    float angularVelocity;
    float angularAcceleration;

    Vec2 sumForces;
    float sumTorque;

    float mass;
    float inverseMass;
    float I;
    float inverseI;

    float restitution;
    float friction;

    Shape* shape = nullptr;

    SDL_Texture* texture = nullptr;

    Body(const Shape& shape, float x, float y, float mass, float restitution, float friction);
    ~Body();

    bool IsStatic() const;

    void AddForce(const Vec2& force);
    void AddTorque(float torque);

    void ClearForces();
    void ClearTorque();

    Vec2 WorldToLocalSpace(const Vec2& point) const;
    Vec2 LocalToWorldSpace(const Vec2& point) const;

    void ApplyImpulse(const Vec2& impulse);
    void ApplyImpulse(const Vec2& impulse, const Vec2& r);

    void IntegrateLinear(float deltaTime);
    void IntegrateAngular(float deltaTime);

    void Update(float deltaTime);

    void SetTexture(const char* textureFileName);
};