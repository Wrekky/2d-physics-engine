#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Body.h"
struct CollisionDetection {
    static bool IsColliding(Body* a, Body* b);
    static bool IsCollidingCircleCircle(Body* a, Body* b);
    static bool IsCollidingCirclePolygon(Body* a, Body* b);
    static bool IsCollidingPolygonPolygon(Body* a, Body* b);
};

#endif