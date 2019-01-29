//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_NOT_H
#define MAIN_NOT_H

#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>

class Not : public Operator {
public:
    explicit Not(int id) : Operator(id, OP_NOT, OP_BASIC) {}
};


#endif //MAIN_NOT_H
