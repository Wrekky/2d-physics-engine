#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"

class Constraint {
    public:
        Body* a;
        Body* b;

        //TODO Helper matrix & vector class:
        MatMN GetInvM(6, 6);
        VecN vec();
        void Solve();
};

#endif