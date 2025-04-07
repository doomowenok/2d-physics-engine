#pragma once

#include "Body.h"

class Constraint
{
public:
    Body* a;
    Body* b;

    MatMN GetInverseM();
    VecN vec;

    void Solve();
};
