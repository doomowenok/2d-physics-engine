#pragma once

#include "VecN.h"

struct MatMN
{
    int M;
    int N;
    VecN* rows;

    MatMN();
    MatMN(int m, int n);
    MatMN(const MatMN& m);
    ~MatMN();

    void Zero();
    MatMN Transpose() const;

    static VecN SolveGaussSeidel(const MatMN& A, const VecN& b);

    const MatMN& operator = (const MatMN& m);
    VecN operator * (const VecN& v) const;
    MatMN operator * (const MatMN& m) const;
};
