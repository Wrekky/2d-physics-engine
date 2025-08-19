#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
// TODO:
struct Body
{
    int radius;
    
    Vec2 sumForces;
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float mass;
    float invMass;
    
    void AddForce(const Vec2& force);
    void ClearForces();
    void Integrate(float dt);
    Body(float x, float y, float mass);
    ~Body();
};
#endif