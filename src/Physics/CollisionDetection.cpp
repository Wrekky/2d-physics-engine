#include "CollisionDetection.h"
#include <limits>
#include <cmath>
#include "../Graphics.h"
#include <iostream>
bool CollisionDetection::IsColliding(Body* a, Body* b, std::vector<Contact>& contacts) {
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;

    bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;
    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contacts);
    }
    
    if (aIsPolygon && bIsPolygon) {
        return IsCollidingPolygonPolygon(a, b, contacts);
    }
    
    if(aIsCircle && bIsPolygon) {
        return IsCollidingCirclePolygon(a, b, contacts);
    }

    if(bIsCircle && aIsPolygon) {
        return IsCollidingCirclePolygon(b, a, contacts);
    }

    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, std::vector<Contact>& contacts) {
    CircleShape* aCircleShape = (CircleShape*) a->shape;
    CircleShape* bCircleShape = (CircleShape*) b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);

    if (!isColliding) {
        return false;
    }
    Contact contact;
    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalize();
    contact.end = a->position + contact.normal * aCircleShape->radius;
    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.depth = (contact.end - contact.start).Magnitude();

    contacts.push_back(contact);
    return isColliding;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, std::vector<Contact>& contacts) {
    PolygonShape* aPolygonShape = (PolygonShape*) a->shape;
    PolygonShape* bPolygonShape = (PolygonShape*) b->shape;
    
    int aIndexReferenceEdge, bIndexReferenceEdge;
    Vec2 aSupportPoint, bSupportPoint;
    //TODO: Return index of the reference edge, not the edge itself.
    float abSeparation = aPolygonShape->FindMinSeparation(bPolygonShape, aIndexReferenceEdge, aSupportPoint);
    if (abSeparation >= 0) {
        return false;
    }
    //TODO: Return the *index* of the reference edge and not the edge  itself.
    float baSeparation = bPolygonShape->FindMinSeparation(aPolygonShape, bIndexReferenceEdge, bSupportPoint);
    if (baSeparation >= 0) {
        return false;
    }

    PolygonShape* referenceShape;
    PolygonShape* incidentShape;
    int indexReferenceEdge;
    if (abSeparation > baSeparation) {
        referenceShape = aPolygonShape;
        incidentShape = bPolygonShape;
        incidentShape = aIndexReferenceEdge;
    }
    else {
        referenceShape = bPolygonShape;
        incidentShape = aPolygonShape;
        incidentShape = bIndexReferenceEdge;
    }

    //Find reference edge based on index that returned.
    Vec2 ReferenceEdge = referenceShape->EdgeAt(indexReferenceEdge);

    //Find incident edge.
    int incidentIndex = incidentShape->FindIncidentEdge(ReferenceEdge.Normal()); 
    int incidentNextIndex = (incidentIndex + 1) % incidentShape->worldVertices.size();
    
    Vec2 v0 = incidentShape->worldVertices[incidentIndex];
    Vec2 v1 = incidentShape->worldVertices[incidentNextIndex];

    std::vector<Vec2> contactPoints = {v0, v1};
    std::vector<Vec2> clippedPoints = contactPoints;

    for (int i = 0; i < referenceShape->worldVertices.size(); i++) {
        if (i == indexReferenceEdge) {
            continue;
        }
        Vec2 c0 = referenceShape->worldVertices[i];
        Vec2 c1 = referenceShape->worldVertices[(i + 1) % referenceShape->worldVertices.size()];

        int numClipped = referenceShape.ClipSegmentToLine(contactPoints, clippedPoints, c0, c1);
        if(numClipped > 2) {
            break;
        }
        contactPoints = clippedPoints;
    }

    auto vref = referenceShape->worldVertices[indexReferenceEdge.Normal()];
    if (auto& vclip: clippedPoints) {
        float separation = (vclip - vref).Dot(referenceEdge.Normal());
        if (separation <= 0) {
            Contact contact;
            contact.a = a;
            contact.b = b;
            contact.normal = ReferenceEdge.Normal();
            contact.start = vclip;
            contact.end = vclip + contact.normal * -separation;
            if (baSeparation >= abSeparation) {
                std::swap(contact.start, contact.end);
                contact.normal *= -1.0;
            }

            contacts.push_back(contact);
        }
    }
    return true;
}

bool CollisionDetection::IsCollidingCirclePolygon(Body* aCircle, Body* bPolygon, std::vector<Contact>& contacts) {
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
    Contact contact;
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
    contacts.push_back(contact);
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

//IsInside Method for a text object.
bool CollisionDetection::IsInside(Vec2 point, UIObject* textButton) {
    TextButton* txtButton = (TextButton*) textButton;
    Vec2 posAdjusted;
    posAdjusted = txtButton->position;
    posAdjusted.x = posAdjusted.x - txtButton->size.x / 2;
    posAdjusted.y = posAdjusted.y - txtButton->size.y / 2;
    if (point.x > posAdjusted.x && point.x < posAdjusted.x + txtButton->size.x
    && point.y > posAdjusted.y && point.y < posAdjusted.y + txtButton->size.y) {
        return true;
    }
    else {
        return false;
    }
}