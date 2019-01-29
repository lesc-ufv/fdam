//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_MIN_H
#define MAIN_MIN_H

#include <fdam/cgra/data_flow_defs.h>
#include <fdam/cgra/operator.h>

class Min : public Operator {
public:
    explicit Min(int id) : Operator(id, OP_MIN, OP_BASIC) {}
};

class Mini : public Operator {
public:
    Mini(int id, int constant) : Operator(id, OP_MIN, OP_IMMEDIATE, constant) {}
};


#endif //MAIN_MIN_H
