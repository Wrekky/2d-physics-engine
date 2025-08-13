#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"
// TODO:
struct Particle
{
    int radius;
    
    Vec2 sumForces;
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float mass;

    void AddForce(const Vec2& force);
    void ClearForces();
    void Integrate(float dt);
    Particle(float x, float y, float mass);
    ~Particle();
};
#endif