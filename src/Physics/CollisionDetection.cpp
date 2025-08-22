#include "CollisionDetection.h"
bool CollisionDetection::IsColliding(Body* a, Body* b) {
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;

    if (aIsCircle == bIsCircle) {
        return IsCollidingCircleCircle(a, b);
    }
    else {
        return false;
    }
};

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b) {
    CircleShape* aCircleShape = (CircleShape*) a->shape;
    CircleShape* bCircleShape = (CircleShape*) b->shape;

    const Vec2 ab = a->position - b->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);
    return isColliding;
};