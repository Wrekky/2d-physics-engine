#ifndef BODY_H
#define BODY_H

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

    Shape* shape = NULL;

    void Update(float deltaTime);

    bool IsStatic() const;
    void ApplyImpulse(const Vec2& j);
    void ApplyImpulse(const Vec2& j, const Vec2& r);
    void AddForce(const Vec2& force);
    void ClearForces();
    void AddTorque(float torque);
    void ClearTorque();
    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

    Body(const Shape& shape, float x, float y, float mass);
    ~Body();
};
#endif