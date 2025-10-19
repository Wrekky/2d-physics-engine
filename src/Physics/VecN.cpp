#include "VecN.h"

VecN::VecN(): N(0), data(nullptr) {}

VecN::VecN(int N): N(N) {
    data = new float[N];
}

VecN::VecN(const VecN& v) {
    N = v.N;
    data = new float[N];
    for (int i = 0; i < N; i++)
        data[i] = v.data[i];
}

VecN::~VecN() {
    delete[] data;
}

void VecN::Zero() {
    for (int i = 0; i < N; i++)
        data[i] = 0.0f;
}

float VecN::Dot(const VecN& v) const {
    float dotProduct = 0.0;
    for (int x = 0; x < N; x++)
        dotProduct += data[x] * v.data[x];
    return dotProduct;
}

VecN& VecN::operator = (const VecN& v) {
    delete[] data;
    N = v.N;
    data = new float[N];
    for (int i = 0; i < N; i++)
        data[i] = v.data[i];
    return *this;
}

VecN VecN::operator + (const VecN& v) const { 
    VecN result = *this;
    for (int i = 0; i < N; i++)
        result.data[i] += v.data[i];
    return result;
}

VecN VecN::operator - (const VecN &v) const {
    VecN result = *this;
    for (int i = 0; i < N; i++)
        result.data[i] -= v.data[i];
    return result;
}

VecN VecN::operator * (const float n) const {
    VecN result = *this;
    for (int i = 0; i < N; i++)
        result.data[i] *= n;
    return result;
}

const VecN& VecN::operator += (const VecN &v) {
    for (int i = 0; i < N; i++)
        data[i] += v.data[i];
    return *this;
}

const VecN& VecN::operator -= (const VecN &v) {
    for (int i = 0; i < N; i++)
        data[i] -= v.data[i];
    return *this;
}

const VecN& VecN::operator *= (const float n) {
    for (int i = 0; i < N; i++)
        data[i] *= n;
    return *this;
}

float VecN::operator[] (const int index) const {
    return data[index];
}

float& VecN::operator [] (const int index) {
    return data[index];
}