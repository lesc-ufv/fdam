//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_XOR_H
#define MAIN_XOR_H

#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class Xor : public Operator {
public:
    explicit Xor(int id) : Operator(id, OP_XOR, OP_BASIC) {}
};

class Xori : public Operator {
public:
    Xori(int id, int constant) : Operator(id, OP_XOR, OP_IMMEDIATE, constant) {}
};

#endif //MAIN_XOR_H
