//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_SLT_H
#define MAIN_SLT_H

#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class Slt : public Operator {
public:
    explicit Slt(int id) : Operator(id, OP_SLT, OP_BASIC) {}
};

class Slti : public Operator {
public:
    explicit Slti(int id, int constant) : Operator(id, OP_SLT, OP_IMMEDIATE, constant) {}
};


#endif //MAIN_SLT_H
