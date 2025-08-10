#include "Particle.h"

// TODO:
Particle::Particle(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;
}
Particle::~Particle() {

}