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

float FindMinSeparation(const PolygonShape& a, const PolygonShape& b, Contact& contact) {
    float separation = std::numeric_limits<float>::lowest();

    //Can optimize this a bit potentially.
    //  exit when first separation is found and use that.
    //  will cause issues if objects are moving too fast but should be fine otherwise.
    int endVerticeA = 0;
    int endVerticeB = 0;
    for (int i = 0; i < a.worldVertices.size(); i++) {
        Vec2 va = a.worldVertices[i];
        Vec2 normal = a.EdgeAt(i).Normal();

        float minSep = std::numeric_limits<float>::max();

        for (int j = 0; j < b.worldVertices.size(); j++) {
            Vec2 vb = b.worldVertices[j];
            if (minSep > (vb - va).Dot(normal)) {
                minSep = (vb - va).Dot(normal);
                endVerticeB = j;
            }
        }
        if (separation < minSep) {
            contact.end = b.worldVertices[endVerticeB];
            separation = minSep;
            endVerticeA = i;
            contact.normal = normal;
        }
    }
    contact.depth = -separation;
    contact.start = contact.end + (contact.normal * contact.depth);
    

    return separation;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact) {
    const PolygonShape* aPolygonShape = (PolygonShape*) a->shape;
    const PolygonShape* bPolygonShape = (PolygonShape*) b->shape;
    
    contact.a = a;
    contact.b = b;
    Contact testContact = contact;
    if (FindMinSeparation(*aPolygonShape, *bPolygonShape, testContact) >= 0) {
        return false;
    }
    else {
        contact.depth = testContact.depth;
        contact.end = testContact.end;
        contact.start = testContact.start;
        contact.normal = testContact.normal;
    }

    if (FindMinSeparation(*bPolygonShape, *aPolygonShape, contact) >= 0) {
        return false;
    }
    return true;
};