//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_AND_H
#define MAIN_AND_H

#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class And : public Operator {
public:
    explicit And(int id) : Operator(id, OP_AND, OP_BASIC) {}
};

class Andi : public Operator {
public:
    Andi(int id, int constant) : Operator(id, OP_AND, OP_IMMEDIATE, constant) {}
};


#endif //MAIN_AND_H
