#include "VecN.h"

VecN::VecN() : N(0), data(nullptr) { }


VecN::VecN(int n) : N(n)
{
    data = new float[n];
}

VecN::VecN(const VecN& v)
{
    N = v.N;
    data = new float[v.N];

    for(int i = 0; i < v.N; i++)
    {
        data[i] = v.data[i];
    }
}

VecN::~VecN()
{
    delete[] data;
}

void VecN::Zero()
{
    for(int i = 0; i < N; i++)
    {
        data[i] = 0.0f;
    }
}

float VecN::Dot(const VecN& v)
{
    float dot = 0.0f;

    for(int i = 0; i < N; i++)
    {
        dot += data[i] * v.data[i];
    }

    return dot;
}

VecN& VecN::operator=(const VecN& v)
{
    delete[] data;
    N = v.N;
    data = new float[N];

    for(int i = 0; i < N; i++)
    {
        data[i] = v.data[i];
    }

    return *this;
}

VecN VecN::operator+(const VecN& v) const
{
    VecN result = *this;

    for(int i = 0; i < N; i++)
    {
        result.data[i] += v.data[i];
    }

    return result;
}

VecN VecN::operator-(const VecN& v) const
{
    VecN result = *this;

    for(int i = 0; i < N; i++)
    {
        result.data[i] -= v.data[i];
    }

    return result;
}

VecN VecN::operator*(float k) const
{
    VecN result = *this;
    result *= k;
    return result;
}

const VecN& VecN::operator+=(const VecN& v)
{
    for(int i = 0; i < N; i++)
    {
        data[i] += v.data[i];
    }
    return *this;
}

const VecN& VecN::operator-=(const VecN& v)
{
    for(int i = 0; i < N; i++)
    {
        data[i] -= v.data[i];
    }
    return *this;
}

const VecN& VecN::operator*=(float k)
{
    for(int i = 0; i < N; i++)
    {
        data[i] *= k;
    }
    return *this;
}

float VecN::operator[](const int index) const
{
    return data[index];
}

float& VecN::operator[](const int index)
{
    return data[index];
}