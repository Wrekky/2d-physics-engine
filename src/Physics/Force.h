#ifndef FORCE_H
#define FORCE_H

#include "Vec2.h"
#include "Particle.h"
#include <vector>
struct Force {
    static Vec2 GenerateDragForce(const Particle& particle, float k);
    static Vec2 GenerateFrictionForce(const Particle& particle, float k);
    static Vec2 GenerateGravitationalForce(const Particle& a, const Particle& b, float g);
    static Vec2 GenerateSpringForce(const Particle& particle, Vec2 anchor, float restLength, float k);
    static Vec2 GenerateSpringForce(const Particle& particle, const Particle& b, float restLength, float k);
    
    static void GenerateChainForces(Vec2 anchor, float restLength, float springStrength, std::vector<Particle*> chainParticles);
};
#endif