#ifndef FORCE_H
#define FORCE_H

#include "../Utility/Vec2.h"
#include "Body.h"
#include <vector>
struct Force {
    static Vec2 GenerateDragForce(const Body& body, float k);
    static Vec2 GenerateFrictionForce(const Body& body, float k);
    static Vec2 GenerateGravitationalForce(const Body& a, const Body& b, float g);
    static Vec2 GenerateSpringForce(const Body& body, Vec2 anchor, float restLength, float k);
    static Vec2 GenerateSpringForce(const Body& body, const Body& b, float restLength, float k);
    
    static void GenerateChainForces(Vec2 anchor, float restLength, float springStrength, std::vector<Body*> chainBodies);
    static void GenerateChainBoxForces(float restLength, float springStrength, std::vector<Body*> boxBodies);
};
#endif