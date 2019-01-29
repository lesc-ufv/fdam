//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_SGT_H
#define MAIN_SGT_H

#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class Sgt : public Operator {
public:
    explicit Sgt(int id) : Operator(id, OP_SGT, OP_BASIC) {}
};

class Sgti : public Operator {
public:
    explicit Sgti(int id, int constant) : Operator(id, OP_SGT, OP_IMMEDIATE, constant) {}
};


#endif //MAIN_SGT_H
