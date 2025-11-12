#include "CollisionDetection.h"
#include <limits>
#include <cmath>
#include "../Graphics.h"
#include <iostream>
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
    
    if(aIsCircle && bIsPolygon) {
        return IsCollidingCirclePolygon(a, b, contact);
    }

    if(bIsCircle && aIsPolygon) {
        return IsCollidingCirclePolygon(b, a, contact);
    }

    return false;
}

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
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact) {
    const PolygonShape* aPolygonShape = (PolygonShape*) a->shape;
    const PolygonShape* bPolygonShape = (PolygonShape*) b->shape;
    
    Vec2 aAxis, bAxis;
    Vec2 aPoint, bPoint;
    float abSeparation = aPolygonShape->FindMinSeparation(bPolygonShape, aAxis, aPoint);
    if (abSeparation >= 0) {
        return false;
    }
    float baSeparation = bPolygonShape->FindMinSeparation(aPolygonShape, bAxis, bPoint);
    if (baSeparation >= 0) {
        return false;
    }
    contact.a = a;
    contact.b = b;
    if (abSeparation > baSeparation)
    {
        contact.depth = -abSeparation;
        contact.normal = aAxis.Normal();
        contact.start = aPoint;
        contact.end = aPoint + contact.normal * contact.depth;
    }
    else
    {
        contact.depth = -baSeparation;
        contact.normal = -bAxis.Normal();
        contact.start = bPoint - contact.normal * contact.depth;
        contact.end = bPoint;
    }

    return true;
}

bool CollisionDetection::IsCollidingCirclePolygon(Body* aCircle, Body* bPolygon, Contact& contact) {
    PolygonShape* polygonShape = (PolygonShape*) bPolygon->shape;
    std::vector<Vec2>& polygonVertices = polygonShape->worldVertices;
    CircleShape* circleShape = (CircleShape*) aCircle->shape;
    Vec2 minCurrVertex;
    Vec2 minNextVertex;
    bool inside = true;
    float distanceCircleEdge = std::numeric_limits<float>::lowest();
    for (int i = 0; i < polygonVertices.size();  i++) {
        int currVertex = i;
        int nextVertex = (i + 1) % polygonVertices.size();
        Vec2 edge = polygonShape->EdgeAt(currVertex);
        Vec2 normal = edge.Normal();

        Vec2 circleCenter = aCircle->position - polygonVertices[currVertex];

        float projection = circleCenter.Dot(normal);

        if (projection > 0) {
            minCurrVertex = polygonShape->worldVertices[currVertex];
            minNextVertex = polygonShape->worldVertices[nextVertex];
            inside = false;
            distanceCircleEdge = projection;
            break;
        }
        else {
            if (projection > distanceCircleEdge) {
                distanceCircleEdge = projection;
                minCurrVertex = polygonShape->worldVertices[currVertex];
                minNextVertex = polygonShape->worldVertices[nextVertex];
            }
        }
    }
    
    if (!inside) {
        Vec2 AB = (minCurrVertex - minNextVertex);
        Vec2 AC = (minCurrVertex - aCircle->position);
        Vec2 BC = (minNextVertex - aCircle->position);
        Vec2 BA = (minNextVertex - minCurrVertex);
        if (AC.Dot(AB) < 0) {
            //region a
            if (AC.Magnitude() > circleShape->radius) {
                return false;
            }
            else {
                contact.a = bPolygon;
                contact.b = aCircle;
                contact.depth = circleShape->radius - AC.Magnitude();
                contact.normal = -AC.Normalize();
                contact.start = aCircle->position + (contact.normal * -circleShape->radius);
                contact.end = contact.start + (contact.normal * contact.depth);
            }
        }
        else if (BC.Dot(BA) < 0) {
            //region b
            if (BC.Magnitude() > circleShape->radius) {
                return false;
            }
            else {
                contact.a = bPolygon;
                contact.b = aCircle;
                contact.depth = circleShape->radius - BC.Magnitude();
                contact.normal = -BC.Normalize();
                contact.start = aCircle->position + (contact.normal * -circleShape->radius);
                contact.end = contact.start + (contact.normal * contact.depth);
            }
        }
        else {
            //region c
            if (distanceCircleEdge > circleShape->radius) {
                return false;
            }
            else {
                contact.a = bPolygon;
                contact.b = aCircle;
                contact.depth = circleShape->radius - distanceCircleEdge;
                contact.normal = (minNextVertex - minCurrVertex).Normal();
                contact.start = aCircle->position - (contact.normal * circleShape->radius);
                contact.end = contact.start + (contact.normal * contact.depth);
            }
        }
    }
    else {
        //same as if in region c
        contact.a = bPolygon;
        contact.b = aCircle;
        contact.depth = circleShape->radius - distanceCircleEdge;
        contact.normal = (minNextVertex - minCurrVertex).Normal();
        contact.start = aCircle->position - (contact.normal * circleShape->radius);
        contact.end = contact.start + (contact.normal * contact.depth);
    }

    return true;
}

bool CollisionDetection::IsInside(Vec2 point, Body* box) {
    if (box->shape->GetType() != BOX) {
        return false;
    }
    BoxShape* boxShape = (BoxShape*) box->shape;
    Vec2 posAdjusted;
    posAdjusted = box->position;
    posAdjusted.x = posAdjusted.x - boxShape->width / 2;
    posAdjusted.y = posAdjusted.y - boxShape->height / 2;
    if (point.x > posAdjusted.x && point.x < posAdjusted.x + boxShape->width
    && point.y > posAdjusted.y && point.y < posAdjusted.y + boxShape->height) {
        return true;
    }
    else {
        return false;
    }
}