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