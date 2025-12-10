#include "Body.h"
#include <math.h>
#include "../Graphics.h"

Body::Body(const Shape& shape, float x, float y, float mass) {
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->mass = mass;
    this->I = shape.GetMomentOfInertia() * mass;
    this->friction = 0.7;
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angularAcceleration = 0.0;
    this->sumForces = Vec2(0, 0);
    this->sumTorque = 0.0;
    this->restitution = 1.0;
    this->color = 0xFF22bb11;
    if (I != 0.0) {
        this->invI = 1.0 / I;
    }
    else {
        this->invI = 0.0;
    }

    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    }
    else {
        this-> invMass = 0.0;
    }
    this->shape->UpdateVertices(rotation, position);
}

Body::~Body() {
    delete shape;
}

void Body::SetTexture(const char* textureFileName) {
    SDL_Surface* surface = IMG_Load(textureFileName);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
    }

}

bool Body::IsStatic() const {
    const float epsilon = 0.005f;
    return fabs(invMass - 0.0) < epsilon;
}

void Body::IntegrateForces(const float dt) {
    if (IsStatic()) {
        return;
    }

    acceleration = sumForces * invMass;
    velocity += acceleration * dt;

    angularAcceleration = sumTorque * invI;
    angularVelocity += angularAcceleration * dt;

    ClearForces();
    ClearTorque();
}

void Body::IntegrateVelocities(const float dt) {
    if (IsStatic()) {
        return;
    }

    position += velocity * dt;
    rotation += angularVelocity * dt;

    shape->UpdateVertices(rotation, position);
}

void Body::AddForce(const Vec2& force) {
    sumForces += force;
}

void Body::ClearForces() {
    sumForces = Vec2(0.0, 0.0);
}

void Body::AddTorque(float torque) {
    sumTorque += torque;
}

void Body::ClearTorque() {
    sumTorque = 0;
}

void Body::ApplyImpulseLinear(const Vec2& j) {
    if (IsStatic()) {
        return;
    }

    velocity += j * invMass;
}

void Body::ApplyImpulseAngular(const float j) {
    if (IsStatic()) {
        return;
    }

    angularVelocity += j * invI;
}

//overload that applies angular velocity, r is distance from collision impact
void Body::ApplyImpulseAtPoint(const Vec2& j, const Vec2& r) {
    if (IsStatic()) {
        return;
    }

    velocity += j* invMass;

    angularVelocity += r.Cross(j) * invI;
}

Vec2 Body::LocalSpaceToWorldSpace(const Vec2& point) const {
    Vec2 rotated = point.Rotate(rotation);
    return rotated + position;
}

Vec2 Body::WorldSpaceToLocalSpace(const Vec2& point) const {
    float translateX = point.x - position.x;
    float translateY = point.y - position.y;

    Vec2 rotated = Vec2(translateX, translateY).Rotate(-rotation);

    return rotated;
}