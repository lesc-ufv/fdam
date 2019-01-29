//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_MAX_H
#define MAIN_MAX_H


#include <fdam/cgra/data_flow_defs.h>
#include <fdam/cgra/operator.h>

class Max : public Operator {
public:
    explicit Max(int id) : Operator(id, OP_MAX, OP_BASIC) {}
};

class Maxi : public Operator {
public:
    Maxi(int id, int constant) : Operator(id, OP_MAX, OP_IMMEDIATE, constant) {}
};


#endif //MAIN_MAX_H
