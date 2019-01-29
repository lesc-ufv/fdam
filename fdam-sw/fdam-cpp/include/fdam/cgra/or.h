//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_OR_H
#define MAIN_OR_H

#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class Or : public Operator {
public:
    explicit Or(int id) : Operator(id, OP_OR, OP_BASIC) {}
};

class Ori : public Operator {
public:
    explicit Ori(int id, int constant) : Operator(id, OP_OR, OP_IMMEDIATE, constant) {}
};

#endif //MAIN_OR_H
