//
// Created by lucas on 13/01/19.
//

#include <Samples.h>

DataFlow *Samples::loopback(int id, int pipe){

    char name[20];
    sprintf(name,"%s_%d","loopback",pipe);
    DataFlow * df = new DataFlow(id,name);

    auto in = new Operator(0,OP_PASS_A,OP_IN);
    auto out =  new Operator(1,OP_PASS_A,OP_OUT);
    df->addOperator(in);
    df->addOperator(out);
    for (int i = 0; i < pipe-1; ++i) {
        auto r = new Operator(2+i,OP_PASS_A,OP_BASIC);
        df->addOperator(r);
        df->connect(in,r, PORT_A);
        in = r;
    }
    df->connect(in,out, PORT_A);

    return df;
}