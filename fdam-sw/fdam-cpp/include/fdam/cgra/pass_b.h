//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_PASSB_H
#define MAIN_PASSB_H


#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class PassB : public Operator {
public:
    explicit PassB(int id) : Operator(id, OP_PASS_B, OP_BASIC) {}
};

class PassBi : public Operator {
public:
    explicit PassBi(int id, int constant) : Operator(id, OP_PASS_B, OP_IMMEDIATE, constant) {}
};

#endif //MAIN_PASSB_H
