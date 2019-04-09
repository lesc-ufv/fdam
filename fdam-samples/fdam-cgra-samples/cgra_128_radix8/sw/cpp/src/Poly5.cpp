//
// Created by lucas on 08/04/19.
//

#include <Poly5.h>

Poly5::Poly5(Cgra *cgra, CgraArch *cgraArch) {
    Poly5::cgraHw = cgra;
    Poly5::cgraArch = cgraArch;
    Poly5::schedulingTime = 0;
    Poly5::cpuExecTime = 0;
    Poly5::cgraExecTime = 0;
    Poly5::cgraConfTime = 0;
}

Poly5::~Poly5() = default;

DataFlow *Poly5::createDataFlow(int id) {
    auto df = new DataFlow(id, "Poly5");
    int idx = 0;
    auto in1 = new InputStream(idx++);
    auto in2 = new InputStream(idx++);
    auto in3 = new InputStream(idx++);
    auto reg1 = new PassB(idx++);
    auto reg2 = new PassB(idx++);
    auto reg3 = new PassB(idx++);
    auto reg4 = new PassB(idx++);
    auto reg5 = new PassB(idx++);
    auto reg6 = new PassB(idx++);
    auto reg7 = new PassB(idx++);
    auto reg8 = new PassB(idx++);
    auto reg9 = new PassB(idx++);
    auto reg20 = new PassB(idx++);
    auto reg10 = new PassB(idx++);
    auto reg11 = new PassB(idx++);
    auto reg12 = new PassB(idx++);
    auto reg13 = new PassB(idx++);
    auto reg14 = new PassB(idx++);
    auto reg15 = new PassB(idx++);
    auto reg16 = new PassB(idx++);
    auto reg17 = new PassB(idx++);
    auto reg18 = new PassB(idx++);
    auto reg19 = new PassB(idx++);
    auto add = new Add(idx++);
    auto sub1 = new Subi(idx++, 432);
    auto sub2 = new Subi(idx++, 207);
    auto mult1 = new Multi(idx++, 78);
    auto mult2 = new Multi(idx++, 288);
    auto mult3 = new Multi(idx++, 2);
    auto add1 = new Addi(idx++, 144);
    auto sub3 = new Subi(idx++, 9504);
    auto sub4 = new Subi(idx++, 5184);
    auto mult4 = new Mult(idx++);
    auto mult5 = new Mult(idx++);
    auto sub41 = new Sub(idx++);
    auto add2 = new Addi(idx++, 62208);
    auto mult6 = new Mult(idx++);
    auto mult7 = new Mult(idx++);
    auto add3 = new Addi(idx++, 3456);
    auto mult8 = new Mult(idx++);
    auto mult9 = new Mult(idx++);
    auto add4 = new Sub(idx++);
    auto sub5 = new Add(idx++);
    auto sub6 = new Subi(idx++, 298584);
    auto sub7 = new Subi(idx++, 248832);
    auto mult13 = new Mult(idx++);
    auto mult10 = new Mult(idx++);
    auto mult11 = new Mult(idx++);
    auto add5 = new Sub(idx++);
    auto add6 = new Sub(idx++);
    auto mult12 = new Mult(idx++);
    auto out = new OutputStream(idx++);

    df->connect(in3, sub1, sub1->getPortA());
    df->connect(in3, sub2, sub2->getPortA());
    df->connect(in3, mult1, mult1->getPortA());
    df->connect(in3, mult2, mult2->getPortA());
    df->connect(in3, mult3, mult3->getPortA());
    df->connect(in3, reg1, reg1->getPortA());
    df->connect(reg1, reg2, reg2->getPortA());
    df->connect(reg2, reg3, reg3->getPortA());
    df->connect(reg3, reg4, reg4->getPortA());

    df->connect(in1, reg5, reg5->getPortA());
    df->connect(reg5, reg6, reg6->getPortA());
    df->connect(reg6, reg7, reg7->getPortA());
    df->connect(reg7, reg8, reg8->getPortA());
    df->connect(reg8, reg9, reg9->getPortA());
    df->connect(reg9, reg20, reg20->getPortA());

    df->connect(in2, add1, add1->getPortA());
    df->connect(in2, reg10, reg10->getPortA());
    df->connect(reg10, reg11, reg11->getPortA());
    df->connect(reg11, reg12, reg12->getPortA());
    df->connect(reg12, reg13, reg13->getPortA());
    df->connect(reg13, reg14, reg14->getPortA());
    df->connect(reg14, reg15, reg15->getPortA());
    df->connect(reg15, reg16, reg16->getPortA());
    df->connect(reg16, reg17, reg17->getPortA());
    df->connect(reg17, reg18, reg18->getPortA());

    df->connect(mult1, sub3, sub3->getPortA());
    df->connect(mult2, sub4, sub4->getPortA());
    df->connect(sub1, mult4, mult4->getPortA());
    df->connect(reg1, mult4, mult4->getPortB());
    df->connect(sub2, mult5, mult5->getPortA());
    df->connect(reg1, mult5, mult5->getPortB());
    df->connect(add1, sub5, sub5->getPortA());
    df->connect(mult3, sub5, sub5->getPortB());

    //

    df->connect(mult4, add2, add2->getPortA());
    df->connect(mult5, add3, add3->getPortA());
    df->connect(sub3, mult6, mult6->getPortA());
    df->connect(reg4, mult6, mult6->getPortB());
    df->connect(sub41, mult7, mult7->getPortA());
    df->connect(reg7, mult7, mult7->getPortB());
    df->connect(sub5, mult8, mult8->getPortA());
    df->connect(reg7, mult8, mult8->getPortB());

    df->connect(add2, mult8, mult8->getPortA());
    df->connect(reg4, mult8, mult8->getPortB());

    df->connect(mult6, add4, add4->getPortA());
    df->connect(mult7, add4, add4->getPortB());

    df->connect(add3, sub5, sub5->getPortA());
    //df->connect(mult8, sub5, sub5->getPortB());

    df->connect(mult9, sub6, sub6->getPortA());
    df->connect(add4, sub7, sub7->getPortA());
    df->connect(sub5, mult13, mult13->getPortA());
    df->connect(reg9, mult13, mult13->getPortB());

    df->connect(sub7, mult10, mult10->getPortA());
    df->connect(reg20, mult10, mult10->getPortB());

    df->connect(mult13, mult11, mult11->getPortA());
    df->connect(reg13, mult11, mult11->getPortB());

    df->connect(sub6, add6, add6->getPortA());
    df->connect(mult10, add6, add6->getPortB());

    df->connect(mult11, reg19, reg19->getPortA());
    df->connect(reg19, add5, add5->getPortA());
    df->connect(add, add5, add5->getPortB());

    df->connect(reg18, mult12, mult12->getPortA());
    df->connect(add6, mult12, mult12->getPortB());

    df->connect(mult12, out, out->getPortA());
    return df;
}


