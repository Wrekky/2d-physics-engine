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

    Vec2 ra = end - a->position;
    Vec2 rb = start - b->position;

    Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);

    Vec2 relativeVelocity = va - vb;

    float vrelDotNormal = relativeVelocity.Dot(normal);

    Vec2 impulseDirection = normal;

    float  impulseMagnitude = 
    -(1 + elasticity) * vrelDotNormal 
    / 
    ((a->invMass + b->invMass) 
    + 
    ra.Cross(normal) * ra.Cross(normal) * a->invI 
    + 
    rb.Cross(normal) * rb.Cross(normal) * b->invI);

    Vec2 impulse = impulseDirection * impulseMagnitude;

    a->ApplyImpulse(impulse, ra);
    b->ApplyImpulse(-impulse, rb);
}