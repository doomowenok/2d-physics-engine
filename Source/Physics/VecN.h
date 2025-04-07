#pragma once

struct VecN
{
    int N;
    float* data;

    VecN();
    VecN(int n);
    VecN(const VecN& v);
    ~VecN();

    void Zero();
    float Dot(const VecN& v);

    VecN operator + (const VecN& v);
    VecN operator - (const VecN& v);
    VecN operator * (float k);
    const VecN& operator += (const VecN& v);
    const VecN& operator -= (const VecN& v);
    const VecN& operator *= (float k);
    float operator [] (int index) const;
    float& operator [] (int index);
};
