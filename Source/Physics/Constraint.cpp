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

JointConstraint::JointConstraint() : Constraint(), jacobian(1, 6)
{

}

JointConstraint::JointConstraint(Body* a, Body* b, const Vec2& anchorPoint) : Constraint(), jacobian(1, 6)
{
    this->a = a;
    this->b = b;

    this->aPoint = a->WorldToLocalSpace(anchorPoint);
    this->bPoint = b->WorldToLocalSpace(anchorPoint);
}

JointConstraint::~JointConstraint()
{

}

void JointConstraint::Solve()
{
    const Vec2 pa = a->LocalToWorldSpace(aPoint);
    const Vec2 pb = b->LocalToWorldSpace(bPoint);

    const Vec2 ra = pa - a->position;
    const Vec2 rb = pb - b->position;

    const Vec2 j1 = (pa - pb) * 2.0f;
    jacobian.rows[0][0] = j1.x; // A - Linear velocity.x
    jacobian.rows[0][1] = j1.y; // A - Linear velocity.y

    const float j2 = ra.Cross(pa - pb) * 2.0f;
    jacobian.rows[0][2] = j2;   // A - Angular velocity

    const Vec2 j3 = (pb - pa) * 2.0f;
    jacobian.rows[0][3] = j3.x; // B - Linear velocity.x
    jacobian.rows[0][4] = j3.y; // B - Linear velocity.y

    const float j4 = rb.Cross(pb - pa) * 2.0f;
    jacobian.rows[0][5] = j4;   // B - Angular velocity

    const VecN v = GetVelocities();
    const MatMN inverseM = GetInverseM();

    const MatMN jacobianT = jacobian.Transpose();

    VecN rhs = jacobian * v * -1.0f;                // A
    MatMN lhs = jacobian * inverseM * jacobianT;    // b

    // Ax = b (Gauss Seidel Method)
    VecN lambda = MatMN::SolveGaussSeidel(lhs, rhs);
}
