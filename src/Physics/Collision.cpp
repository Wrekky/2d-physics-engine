#include "Collision.h"

void Collision::ResolveCollision(Contact& contact) {
    Body* a = contact.a;
    Body* b = contact.b;

    float elasticity = std::min(a->restitution, b->restitution);

    Vec2 relativeVelocity = a->velocity - b->velocity;

    float  impulseMagnitude = -(1 + elasticity) * Vec2::Dot(relativeVelocity, contact.normal) / (a->invMass + b->invMass);
    Vec2 impulseDirection = contact.normal;

    Vec2 impulse = impulseDirection * impulseMagnitude;

    a->ApplyImpulse(impulse);
    b->ApplyImpulse(-impulse);
}