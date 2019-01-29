//
// Created by lucas on 18/01/19.
//

#ifndef MAIN_INPUTSTREAM_H
#define MAIN_INPUTSTREAM_H


#include <fdam/cgra/operator.h>
#include <fdam/cgra/data_flow_defs.h>
#include <fdam/cgra/cgra_arch_defs.h>

class InputStream : public Operator {
public:
    explicit InputStream(int id) : Operator(id, OP_PASS_A, OP_IN) {}
};


#endif //MAIN_INPUTSTREAM_H
