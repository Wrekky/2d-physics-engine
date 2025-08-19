#include "Body.h"

// TODO:
Body::Body(const Shape& shape, float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;
    this->shape = shape.Clone();
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    }
    else {
        this-> invMass = 0.0;
    }
}

Body::~Body() {
    delete shape;
}

void Body::Integrate(float dt) {
    acceleration = sumForces * invMass;
    velocity += acceleration * dt;
    position += velocity * dt;
    ClearForces();
}

void Body::AddForce(const Vec2& force) {
    sumForces += force;
}

void Body::ClearForces() {
    sumForces = Vec2(0.0, 0.0);
}