#include "CollisionDetection.h"
#include <limits>
bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact) {
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;

    bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;
    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contact);
    }
    
    if (aIsPolygon && bIsPolygon) {
        return IsCollidingPolygonPolygon(a, b, contact);
    }

    return false;
};

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact) {
    CircleShape* aCircleShape = (CircleShape*) a->shape;
    CircleShape* bCircleShape = (CircleShape*) b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);

    if (!isColliding) {
        return false;
    }

    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalize();
    contact.end = a->position + contact.normal * aCircleShape->radius;
    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.depth = (contact.end - contact.start).Magnitude();

    return isColliding;
};

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact) {
    const PolygonShape* aPolygonShape = (PolygonShape*) a->shape;
    const PolygonShape* bPolygonShape = (PolygonShape*) b->shape;
    
    Vec2 aAxis, bAxis;
    Vec2 aPoint, bPoint;
    float abSeparation = aPolygonShape->FindMinSeparation(bPolygonShape, aAxis, aPoint);
    float baSeparation = bPolygonShape->FindMinSeparation(aPolygonShape, bAxis, bPoint);
    if (abSeparation >= 0) {
        return false;
    }

    if (baSeparation >= 0) {
        return false;
    }

    if (abSeparation > baSeparation)
    {
        contact.normal = -aAxis.Normal();
        contact.end = aPoint;
        contact.start = contact.end + (contact.normal * abSeparation);
        contact.depth = abSeparation;
        contact.a = a;
        contact.b = b;
    }

    if (baSeparation > abSeparation)
    {
        contact.normal = -bAxis.Normal();
        contact.end = bPoint;
        contact.start = contact.end + (contact.normal * baSeparation);
        contact.depth = baSeparation;
        contact.a = b;
        contact.b = a;
    }

    return true;
};