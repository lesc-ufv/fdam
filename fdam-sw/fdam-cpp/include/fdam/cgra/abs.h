//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_ABS_H
#define MAIN_ABS_H

#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class Abs : public Operator {
public:
    explicit Abs(int id) : Operator(id, OP_ABS, OP_BASIC) {}
};

#endif //MAIN_ABS_H
