//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_MUX_H
#define MAIN_MUX_H

#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class Mux : public Operator {
public:
    explicit Mux(int id) : Operator(id, OP_MUX, OP_BASIC) {}
};

class Muxi : public Operator {
public:
    Muxi(int id, int constant) : Operator(id, OP_MUX, OP_IMMEDIATE, constant) {}
};

#endif //MAIN_MUX_H
