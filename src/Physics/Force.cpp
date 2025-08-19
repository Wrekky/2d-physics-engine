#include "Force.h"
//You mark a variable a const if you're referencing it and not changing it.
//Shows intent to NOT change the variable information.
Vec2 Force::GenerateDragForce(const Particle& particle, float k) {
    Vec2 dragForce = Vec2(0,0);
    if (particle.velocity.MagnitudeSquared() > 0)
    {
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;
        float dragMagnitude = k * particle.velocity.MagnitudeSquared();
        dragForce = dragDirection * dragMagnitude;
    }
    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle& particle, float k) {
    Vec2 frictionForce = particle.velocity.UnitVector() * -1.0;
    frictionForce *= k;
    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Particle& a, const Particle& b, float g) {
    Vec2 d = (b.position - a.position);

    float distanceSquared = d.MagnitudeSquared();

    Vec2 attractionDirection = d.UnitVector();

    float attractionMagnitude = g * (a.mass * b.mass) / distanceSquared;

    Vec2 attractionForce = attractionDirection * attractionMagnitude;
}

Vec2 Force::GenerateSpringForce(const Particle& particle, Vec2 anchor, float restLength, float k) {
    Vec2 d = particle.position - anchor;

    float displacement = d.Magnitude() - restLength;

    Vec2 springDirection = d.UnitVector();
    float springMagnitude = -k * displacement;

    Vec2 springForce = springDirection * springMagnitude;

    return springForce;
}

Vec2 Force::GenerateSpringForce(const Particle& particle, const Particle& b, float restLength, float k) {
    Vec2 d = particle.position - b.position;

    float displacement = d.Magnitude() - restLength;

    Vec2 springDirection = d.UnitVector();
    float springMagnitude = -k * displacement;

    Vec2 springForce = springDirection * springMagnitude;

    return springForce;
}

//Creates a chain of particles. Adds the required force for every particle.
void Force::GenerateChainForces(Vec2 anchor, float restLength, float springStrength, std::vector<Particle*> chainParticles) {
    //apply spring force on every particle. order? each particle needs to understand which particle its attached to.
    //spring force will be applied in the order of the array.
    Vec2 springForce = GenerateSpringForce(*chainParticles[0], anchor, restLength, springStrength);
    chainParticles[0]->AddForce(springForce);
    for (int i = 1; i < chainParticles.size(); i++) {
        springForce = GenerateSpringForce(*chainParticles[i], *chainParticles[i-1], restLength, springStrength);
        chainParticles[i]->AddForce(springForce);
        //Force needs to be applied both ways.
        springForce = GenerateSpringForce(*chainParticles[i-1], *chainParticles[i], restLength, springStrength);
        chainParticles[i-1]->AddForce(springForce);
    }
}
