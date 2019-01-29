//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_OUTPUTSTREAM_H
#define MAIN_OUTPUTSTREAM_H


#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>
#include <fdam/cgra/cgra_arch_defs.h>

class OutputStream : public Operator {
public:
    explicit OutputStream(int id) : Operator(id, OP_PASS_A, OP_OUT) {}

};


#endif //MAIN_OUTPUTSTREAM_H
