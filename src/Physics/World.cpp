#include "./World.h"
#include "./Contact.h"
#include "./CollisionDetection.h"
World::World(float gravity) {
    this.G = gravity;
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
    return &bodies;
}

void World::AddForce(const Vec2& force) {
    forces.push_back(force);
}

void World::AddTorque(float torque) {
    torque.push_back(torque);
};

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
        body->Update(dt);
    }
}

void World::CheckCollisions() {
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