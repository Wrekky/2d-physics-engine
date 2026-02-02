#include "World.h"
World::World(float gravity) {
    this->G = gravity;
}

World::~World() {
    for (auto body : bodies) {
        body->~Body();
        delete body;
    }
    for (auto constraint : constraints) {
        constraint->~Constraint();
        delete constraint;
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
    //Createa a vector of penetration constraints that will be solved per frame
    std::vector<PenetrationConstraint> penetrations;
    //apply world forces
    for (auto& body : bodies) {
        Vec2 weight = Vec2(0.0, body->mass * G);
        body->AddForce(weight);
        for (auto f : forces) {
            body->AddForce(f);
        }
        for (auto t : torque) {
            body->AddTorque(t);
        }
    }

    for (auto& body : bodies) {
        body->IntegrateForces(dt);
    }

    if (bodies.size() > 1)
    {
        for (int i = 0; i <= bodies.size() - 1; i++)
        {
            for (int j = i + 1; j < bodies.size(); j++)
            {
                Body *a = bodies[i];
                Body *b = bodies[j];
                //TODO Find an algorithm for finding all edge contact points.
                std::vector<Contact> contacts;
                if (CollisionDetection::IsColliding(a, b, contacts))
                {
                    for (auto contact : contacts) {
                        //debug stuff for drawing collision points
                        if (debug) {
                            Graphics::DrawCircle(contact.start.x, contact.start.y, 5, 0.0, 0xFF00FFFF);
                            Graphics::DrawCircle(contact.end.x, contact.end.y, 2, 0.0, 0xFF00FFFF);
                        }
                        //add a newpenetration constraint
                        PenetrationConstraint penetration = PenetrationConstraint(contact.a, contact.b, contact.start, contact.end, contact.normal);
                        penetrations.push_back(penetration);
                    }
                }
            }
        }
    }
    //solve all constraints
    for (auto& constraint: constraints) {
        constraint->PreSolve(dt);
    }
    for (auto& constraint: penetrations) {
        constraint.PreSolve(dt);
    }
    for (int i = 0; i < 5; i++) {
        for (auto& constraint : constraints)  {
            constraint->Solve();
        }
        for (auto& constraint: penetrations) {
            constraint.Solve();
        }
    }
    for (auto& constraint: penetrations) {
        constraint.PostSolve();
    }
    for (auto& constraint: constraints) {
        constraint-> PostSolve();
    }
    //integrate velocities
    for (auto& body : bodies) {
        body->IntegrateVelocities(dt);
    }
    
}

void World::AddConstraint(Constraint* constraint) {
    constraints.push_back(constraint);
}

std::vector<Constraint*>& World::GetConstraints() {
    return constraints;
}