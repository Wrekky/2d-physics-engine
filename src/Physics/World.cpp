#include "World.h"
World::World(float gravity) {
    this->G = gravity;
}

World::~World() {
    for (auto body : bodies) {
        body->~Body();
        delete body;
    }
}

void World::AddBody(Body* body) {
    bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies() {
    return bodies;
}

void World::AddForce(const Vec2& force) {
    forces.push_back(force);
}

void World::AddTorque(float t) {
    torque.push_back(t);
}

void World::Update(float dt) {
    //apply world forces
    for (auto body : bodies) {
        Vec2 weight = Vec2(0.0, body->mass * G);
        body->AddForce(weight);
        for (auto f : forces) {
            body->AddForce(f);
        }
        for (auto t : torque) {
            body->AddTorque(t);
        }
    }

    for (auto body : bodies) {
        body->IntegrateForces(dt);
    }
    //solve all constraints
    for (int i = 0; i < 50; i++) {
        for (auto& constraint : constraints)  {
            constraint->Solve();
        }
    }

        
    //integrate velocities
    for (auto body : bodies) {
        body->IntegrateVelocities(dt);
    }
    
    CheckCollisions();
}

void World::CheckCollisions() {
    if (bodies.size() > 1) {
        for (int i = 0; i <= bodies.size() - 1; i++)
        {
            for (int j = i + 1; j < bodies.size(); j++)
            {
                Body *a = bodies[i];
                Body *b = bodies[j];
                //TODO: remove isColliding bool
                a->isColliding = false;
                b->isColliding = false;
                Contact contact;
                if (CollisionDetection::IsColliding(a, b, contact))
                {
                    contact.ResolveCollision();
                    a->isColliding = true;
                    b->isColliding = true;
                }
            }
        }
    }
}

void World::AddConstraint(Constraint* constraint) {
    constraints.push_back(constraint);
}

std::vector<Constraint*>& World::GetConstraints() {
    return constraints;
}