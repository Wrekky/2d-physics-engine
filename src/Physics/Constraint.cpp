#include "Constraint.h"

MatMN Constraint::GetInvM() const {
    MatMN invM(6,6);

    invM.Zero();

    invM.rows[0][0] = a->invMass;
    invM.rows[1][1] = a->invMass;
    invM.rows[2][2] = a->invI;

    invM.rows[3][3] = b->invMass;
    invM.rows[4][4] = b->invMass;
    invM.rows[5][5] = b->invI;
    return invM;
}
VecN Constraint::GetVelocities() const {
    VecN V(6);
    V.Zero();

    V[0] = a->velocity.x;
    V[1] = a->velocity.y;
    V[2] = a->angularVelocity;

    V[3] = b->velocity.x;
    V[4] = b->velocity.y;
    V[5] = b->angularVelocity;
    return V;
}

//creates a MatMN & initializes with default constraint constructor
JointConstraint::JointConstraint(): Constraint(), jacobian(1, 6) {

}

JointConstraint::JointConstraint(Body* a, Body* b, const Vec2& anchorPoint): Constraint(), jacobian(1, 6) {
    this->a = a;
    this->b = b;

    this->aPoint = a->WorldSpaceToLocalSpace(anchorPoint);
    this->bPoint = b->WorldSpaceToLocalSpace(anchorPoint);
}

void JointConstraint::Solve() {
    //TODO
}