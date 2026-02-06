#ifndef RAY_H
#define RAY_H

#include "../Physics/Vec2.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include "LightSource.h"
class Ray {
    public:
        Ray();
        Vec2 position;
        float direction;
        Uint32 color; //initial color of the light source
        float intensity; //how bright the light is from the source, should decrease over time.
        int bounces; //amount of bounces for each ray
        int beamSpread; //amount of rays being shot
        float distance; // maybe.
};
#endif