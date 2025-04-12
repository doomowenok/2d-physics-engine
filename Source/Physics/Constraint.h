#pragma once

#include "Body.h"
#include "MatMN.h"
#include "VecN.h"

class Constraint
{
public:
    Body* a;
    Body* b;

    Vec2 aPoint;    // Anchor in local space - a
    Vec2 bPoint;    // Anchor in local space - b

    virtual ~Constraint() = default;

    MatMN GetInverseM() const;
    VecN GetVelocities() const;

    virtual void PreSolve(float deltaTime) { }
    virtual void Solve(float deltaTime) { }
    virtual void PostSolve(float deltaTime) { }
};

class JointConstraint : public Constraint
{
private:
    MatMN jacobian;
    VecN cachedLambda;
    float bias;

public:
    JointConstraint();
    JointConstraint(Body* a, Body* b, const Vec2& anchorPoint);
    void PreSolve(float deltaTime) override;
    void Solve(float deltaTime) override;
    void PostSolve(float deltaTime) override;
};

class PenetrationConstraint : public Constraint
{
private:
    MatMN jacobian;

public:
    // void Solve(float deltaTime) override;
};