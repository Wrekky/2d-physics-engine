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