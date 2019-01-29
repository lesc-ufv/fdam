//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_SHR_H
#define MAIN_SHR_H


#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class Shr : public Operator {
public:
    explicit Shr(int id) : Operator(id, OP_SHR, OP_BASIC) {}
};

class Shri : public Operator {
public:
    explicit Shri(int id, int constant) : Operator(id, OP_SHR, OP_IMMEDIATE, constant) {}
};

#endif //MAIN_SHR_H
