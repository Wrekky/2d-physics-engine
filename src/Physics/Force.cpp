#include "Force.h"
//You mark a variable a const if you're referencing it and not changing it.
//Shows intent to NOT change the variable information.
Vec2 Force::GenerateDragForce(const Body& body, float k) {
    Vec2 dragForce = Vec2(0,0);
    if (body.velocity.MagnitudeSquared() > 0)
    {
        Vec2 dragDirection = body.velocity.UnitVector() * -1.0;
        float dragMagnitude = k * body.velocity.MagnitudeSquared();
        dragForce = dragDirection * dragMagnitude;
    }
    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Body& body, float k) {
    Vec2 frictionForce = body.velocity.UnitVector() * -1.0;
    frictionForce *= k;
    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Body& a, const Body& b, float g) {
    Vec2 d = (b.position - a.position);

    float distanceSquared = d.MagnitudeSquared();

    Vec2 attractionDirection = d.UnitVector();

    float attractionMagnitude = g * (a.mass * b.mass) / distanceSquared;

    Vec2 attractionForce = attractionDirection * attractionMagnitude;
}

Vec2 Force::GenerateSpringForce(const Body& body, Vec2 anchor, float restLength, float k) {
    Vec2 d = body.position - anchor;

    float displacement = d.Magnitude() - restLength;

    Vec2 springDirection = d.UnitVector();
    float springMagnitude = -k * displacement;

    Vec2 springForce = springDirection * springMagnitude;

    return springForce;
}

Vec2 Force::GenerateSpringForce(const Body& body, const Body& b, float restLength, float k) {
    Vec2 d = body.position - b.position;

    float displacement = d.Magnitude() - restLength;

    Vec2 springDirection = d.UnitVector();
    float springMagnitude = -k * displacement;

    Vec2 springForce = springDirection * springMagnitude;

    return springForce;
}

//Creates a chain of bodys. Adds the required force for every body.
void Force::GenerateChainForces(Vec2 anchor, float restLength, float springStrength, std::vector<Body*> chainBodys) {
    //apply spring force on every body. order? each body needs to understand which body its attached to.
    //spring force will be applied in the order of the array.
    Vec2 springForce = GenerateSpringForce(*chainBodys[0], anchor, restLength, springStrength);
    chainBodys[0]->AddForce(springForce);
    for (int i = 1; i < chainBodys.size(); i++) {
        springForce = GenerateSpringForce(*chainBodys[i], *chainBodys[i-1], restLength, springStrength);
        chainBodys[i]->AddForce(springForce);
        chainBodys[i-1]->AddForce(-springForce);
    }
}

//make sure boxBodys is 4 bodys.
void Force::GenerateChainBoxForces(float restLength, float springStrength, std::vector<Body*> boxBodys) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (x != y) {
                Vec2 springForce = GenerateSpringForce(*boxBodys[x], *boxBodys[y], restLength, springStrength);
                boxBodys[x]->AddForce(springForce);
            }
        }
    }
}
