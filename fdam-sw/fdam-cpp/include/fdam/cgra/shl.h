//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_SHL_H
#define MAIN_SHL_H

#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class Shl : public Operator {
public:
    explicit Shl(int id) : Operator(id, OP_SHL, OP_BASIC) {}
};

class Shli : public Operator {
public:
    explicit Shli(int id, int constant) : Operator(id, OP_SHL, OP_IMMEDIATE, constant) {}
};


#endif //MAIN_SHL_H
