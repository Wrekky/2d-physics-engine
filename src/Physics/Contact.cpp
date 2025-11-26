#include "Contact.h"
//Moves both objects depending on depth and mass to the correct position to resolve the collision.
void Contact::ResolvePenetration() {
    if (a->IsStatic() && b->IsStatic()) {
        return;
    }
    
    float depthAdjustA = (depth / (a->invMass + b->invMass)) * a->invMass;
    float depthAdjustB = (depth / (a->invMass + b->invMass)) * b->invMass;

    a->position -= normal * depthAdjustA * 0.1;
    b->position += normal * depthAdjustB * 0.1;
    
    a->shape->UpdateVertices(a->rotation, a->position);
    b->shape->UpdateVertices(b->rotation, b->position);
}

void Contact::ResolveCollision() {
    
    for (int i = 0; i < 10; i++) {
        ResolvePenetration();
    }

    float elasticity = std::max(a->restitution, b->restitution);

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

    float frictionCoeffecient = std::min(a->friction, b->friction);
    Vec2 tangent = normal.Normal();
    float vrelDotTangent = relativeVelocity.Dot(tangent);
    Vec2 impulseDirectionT = tangent;
    float impulseMagnitudeT = 
    frictionCoeffecient * 
    -(1 + elasticity) * vrelDotTangent 
    / 
    ((a->invMass + b->invMass) 
    + 
    ra.Cross(tangent) * ra.Cross(tangent) * a->invI 
    + 
    rb.Cross(tangent) * rb.Cross(tangent) * b->invI);

    Vec2 jT = impulseDirectionT * impulseMagnitudeT;
    Vec2 jN = impulseDirection * impulseMagnitude;
    Vec2 j = jT + jN;

    a->ApplyImpulseAtPoint(j, ra);
    b->ApplyImpulseAtPoint(-j, rb);
}