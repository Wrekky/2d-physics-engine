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
JointConstraint::JointConstraint(): Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f) {
    this->cachedLambda.Zero();
}

JointConstraint::JointConstraint(Body* a, Body* b, const Vec2& anchorPoint): Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f) {
    this->cachedLambda.Zero();
    this->a = a;
    this->b = b;

    this->aPoint = a->WorldSpaceToLocalSpace(anchorPoint);
    this->bPoint = b->WorldSpaceToLocalSpace(anchorPoint);
}

void JointConstraint::Solve() {
    const VecN V = GetVelocities();
    const MatMN invM = GetInvM();
    
    const MatMN J = jacobian;
    const MatMN Jt = jacobian.Transpose();
    
    MatMN lhs = J * invM * Jt;
    VecN rhs = J * V * -1.0f;
    rhs[0] -= bias;
    
    VecN lambda = MatMN::SolveGaussSeidel(lhs, rhs);
    cachedLambda += lambda;
    //final impulses
    VecN impulses = Jt * lambda;

    a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1]));
    a->ApplyImpulseAngular(impulses[2]);
    b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4]));
    b->ApplyImpulseAngular(impulses[5]);
}

void JointConstraint::PreSolve(const float dt) {
    // Get anchor point in world space
    const Vec2 pa = a->LocalSpaceToWorldSpace(aPoint);
    const Vec2 pb = b->LocalSpaceToWorldSpace(bPoint);

    const Vec2 ra = pa - a->position;
    const Vec2 rb = pb - b->position;

    jacobian.Zero();

    Vec2 J1 = (pa - pb) * 2.0;
    jacobian.rows[0][0] = J1.x;
    jacobian.rows[0][1] = J1.y;
    
    float J2 = ra.Cross(pa - pb) * 2.0;
    jacobian.rows[0][2] = J2;


    Vec2 J3 = (pb - pa) * 2.0;
    jacobian.rows[0][3] = J3.x;
    jacobian.rows[0][4] = J3.y;
    
    float J4 = rb.Cross(pb - pa) * 2.0;
    jacobian.rows[0][5] = J4;

    //applying cached lambda
    const MatMN Jt = jacobian.Transpose();
    VecN impulses = Jt * cachedLambda;

    a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1]));
    a->ApplyImpulseAngular(impulses[2]);
    b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4]));
    b->ApplyImpulseAngular(impulses[5]);

    //calculate bias
    const float beta = 0.1f;
    float C = (pb - pa).Dot(pb - pa);
    C = std::max(0.0f, C - 0.01f);
    bias = (beta / dt) * C;
}

void JointConstraint::PostSolve() {
    
}

PenetrationConstraint::PenetrationConstraint(): Constraint(), jacobian(2,6), cachedLambda(2), bias(0.0f) {
    cachedLambda.Zero();
    this->friction = 0.0f;
}
PenetrationConstraint::PenetrationConstraint(Body* a, Body* b, const Vec2& aCollisionPoint, 
    const Vec2& bCollisionPoint, const Vec2& normal): Constraint(), jacobian(2,6), cachedLambda(2), bias(0.0f)  {
        this->friction = 0.0f;
        this->a = a;
        this->b = b;
        this->aPoint = a->WorldSpaceToLocalSpace(aCollisionPoint);
        this->bPoint = b->WorldSpaceToLocalSpace(bCollisionPoint);
        this->normal = a->WorldSpaceToLocalSpace(normal);
        cachedLambda.Zero();
}

void PenetrationConstraint::PreSolve(const float dt) {
    //Get the collision points in world space
    const Vec2 pa = a->LocalSpaceToWorldSpace(aPoint);
    const Vec2 pb = b->LocalSpaceToWorldSpace(bPoint);

    Vec2 n = a->LocalSpaceToWorldSpace(normal);

    const Vec2 ra = pa - a->position;
    const Vec2 rb = pb - b->position;

    jacobian.Zero();

    Vec2 J1 = -n;
    jacobian.rows[0][0] = J1.x;
    jacobian.rows[0][1] = J1.y;

    float J2 = -ra.Cross(n);
    jacobian.rows[0][2] = J2;

    Vec2 J3 = n;
    jacobian.rows[0][3] = J3.x;
    jacobian.rows[0][4] = J3.y;

    float J4 = rb.Cross(n);
    jacobian.rows[0][5] = J4;

    //friction stuff
    friction = std::max(a->friction, b->friction);
    if (friction > 0.0) {
        Vec2 t = n.Normal();
        jacobian.rows[1][0] = -t.x;
        jacobian.rows[1][1] = -t.y;
        jacobian.rows[1][2] = -ra.Cross(t);
        jacobian.rows[1][3] = t.x;
        jacobian.rows[1][4] = t.y;
        jacobian.rows[1][5] = rb.Cross(t);
    }
    //Warm starting -- apply cached lambda
    const MatMN Jt = jacobian.Transpose();
    VecN impulses = Jt * cachedLambda;

    a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1]));
    a->ApplyImpulseAngular(impulses[2]);
    b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4]));
    b->ApplyImpulseAngular(impulses[5]);

    const float beta = 0.2f;
    float C = (pb - pa).Dot(-n);
    C = std::min(0.0f, C + 0.01f);
    bias = (beta/dt) * C;
}

void PenetrationConstraint::Solve() {
    const VecN V = GetVelocities();
    const MatMN invM = GetInvM();
    
    const MatMN J = jacobian;
    const MatMN Jt = jacobian.Transpose();
    
    MatMN lhs = J * invM * Jt;
    VecN rhs = J * V * -1.0f;
    rhs[0] -= bias;
    
    VecN lambda = MatMN::SolveGaussSeidel(lhs, rhs);

    if (friction > 0.0) {
        const float maxFriction = cachedLambda[0] * friction;
        cachedLambda[1] = std::clamp(cachedLambda[1], -maxFriction, maxFriction);
    }
    
    VecN oldLambda = cachedLambda;
    cachedLambda += lambda;
    cachedLambda[0] = (cachedLambda[0] < 0.0f ? 0.0f : cachedLambda[0]);
    lambda = cachedLambda - oldLambda; 
    //final impulses
    VecN impulses = Jt * lambda;

    a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1]));
    a->ApplyImpulseAngular(impulses[2]);
    b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4]));
    b->ApplyImpulseAngular(impulses[5]);
}
void PenetrationConstraint::PostSolve() {

}
