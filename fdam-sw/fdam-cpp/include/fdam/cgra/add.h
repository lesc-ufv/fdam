//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_ADD_H
#define MAIN_ADD_H


#include <fdam/cgra/data_flow_defs.h>
#include <fdam/cgra/operator.h>

class Add : public Operator {
public:
    explicit Add(int id) : Operator(id, OP_ADD, OP_BASIC) {}
};

class Addi : public Operator {
public:
    Addi(int id, int constant) : Operator(id, OP_ADD, OP_IMMEDIATE, constant) {}
};

#endif //MAIN_ADD_H
