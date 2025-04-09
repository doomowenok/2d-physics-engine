#include "Constraint.h"

MatMN Constraint::GetInverseM() const
{
    MatMN inverseM(6, 6);

    inverseM.Zero();

    inverseM.rows[0][0] = a->inverseMass;
    inverseM.rows[1][1] = a->inverseMass;
    inverseM.rows[2][2] = a->inverseI;

    inverseM.rows[3][3] = b->inverseMass;
    inverseM.rows[4][4] = b->inverseMass;
    inverseM.rows[5][5] = b->inverseI;

    return inverseM;
}

VecN Constraint::GetVelocities() const
{
    VecN v(6);

    v.Zero();

    v[0] = a->velocity.x;
    v[1] = a->velocity.y;
    v[2] = a->angularVelocity;

    v[3] = b->velocity.x;
    v[4] = b->velocity.y;
    v[5] = b->angularVelocity;

    return v;
}

void Constraint::Solve()
{

}
