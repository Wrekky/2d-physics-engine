#include "Particle.h"

// TODO:
Particle::Particle(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;
}

Particle::~Particle() {

}

void Particle::Integrate(float dt) {
    acceleration = sumForces / mass;
    velocity += acceleration * dt;
    position += velocity * dt;
    ClearForces();
}

void Particle::AddForce(const Vec2& force) {
    sumForces += force;
}

void Particle::ClearForces() {
    sumForces = Vec2(0.0, 0.0);
}