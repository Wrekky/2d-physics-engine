#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"
// TODO:
struct Body
{
    Vec2 sumForces;
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float mass;
    float invMass;
    
    Shape* shape = NULL;

    void AddForce(const Vec2& force);
    void ClearForces();

    void Integrate(float dt);

    Body(const Shape& shape, float x, float y, float mass);
    ~Body();
};
#endif