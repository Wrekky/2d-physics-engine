#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"
// TODO:
struct Particle
{
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float mass;

    void Integrate(float dt);
    Particle(float x, float y, float mass);
    ~Particle();
};
#endif