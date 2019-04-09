//
// Created by lucas on 08/04/19.
//

#include <Chebyshev.h>

Chebyshev::Chebyshev(Cgra *cgra, CgraArch *cgraArch) {
    Chebyshev::cgraHw = cgra;
    Chebyshev::cgraArch = cgraArch;
    Chebyshev::schedulingTime = 0;
    Chebyshev::cpuExecTime = 0;
    Chebyshev::cgraExecTime = 0;
    Chebyshev::cgraConfTime = 0;
}

Chebyshev::~Chebyshev() = default;

DataFlow *Chebyshev::createDataFlow(int id) {
    auto df = new DataFlow(id, "chebyshev");
    int idx = 0;
    auto in = new InputStream(idx++);
    auto reg1 = new PassB(idx++);
    auto reg2 = new PassB(idx++);
    auto reg3 = new PassB(idx++);
    auto reg4 = new PassB(idx++);
    auto mult1 = new Multi(idx++, 16);
    auto mult2 = new Mult(idx++);
    auto sub1 = new Subi(idx++, 20);
    auto mult3 = new Mult(idx++);
    auto mult4 = new Mult(idx++);
    auto add1 = new Addi(idx++, 5);
    auto mult5 = new Mult(idx++);
    auto out = new OutputStream(idx++);

    df->connect(in, mult1, mult1->getPortA());
    df->connect(in, reg1, reg1->getPortA());
    //pilha de regs
    df->connect(reg1, reg2, reg2->getPortA());
    df->connect(reg2, reg3, reg3->getPortA());
    df->connect(reg3, reg4, reg4->getPortA());

    df->connect(reg1, mult2, mult2->getPortA());
    df->connect(mult1, mult2, mult2->getPortB());
    df->connect(mult2, sub1, sub1->getPortA());
    df->connect(reg2, mult3, mult3->getPortA());
    df->connect(sub1, mult3, mult3->getPortB());
    df->connect(reg3, mult4, mult4->getPortA());
    df->connect(mult3, mult4, mult4->getPortB());
    df->connect(mult4, add1, add1->getPortA());
    df->connect(reg4, mult5, mult5->getPortA());
    df->connect(add1, mult5, mult5->getPortB());

    df->connect(mult5, out, out->getPortA());
    return df;
}
