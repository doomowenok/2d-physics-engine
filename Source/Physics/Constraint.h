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

    virtual void PreSolve() { }
    virtual void Solve() { }
    virtual void PostSolve() { }
};

class JointConstraint : public Constraint
{
private:
    MatMN jacobian;
    VecN cachedLambda;

public:
    JointConstraint();
    JointConstraint(Body* a, Body* b, const Vec2& anchorPoint);
    void PreSolve() override;
    void Solve() override;
    void PostSolve() override;
};

class PenetrationConstraint : public Constraint
{
private:
    MatMN jacobian;

public:
    void Solve() override;
};