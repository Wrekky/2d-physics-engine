#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"
#include "MatMN.h"
class Constraint {
    public:
        Body* a;
        Body* b;

        virtual ~Constraint() = default;

        //TODO Helper matrix & vector class:
        MatMN GetInvM() const;
        VecN GetVelocities() const;

        virtual void Solve() {}
};

class DistanceConstraint : public Constraint {
    //void Solve() override;
};

class PenetrationConstraint : public Constraint {
    //void Solve() override;
};
#endif
