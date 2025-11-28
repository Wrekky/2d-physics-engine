#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"
#include "MatMN.h"
class Constraint {
    public:
        Body* a;
        Body* b;

        Vec2 aPoint;
        Vec2 bPoint;

        virtual ~Constraint() = default;

        //TODO Helper matrix & vector class:
        MatMN GetInvM() const;
        VecN GetVelocities() const;

        virtual void PreSolve(const float dt) {}
        virtual void Solve() {}
        virtual void PostSolve() {}
};

class JointConstraint : public Constraint {
    private:
        MatMN jacobian;
        VecN cachedLambda;
        float bias;
    public:
        JointConstraint();
        JointConstraint(Body* a, Body* b, const Vec2& anchorPoint);
        void PreSolve(const float dt) override;
        void Solve() override;
        void PostSolve() override;
};

class PenetrationConstraint : public Constraint {
    private:
        MatMN jacobian;
    public: 
        //TODO: presolve, postsolve, solve
        //void Solve() override;
};
#endif
