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

        virtual void Solve() {}
};

class JointConstraint : public Constraint {
    private:
        MatMN jacobian;
    public:
        JointConstraint();
        JointConstraint(Body* a, Body* b, const Vec2& anchorPoint);
        void Solve() override;
};

class PenetrationConstraint : public Constraint {
    private:
        MatMN jacobian;
    public: 
        //void Solve() override;
};
#endif
