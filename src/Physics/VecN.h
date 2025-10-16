#ifndef VECN_H
#define VECN_H

struct VecN {
    int N;
    float* data;
    
    VecN(int N);
    VecN(const VecN& v);
    ~VecN();

    
    //TODO implement all of these: 
    void Zero();
    // addition (v1 + v2)
    VecN operator + (const VecN& v);    
    // subtraction (v1 - v2)
    Vecn operator - (const VecN& v);
    // multiply (v1 * n)
    VecN operator * (const float n);
    // overloads (+=, -=, *=)
    const VecN& operator += (const VecN & v);
    const VecN& operator -= (const VecN & v);
    const VecN& operator *= (const float n);
    // fetch at index (eg vec[2])
    float operator [] (const int index) const;
    float& operator [] (const int index);
    // Dot product (v1.Dot(v2))
    float Dot(const VecN& v) const;
};

#endif