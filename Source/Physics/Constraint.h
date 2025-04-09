#pragma once

#include "Body.h"
#include "MatMN.h"
#include "VecN.h"

class Constraint
{
public:
    Body* a;
    Body* b;

    virtual ~Constraint() = default;

    MatMN GetInverseM() const;
    VecN GetVelocities() const;

    virtual void Solve() { }
};

class DistantConstraint : public Constraint
{

};

class PenetrationConstraint : public Constraint
{

};