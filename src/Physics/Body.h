#ifndef BODY_H
#define BODY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vec2.h"
#include "Shape.h"
// TODO:
struct Body
{
    bool isColliding = false;
    
    Vec2 sumForces;
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float sumTorque;
    float angularAcceleration;
    float angularVelocity;
    float rotation;

    float friction;
    float restitution;
    float mass;
    float invMass;
    float I;
    float invI;

    Uint32 color;
    
    Shape* shape = NULL;

    SDL_Texture* texture = nullptr;

    bool IsStatic() const;
    void ApplyImpulseLinear(const Vec2& j);
    void ApplyImpulseAngular(const float j);
    void ApplyImpulseAtPoint(const Vec2& j, const Vec2& r);
    void AddForce(const Vec2& force);
    void ClearForces();
    void AddTorque(float torque);
    void ClearTorque();

    void SetTexture(const char* textureFileName);


    Vec2 LocalSpaceToWorldSpace(const Vec2& point) const;
    Vec2 WorldSpaceToLocalSpace(const Vec2& point) const;
    
    void IntegrateForces(const float dt);
    void IntegrateVelocities(const float dt);
    Body(const Shape& shape, float x, float y, float mass);
    ~Body();
};
#endif