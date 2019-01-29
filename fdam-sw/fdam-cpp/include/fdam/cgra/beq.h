//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_BEQ_H
#define MAIN_BEQ_H

#include <fdam/cgra/data_flow_defs.h>
#include <fdam/cgra/operator.h>

class Beq : public Operator {
public:
    explicit Beq(int id) : Operator(id, OP_BEQ, OP_BASIC) {}
};

class Beqi : public Operator {
public:
    Beqi(int id, int constant) : Operator(id, OP_BEQ, OP_IMMEDIATE, constant) {}
};

#endif //MAIN_BEQ_H
