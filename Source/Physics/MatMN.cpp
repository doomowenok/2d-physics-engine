#include "MatMN.h"
MatMN::MatMN() : M(0), N(0) { }

MatMN::MatMN(int m, int n) : M(m), N(n)
{
    rows = new VecN[M];

    for(int i = 0; i < M; i++)
    {
        rows[i] = VecN(N);
    }
}

MatMN::MatMN(const MatMN& m)
{
    *this = m;
}

MatMN::~MatMN()
{
    delete[] rows;
}

void MatMN::Zero()
{
    for(int i = 0; i < M; i++)
    {
        rows[i].Zero();
    }
}

MatMN MatMN::Transpose() const
{
    MatMN m = MatMN(N, M);

    for(int i = 0; i < M; i++)
    {
        for(int j = 0; j < N; j++)
        {
            m.rows[j][i] = rows[i][j];
        }
    }

    return m;
}

VecN MatMN::SolveGaussSeidel(const MatMN& A, const VecN& b)
{
    const int N = b.N;
    VecN X(N);
    X.Zero();

    for(int iterations = 0; iterations < N; iterations++)
    {
        for(int i = 0; i < N; i++)
        {
            if(A.rows[i][i] != 0.0f)
            {
                X[i] += (b[i] / A.rows[i][i] - (A.rows[i].Dot(X) / A.rows[i][i]));
            }
        }
    }

    return X;
}

const MatMN& MatMN::operator=(const MatMN& m)
{
    M = m.M;
    N = m.N;
    rows = new VecN[M];

    for(int i = 0; i < M; i++)
    {
        rows[i] = m.rows[i];
    }

    return *this;
}

VecN MatMN::operator*(const VecN& v) const
{
    if(v.N != N)
    {
        return v;
    }

    VecN result(M);
    VecN t = v;

    for(int i = 0; i < M; i++)
    {
        result[i] = t.Dot(rows[i]);
    }

    return result;
}

MatMN MatMN::operator*(const MatMN& m) const
{
    if(m.M != N && m.N != M)
    {
        return m;
    }

    MatMN result = MatMN(M, m.N);
    MatMN transposed = m.Transpose();

    for(int i = 0; i < M; i++)
    {
        for(int j = 0; j < N; j++)
        {
            result.rows[i][j] = rows[i].Dot(transposed.rows[j]);
        }
    }

    return result;
}