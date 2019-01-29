//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_BNE_H
#define MAIN_BNE_H


#include <fdam/cgra/data_flow_defs.h>
#include <fdam/cgra/operator.h>

class Bne : public Operator {
public:
    explicit Bne(int id) : Operator(id, OP_BNE, OP_BASIC) {}
};

class Bnei : public Operator {
public:
    Bnei(int id, int constant) : Operator(id, OP_BNE, OP_IMMEDIATE, constant) {}
};

#endif //MAIN_BNE_H
