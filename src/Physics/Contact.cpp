#include "Contact.h"
//Moves both objects depending on depth and mass to the correct position to resolve the collision.
void Contact::ResolvePenetration() {
    if (a->IsStatic() && b->IsStatic()) {
        return;
    }
    
    float depthAdjustA = (depth / (a->invMass + b->invMass)) * a->invMass;
    float depthAdjustB = (depth / (a->invMass + b->invMass)) * b->invMass;

    a->position -= normal * depthAdjustA;
    b->position += normal * depthAdjustB;
}

void Contact::ResolveCollision() {
    ResolvePenetration();
    float elasticity = std::min(a->restitution, b->restitution);

    Vec2 relativeVelocity = a->velocity - b->velocity;

    float  impulseMagnitude = -(1 + elasticity) * relativeVelocity.Dot(normal) / (a->invMass + b->invMass);
    Vec2 impulseDirection = normal;

    Vec2 impulse = impulseDirection * impulseMagnitude;

    a->ApplyImpulse(impulse);
    b->ApplyImpulse(-impulse);
}