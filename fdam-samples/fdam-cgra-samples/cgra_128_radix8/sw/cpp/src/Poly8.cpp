//
// Created by lucas on 11/04/19.
//

#include "Poly8.h"

Poly8::Poly8(Cgra *cgra, CgraArch *cgraArch) {
    Poly8::cgraHw = cgra;
    Poly8::cgraArch = cgraArch;
    Poly8::schedulingTime = 0;
    Poly8::cpuExecTime = 0;
    Poly8::cgraExecTime = 0;
    Poly8::cgraConfTime = 0;
}

Poly8::~Poly8() = default;

DataFlow *Poly8::createDataFlow(int id) {
    auto df = new DataFlow(id, "Poly8");
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
    auto reg20 = new PassB(idx++);
    auto reg21 = new PassB(idx++);
    auto reg22 = new PassB(idx++);
    auto reg23 = new PassB(idx++);
    auto reg24 = new PassB(idx++);
    auto reg25 = new PassB(idx++);
    auto reg26 = new PassB(idx++);
    auto reg27 = new PassB(idx++);
    auto reg28 = new PassB(idx++);
    auto reg29 = new PassB(idx++);
    auto reg30 = new PassB(idx++);
    auto reg31 = new PassB(idx++);
    auto reg32 = new PassB(idx++);

    auto add1 = new Addi(idx++, 72);
    auto add2 = new Add(idx++);
    auto add3 = new Addi(idx++,6912);
    auto add4 = new Addi(idx++, 1728);
    auto add5 = new Add(idx++);
    auto add6 = new Add(idx++, 55296);
    auto add7 = new Add(idx++);
    auto add8 = new Add(idx++);
    auto add9 = new Add(idx++);
    auto add10 = new Add(idx++);

    auto mult1 = new Multi(idx++, 4);
    auto mult2 = new Mult(idx++);
    auto mult3 = new Multi(idx++, 432);
    auto mult4 = new Mult(idx++);
    auto mult5 = new Mult(idx++);
    auto mult6 = new Multi(idx++,360);
    auto mult7 = new Mult(idx++);
    auto mult8 = new Multi(idx++, 71);
    auto mult9 = new Mult(idx++);
    auto mult10 = new Multi(idx++, 6);
    auto mult11 = new Mult(idx++);
    auto mult12 = new Multi(idx++, 464);
    auto mult13 = new Mult(idx++);
    auto mult14 = new Mult(idx++);
    auto mult15 = new Mult(idx++);
    auto mult16 = new Mult(idx++);

    auto sub1 = new Subi(idx++, 13824);
    auto sub2 = new Sub(idx++);
    auto sub3 = new Subi(idx++, 4312);
    auto sub4 = new Subi(idx++, 13824);
    auto sub5 = new Sub(idx++);
    auto sub6 = new Subi(idx++, 34);

    auto out = new OutputStream(idx++);

    df->connect(in1, reg24, reg24->getPortA());
    df->connect(reg24, reg25, reg25->getPortA());
    df->connect(reg25, reg26, reg26->getPortA());
    df->connect(reg26, reg27, reg27->getPortA());
    df->connect(reg27, reg28, reg28->getPortA());
    df->connect(reg28, reg29, reg29->getPortA());
    df->connect(reg29, reg30, reg30->getPortA());
    df->connect(reg30, reg31, reg31->getPortA());
    df->connect(reg31, reg32, reg32->getPortA());

    df->connect(in2, reg1, reg1->getPortA());
    df->connect(reg1, reg2, reg2->getPortA());
    df->connect(reg2, reg3, reg3->getPortA());
    df->connect(reg3, reg4, reg4->getPortA());

    df->connect(in2, add1, add1->getPortA());
    df->connect(add1, mult1, mult1->getPortA());
    df->connect(mult1, mult2, mult2->getPortA());
    df->connect(reg2, mult2, mult2->getPortB());

    df->connect(in2, mult3, mult3->getPortA());
    df->connect(mult3, sub1, sub1->getPortA());
    df->connect(sub1, mult4, mult4->getPortA());
    df->connect(reg13, mult4, mult4->getPortB());
    df->connect(mult4, reg5, reg5->getPortA());
    df->connect(reg5, reg6, reg6->getPortA());
    df->connect(reg6, reg7, reg7->getPortA());
    df->connect(reg7, reg8, reg8->getPortA());
    df->connect(reg8, reg9, reg9->getPortA());

    df->connect(in2, sub2, sub2->getPortA());
    df->connect(in3, sub2, sub2->getPortB());
    df->connect(sub2, mult5, mult5->getPortA());
    df->connect(reg1, mult5, mult5->getPortB());

    df->connect(in3, reg12, reg12->getPortA());
    df->connect(reg12, reg13, reg13->getPortA());

    df->connect(in3, mult6, mult6->getPortA());
    df->connect(mult6, add2, add2->getPortA());
    df->connect(add2, reg10, reg10->getPortA());
    df->connect(reg10, add3, add3->getPortA());
    df->connect(reg3, add3, add3->getPortB());
    df->connect(reg4, mult7, mult7->getPortA());
    df->connect(add3, mult7, mult7->getPortB());

    df->connect(in3, mult8, mult8->getPortA());
    df->connect(mult8, add4, add4->getPortA());
    df->connect(add4, add5, add5->getPortA());
    df->connect(mult5, add5, add5->getPortB());
    df->connect(add5, mult9, mult9->getPortA());
    df->connect(reg3, mult9, mult9->getPortB());

    df->connect(in3, mult10, mult10->getPortA());
    df->connect(mult10, sub3, sub3->getPortA());
    df->connect(sub3, mult11, mult11->getPortA());
    df->connect(reg13, mult11, mult11->getPortB());
    df->connect(mult11, add6, add6->getPortA());
    df->connect(add6, reg11, reg11->getPortA());
    df->connect(mult7, add7, add7->getPortA());
    df->connect(reg11, add7, add7->getPortB());

    df->connect(in3, mult12, mult12->getPortA());
    df->connect(mult12, sub4, sub4->getPortA());
    df->connect(sub4, reg14, reg14->getPortA());
    df->connect(reg14, reg15, reg15->getPortA());
    df->connect(reg15, sub5, sub5->getPortA());
    df->connect(mult9, sub5, sub5->getPortB());
    df->connect(sub5, mult13, mult13->getPortA());
    df->connect(reg28, mult13, mult13->getPortB());
    df->connect(mult13, add8, add8->getPortA());
    df->connect(add7, add8, add8->getPortB());
    df->connect(add8, mult14, mult14->getPortA());
    df->connect(reg30, mult14, mult14->getPortB());
    df->connect(mult14, add9, add9->getPortA());
    df->connect(reg9, add9, add9->getPortB());
    df->connect(add9, mult15, mult15->getPortA());
    df->connect(reg32, mult15, mult15->getPortB());
    df->connect(mult15, add10, add10->getPortA());
    df->connect(reg23, add10, add10->getPortB());

    df->connect(in3, sub6, sub6->getPortA());
    df->connect(sub6, mult16, mult16->getPortA());
    df->connect(reg12, mult16, mult16->getPortB());
    df->connect(mult16, reg16, reg16->getPortA());
    df->connect(reg16, reg17, reg17->getPortA());
    df->connect(reg17, reg18, reg18->getPortA());
    df->connect(reg18, reg19, reg19->getPortA());
    df->connect(reg19, reg20, reg20->getPortA());
    df->connect(reg20, reg21, reg21->getPortA());
    df->connect(reg21, reg22, reg22->getPortA());
    df->connect(reg22, reg23, reg23->getPortA());

    df->connect(add10, out, out->getPortA());
    return df;
}
