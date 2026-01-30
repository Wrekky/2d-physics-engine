#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Body.h"
#include "Contact.h"
#include "../UI/UIObject.h"
struct CollisionDetection {
    static bool IsColliding(Body* a, Body* b, Contact& contact);
    static bool IsCollidingCircleCircle(Body* a, Body* b, Contact& contact);
    static bool IsCollidingCirclePolygon(Body* a, Body* b, Contact& contact);
    static bool IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact);

    static bool IsInside(Vec2 point, Body* box);
    static bool IsInside(Vec2 point, UIObject* textButton);
};

#endif