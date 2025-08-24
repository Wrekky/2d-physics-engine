#ifndef COLLISION_H
#define COLLISION_H

#include "Vec2.h"
#include "Contact.h"
struct Collision {
    static void ResolveCollision(Contact& contact);
};
#endif