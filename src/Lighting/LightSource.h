#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "../Physics/Vec2.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Ray.h"
#include <vector>
class LightSource {
    private:
        Vec2 position;
        float direction;
        Uint32 color; //initial color of the light source
        float intensity; //how bright the light is from the source, should decrease over time.
        int bounces; //amount of bounces for each ray
        int beamSpread; //amount of rays being shot
        std::vector<Ray*> currentRays;
    public:
        void ShootRays();
        void ProcessRays();
        LightSource();
        ~LightSource();
};
#endif