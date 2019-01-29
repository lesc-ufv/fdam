//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_SUB_H
#define MAIN_SUB_H

#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>


class Sub : public Operator {
public:
    explicit Sub(int id) : Operator(id, OP_SUB, OP_BASIC) {}
};

class Subi : public Operator {
public:
    Subi(int id, int constant) : Operator(id, OP_SUB, OP_IMMEDIATE, constant) {}
};

#endif //MAIN_SUB_H
