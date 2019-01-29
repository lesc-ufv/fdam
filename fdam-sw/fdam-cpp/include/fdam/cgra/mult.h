//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_MULT_H
#define MAIN_MULT_H


#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class Mult : public Operator {
public:
    explicit Mult(int id) : Operator(id, OP_MULT, OP_BASIC) {}
};

class Multi : public Operator {
public:
    Multi(int id, int constant) : Operator(id, OP_MULT, OP_IMMEDIATE, constant) {}
};


#endif //MAIN_MULT_H
