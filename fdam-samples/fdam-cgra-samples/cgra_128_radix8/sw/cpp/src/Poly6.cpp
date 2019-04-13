//
// Created by lucas on 11/04/19.
//

#include "Poly6.h"

Poly6::Poly6(Cgra *cgra, CgraArch *cgraArch) {
    Poly6::cgraHw = cgra;
    Poly6::cgraArch = cgraArch;
    Poly6::schedulingTime = 0;
    Poly6::cpuExecTime = 0;
    Poly6::cgraExecTime = 0;
    Poly6::cgraConfTime = 0;
}

Poly6::~Poly6() = default;

DataFlow *Poly6::createDataFlow(int id) {
    auto df = new DataFlow(id, "Poly6");
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
    //auto reg20 = new PassB(idx++);
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
    auto reg33 = new PassB(idx++);
    auto reg34 = new PassB(idx++);
    auto reg35 = new PassB(idx++);
    auto reg36 = new PassB(idx++);
    auto reg37 = new PassB(idx++);
    auto reg38 = new PassB(idx++);
    auto reg39 = new PassB(idx++);

    auto add1 = new Addi(idx++, 216);
    auto add2 = new Add(idx++);
    auto add3 = new Addi(idx++, 6912);
    auto add31 = new Add(idx++);
    auto add4 = new Addi(idx++, 1728);

    auto add5 = new Add(idx++);
    auto add6 = new Addi(idx++, 55296);
    auto add7 = new Add(idx++);

    auto mult1 = new Multi(idx++, 8);
    auto mult2 = new Mult(idx++);
    auto mult3 = new Mult(idx++);
    auto mult4 = new Mult(idx++);
    auto mult5 = new Multi(idx++, 6);
    auto mult6 = new Mult(idx++);
    auto mult7 = new Mult(idx++);
    auto mult8 = new Mult(idx++);
    auto mult9 = new Multi(idx++, 124416);
    auto mult10 = new Multi(idx++, 4);
    auto mult11 = new Multi(idx++, 2985984);
    auto mult12 = new Multi(idx++, 414);
    auto mult13 = new Mult(idx++);
    auto mult14 = new Mult(idx++);
    auto mult15 = new Mult(idx++);
    auto mult16 = new Mult(idx++);
    auto mult17 = new Multi(idx++, 3456);
    auto mult18 = new Mult(idx++);
    auto mult19 = new Mult(idx++);
    auto mult20 = new Mult(idx++);
    auto mult21 = new Multi(idx++, 87);
    auto mult22 = new Mult(idx++);
    auto mult23 = new Mult(idx++);
    auto mult24 = new Mult(idx++);
    auto mult25 = new Mult(idx++);

    auto sub1 = new Subi(idx++, 432);
    auto sub2 = new Sub(idx++);
    auto sub3 = new Sub(idx++);
    auto sub4 = new Sub(idx++);
    auto sub5 = new Subi(idx++, 20736);
    auto sub6 = new Subi(idx++, 32);
    auto sub7 = new Sub(idx++);
    auto sub8 = new Subi(idx++, 1492992);
    auto sub9 = new Sub(idx++);
    auto sub10 = new Subi(idx++, 72);
    auto sub11 = new Subi(idx++, 864);
    auto sub12 = new Subi(idx++, 186624);

    auto out = new OutputStream(idx++);

    df->connect(in1, reg28, reg28->getPortA());
    df->connect(reg28, reg29, reg29->getPortA());
    df->connect(reg29, reg30, reg30->getPortA());
    df->connect(reg30, reg31, reg31->getPortA());
    df->connect(reg31, reg32, reg32->getPortA());
    df->connect(reg32, reg33, reg33->getPortA());
    df->connect(reg33, reg34, reg34->getPortA());
    df->connect(reg34, reg35, reg35->getPortA());
    df->connect(reg35, reg36, reg36->getPortA());

    df->connect(in2, reg1, reg1->getPortA());
    df->connect(reg1, reg2, reg2->getPortA());
    df->connect(reg2, reg3, reg3->getPortA());
    df->connect(reg3, reg4, reg4->getPortA());
    df->connect(reg4, reg5, reg5->getPortA());
    df->connect(reg5, reg6, reg6->getPortA());

    df->connect(in2, add1, add1->getPortA());
    df->connect(add1, mult1, mult1->getPortA());
    df->connect(mult1, mult2, mult2->getPortA());
    df->connect(reg18, mult2, mult2->getPortB());
    df->connect(mult2, mult3, mult3->getPortA());
    df->connect(reg3, mult3, mult3->getPortB());
    df->connect(mult3, add2, add2->getPortA());
    df->connect(reg39, add2, add2->getPortB());
    df->connect(add2, mult4, mult4->getPortA());
    df->connect(reg5, mult4, mult4->getPortB());
    df->connect(reg14, add3, add3->getPortA());
    df->connect(mult4, add3, add3->getPortB());
    df->connect(add3, reg7, reg7->getPortA());
    df->connect(reg7, reg8, reg8->getPortA());
    df->connect(reg8, reg9, reg9->getPortA());
    df->connect(reg9, sub4, sub4->getPortA());
    //df->connect(mult29, sub4, sub4->getPortB());


    df->connect(in2, mult5, mult5->getPortA());
    df->connect(mult5, add4, add4->getPortA());
    df->connect(mult10, add4, add4->getPortB());
    df->connect(add4, sub1, sub1->getPortA());
    df->connect(sub1, mult6, mult6->getPortA());
    df->connect(mult6, mult7, mult7->getPortA());
    df->connect(reg4, mult7, mult7->getPortB());
    df->connect(mult7, sub2, sub2->getPortA());
    df->connect(reg16, sub2, sub2->getPortB());
    df->connect(sub2, mult8, mult8->getPortA());
    df->connect(reg6, mult8, mult8->getPortB());
    df->connect(mult8, sub3, sub3->getPortA());
    df->connect(reg24, sub3, sub3->getPortB());
    df->connect(mult19, sub9, sub9->getPortA());
    df->connect(sub3, sub9, sub9->getPortB());
    //df->connect(sub9, mult29, mult29->getPortA());
    //df->connect(reg36, mult29, mult29->getPortB());

    df->connect(in3, reg17, reg17->getPortA());
    df->connect(reg17, reg18, reg18->getPortA());
    df->connect(reg18, reg19, reg19->getPortA());
    df->connect(reg19, reg20, reg20->getPortA());

    df->connect(in3, reg37, reg37->getPortA());
    df->connect(reg37, reg38, reg38->getPortA());
    df->connect(reg38, reg39, reg39->getPortA());

    df->connect(in3, mult10, mult10->getPortA());
    df->connect(in3, mult11, mult11->getPortA());
    df->connect(mult11, reg10, reg10->getPortA());
    df->connect(reg10, reg11, reg11->getPortA());
    df->connect(reg11, reg12, reg12->getPortA());
    df->connect(reg12, reg13, reg13->getPortA());
    df->connect(reg13, reg14, reg14->getPortA());

    df->connect(in3, mult12, mult12->getPortA());
    df->connect(mult12, sub5, sub5->getPortA());
    df->connect(sub5, mult13, mult13->getPortA());
    df->connect(reg18, mult13, mult13->getPortB());
    df->connect(mult13, reg15, reg15->getPortA());
    df->connect(reg15, reg16, reg16->getPortA());

    df->connect(in3, sub6, sub6->getPortA());
    df->connect(sub6, mult14, mult14->getPortA());
    df->connect(reg1, mult14, mult14->getPortB());
    df->connect(mult14, add5, add5->getPortA());
    df->connect(mult25, add5, add5->getPortB());
    df->connect(add5, mult15, mult15->getPortA());
    df->connect(reg3, mult15, mult15->getPortB());
    df->connect(mult15, sub7, sub7->getPortA());
    df->connect(reg25, sub7, sub7->getPortB());
    df->connect(sub7, mult16, mult16->getPortA());
    df->connect(mult16, add6, add6->getPortA());
    df->connect(reg27, add6, add6->getPortB());
    df->connect(add6, mult19, mult19->getPortA());
    df->connect(reg34, mult19, mult19->getPortB());

    df->connect(in3, mult17, mult17->getPortA());
    df->connect(mult17, sub8, sub8->getPortA());
    df->connect(sub8, mult18, mult18->getPortA());
    df->connect(reg18, mult18, mult18->getPortB());
    df->connect(mult18, reg21, reg21->getPortB());
    df->connect(reg21, reg22, reg22->getPortB());
    df->connect(reg22, reg23, reg23->getPortB());
    df->connect(reg23, reg24, reg24->getPortB());

    df->connect(in3, sub10, sub10->getPortA());
    df->connect(sub10, mult25, mult25->getPortA());

    df->connect(in3, mult21, mult21->getPortA());
    df->connect(mult21, add7, add7->getPortA());
    df->connect(add7, mult22, mult22->getPortA());
    df->connect(reg18, mult22, mult22->getPortB());
    df->connect(mult22, reg25, reg25->getPortA());

    df->connect(in3, sub11, sub11->getPortA());
    df->connect(sub11, mult23, mult23->getPortA());
    df->connect(reg17, mult23, mult23->getPortB());
    df->connect(mult23, sub12, sub12->getPortA());
    df->connect(sub12, mult24, mult24->getPortA());
    df->connect(reg19, mult24, mult24->getPortB());
    df->connect(mult24, reg26, reg26->getPortA());
    df->connect(reg26, reg27, reg27->getPortA());

    df->connect(sub4, out, out->getPortA());
    return df;
}
