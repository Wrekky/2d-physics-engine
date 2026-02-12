#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "../Utility/Vec2.h"
#include "../Utility/Utils.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../Graphics.h"
#include "Ray.h"
#include "LightMapObject.h"
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
        std::vector<LightMapObject*> lightMap;
    public:
        void AddLightMapObject(LightMapObject* lightObject);
        void ShootRays();
        //Returns true when an object is hit, returns LightMapObject hit.
        LightMapObject* HitObject();
        //TODO: Loops all edges of a polygon object and returns true if the point is on the edge of that object.
        //Think its going to be like this:
        //Grab every object that the ray is going to intersect with, grab closest objected and run this function on it. Rays shouldnt be able to hit multiple objects.
        bool RayIntersect(LightMapObject* obj, Ray* ray);
        bool RayIntersectCircle(LightMapObject* obj, Ray* ray);

        void ProcessRays();
        float clampDegree(float degree);
        LightSource(Vec2 position, float direction, Uint32 color, float intensity, int bounces, int beamSpread);
        ~LightSource();
};
#endif