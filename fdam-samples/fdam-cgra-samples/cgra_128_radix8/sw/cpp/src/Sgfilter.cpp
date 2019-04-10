//
// Created by lucas on 08/04/19.
//

#include <Sgfilter.h>

Sgfilter::Sgfilter(Cgra *cgra, CgraArch *cgraArch) {
    Sgfilter::cgraHw = cgra;
    Sgfilter::cgraArch = cgraArch;
    Sgfilter::schedulingTime = 0;
    Sgfilter::cpuExecTime = 0;
    Sgfilter::cgraExecTime = 0;
    Sgfilter::cgraConfTime = 0;
}

Sgfilter::~Sgfilter() = default;

DataFlow *Sgfilter::createDataFlow(int id) {
    auto df = new DataFlow(id, "Sgfilter");
    int idx = 0;
    auto in1 = new InputStream(idx++);
    auto in2 = new InputStream(idx++);
    auto reg1 = new PassB(idx++);
    auto reg2 = new PassB(idx++);
    auto reg3 = new PassB(idx++);
    auto reg4 = new PassB(idx++);
    auto reg5 = new PassB(idx++);
    auto reg6 = new PassB(idx++);
    auto reg7 = new PassB(idx++);
    auto reg8 = new PassB(idx++);
    auto reg9 = new PassB(idx++);
    auto reg10 = new PassB(idx++);
    auto reg11 = new PassB(idx++);
    auto reg12 = new PassB(idx++);
    auto reg13 = new PassB(idx++);
    auto reg14 = new PassB(idx++);
    auto reg15 = new PassB(idx++);
    auto mult1 = new Multi(idx++, 92);
    auto mult2 = new Multi(idx++, -984);
    auto mult3 = new Multi(idx++, -76);
    auto mult4 = new Multi(idx++, 7);
    auto sub1 = new Subi(idx++, 46);
    auto sub2 = new Subi(idx++, 39);
    auto add1 = new Add(idx++);
    auto mult5 = new Mult(idx++);
    auto mult6 = new Mult(idx++);
    auto add2 = new Addi(idx++, 7);
    auto sub3 = new Subi(idx++, 46);
    auto mult7 = new Mult(idx++);
    auto mult8 = new Mult(idx++);
    auto add3 = new Add(idx++);
    auto add4 = new Addi(idx++, 7);
    auto mult9 = new Mult(idx++);
    auto add5 = new Add(idx++);
    auto sub4 = new Subi(idx++, 75);
    auto out = new OutputStream(idx++);

    df->connect(in1, mult1, mult1->getPortA());
    df->connect(in1, mult2, mult2->getPortA());
    df->connect(in1, mult3, mult3->getPortA());
    df->connect(in1, reg11, reg11->getPortA());
    df->connect(reg11, reg12, reg12->getPortA()); //TODO:
    df->connect(in1, reg1, reg1->getPortA());
    df->connect(reg1, reg2, reg2->getPortA());
    df->connect(reg2, reg3, reg3->getPortA());
    df->connect(reg3, reg4, reg4->getPortA());
    df->connect(in2, mult4, mult4->getPortA());
    df->connect(in2, reg5, reg5->getPortA());
    df->connect(reg5, reg6, reg6->getPortA());
    df->connect(reg6, reg7, reg7->getPortA());
    df->connect(reg7, reg8, reg8->getPortA());
    df->connect(reg8, reg9, reg9->getPortA());
    df->connect(reg9, reg10, reg10->getPortA());
    df->connect(mult2, sub1, sub1->getPortA());
    df->connect(mult1, sub2, sub2->getPortA());
    df->connect(mult3, add1, add1->getPortA());
    df->connect(mult4, add1, add1->getPortB());
    df->connect(reg12, mult5, mult5->getPortA());
    df->connect(sub1, mult5, mult5->getPortB());
    df->connect(reg2, mult6, mult6->getPortA());
    df->connect(sub2, mult6, mult6->getPortB());
    df->connect(add1, add2, add2->getPortA());
    df->connect(mult5, sub3, sub3->getPortA());
    df->connect(reg7, mult7, mult7->getPortA());
    df->connect(add2, mult7, mult7->getPortB());
    df->connect(reg4, mult8, mult8->getPortA());
    df->connect(sub3, mult8, mult8->getPortB());
    df->connect(mult6, reg13, reg13->getPortA());
    df->connect(reg13, add3, add3->getPortA());
    df->connect(mult7, add3, add3->getPortB());
    df->connect(add3, add4, add4->getPortA());
    df->connect(add4, mult9, mult9->getPortA());
    df->connect(reg10, mult9, mult9->getPortB());
    df->connect(mult9, add5, add5->getPortA());
    df->connect(mult8, reg14, reg14->getPortB());
    df->connect(reg14, reg15, reg15->getPortA());
    df->connect(reg15, add5, add5->getPortA());
    df->connect(add5, sub4, sub4->getPortA());
    df->connect(sub4, out, out->getPortA());

    return df;
}


