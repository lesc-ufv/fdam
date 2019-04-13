//
// Created by lucas on 08/04/19.
//

#include <Qspline.h>

Qspline::Qspline(Cgra *cgra, CgraArch *cgraArch) {
    Qspline::cgraHw = cgra;
    Qspline::cgraArch = cgraArch;
    Qspline::schedulingTime = 0;
    Qspline::cpuExecTime = 0;
    Qspline::cgraExecTime = 0;
    Qspline::cgraConfTime = 0;
}

Qspline::~Qspline() = default;

DataFlow *Qspline::createDataFlow(int id) {
    auto df = new DataFlow(id, "Qslpine");
    int idx = 0;
    // level 1
    auto i0_n1 = new InputStream(idx++);
    auto i1_n2 = new InputStream(idx++);
    auto i2_n3 = new InputStream(idx++);
    auto i3_n4 = new InputStream(idx++);
    auto i4_n5 = new InputStream(idx++);
    auto i5_n6 = new InputStream(idx++);
    auto i6_n7 = new InputStream(idx++);

    auto i1_n2_1_0 = new PassB(idx++);
    auto i1_n2_2_0 = new PassB(idx++);

    auto reg1 = new PassA(idx++);
    auto reg2 = new PassA(idx++);
    auto reg3 = new PassA(idx++);
    auto reg4 = new PassA(idx++);
    auto reg5 = new PassA(idx++);
    auto reg6 = new PassA(idx++);
    auto reg7 = new PassA(idx++);
    auto reg8 = new PassA(idx++);
    auto reg9 = new PassA(idx++);
    auto reg10 = new PassA(idx++);
    auto reg11 = new PassA(idx++);
    auto reg12 = new PassA(idx++);
    auto reg13 = new PassA(idx++);
    auto reg14 = new PassA(idx++);
    auto reg15 = new PassA(idx++);
    auto reg16 = new PassA(idx++);
    auto reg17 = new PassA(idx++);
    auto reg18 = new PassA(idx++);
    auto reg19 = new PassA(idx++);
    auto reg20 = new PassA(idx++);
    auto reg21 = new PassA(idx++);
    auto reg22 = new PassA(idx++);
    auto reg23 = new PassA(idx++);
    auto reg24 = new PassA(idx++);
    auto reg25 = new PassA(idx++);
    auto reg26 = new PassA(idx++);
    auto reg27 = new PassA(idx++);
    auto reg28 = new PassA(idx++);
    auto reg29 = new PassA(idx++);
    auto reg30 = new PassA(idx++);
    auto reg31 = new PassA(idx++);
    auto reg32 = new PassA(idx++);
    auto reg33 = new PassA(idx++);
    auto reg34 = new PassA(idx++);
    auto reg35 = new PassA(idx++);
    auto reg36 = new PassA(idx++);

    auto mul_n8 = new Mult(idx++);
    auto mul_n9 = new Mult(idx++);
    auto mul_n11 = new Mult(idx++);
    auto mul_n13 = new Mult(idx++);
    auto mul_n29 = new Mult(idx++);
    auto mul_n25 = new Mult(idx++);
    auto mul_n23 = new Mult(idx++);
    auto mul_n24 = new Mult(idx++);
    auto mul_n18 = new Mult(idx++);
    auto mul_n10 = new Mult(idx++);
    auto mul_n28 = new Mult(idx++);
    auto mul_n20 = new Mult(idx++);
    auto mul_n21 = new Mult(idx++);
    auto mul_n14 = new Mult(idx++);
    auto mul_n15 = new Mult(idx++);
    auto mul_n12 = new Mult(idx++);
    auto mul_n26 = new Mult(idx++);
    auto mul_n19 = new Mult(idx++);
    auto mul_n17 = new Mult(idx++);
    auto add_n30 = new Add(idx++);
    auto add_n31 = new Add(idx++);
    auto add_n32 = new Add(idx++);
    auto add_n37 = new Add(idx++);
    auto add_n33 = new Add(idx++);

    auto mul_lmm_6_n27 = new Multi(idx++, 6);
    auto mul_lmm_4_n22 = new Multi(idx++, 4);
    auto mul_lmm_4_n16 = new Multi(idx++, 4);

    auto o0_n34 = new OutputStream(idx++);
    
    df->connect(i4_n5, mul_n13, mul_n13->getPortA());
    df->connect(i1_n2, mul_n13, mul_n13->getPortB());

    df->connect(i1_n2, reg1, reg1->getPortA());
    df->connect(reg1, reg2, reg2->getPortA());
    df->connect(reg2, reg3, reg3->getPortA());
    df->connect(reg3, mul_n8, mul_n8->getPortA());

    df->connect(i1_n2, reg4, reg4->getPortA());
    df->connect(reg4, mul_n11, mul_n11->getPortA());

    df->connect(i1_n2, reg5, reg5->getPortA());
    df->connect(reg5, reg6, reg6->getPortA());
    df->connect(reg6, mul_n29, mul_n29->getPortA());

    df->connect(i1_n2, reg7, reg7->getPortA());
    df->connect(reg7, reg8, reg8->getPortA());
    df->connect(reg8, reg9, reg9->getPortA());
    df->connect(reg9, mul_n25, mul_n25->getPortA());

    df->connect(i1_n2, reg10, reg10->getPortA());
    df->connect(reg10, reg11, reg11->getPortA());
    df->connect(reg11, reg12, reg12->getPortA());
    df->connect(reg12, mul_n20, mul_n20->getPortA());

    df->connect(i1_n2, reg13, reg13->getPortA());
    df->connect(reg13, reg14, reg14->getPortA());
    df->connect(reg14, mul_n24, mul_n24->getPortA());

    df->connect(i1_n2, mul_n18, mul_n18->getPortA());
    
    df->connect(i1_n2, reg15, reg15->getPortA());
    df->connect(reg15, mul_n10, mul_n11->getPortB());

    df->connect(mul_n13, mul_n11, mul_n11->getPortA());
    
    df->connect(mul_n8, reg16, reg16->getPortA());
    df->connect(reg16, reg35, reg35->getPortA());
    df->connect(reg35, reg36, reg36->getPortA());
    df->connect(reg36, add_n30, add_n30->getPortA());

    df->connect(mul_n29, mul_n8, mul_n8->getPortB());
    
    df->connect(mul_n25, reg17, reg17->getPortA());
    df->connect(reg17, reg18, reg18->getPortA());
    df->connect(reg18, add_n33, add_n33->getPortA());

    df->connect(mul_n24, mul_n25, mul_n25->getPortB());
    df->connect(mul_n10, mul_n28, mul_n28->getPortA());
    df->connect(mul_n11, mul_n29, mul_n29->getPortB());
    df->connect(add_n30, o0_n34, o0_n34->getPortA());
    df->connect(add_n33, add_n30, add_n30->getPortA());
    df->connect(mul_n28, mul_n9, mul_n9->getPortA());

    df->connect(mul_n9, reg20, reg20->getPortA());
    df->connect(reg20, add_n31, add_n31->getPortA());

    df->connect(add_n31, add_n32, add_n32->getPortA());
    df->connect(add_n32, add_n33, add_n33->getPortA());
    
    df->connect(mul_n18, reg19, reg19->getPortA());
    df->connect(reg19, mul_n9, mul_n9->getPortB());

    df->connect(mul_n20, mul_n21, mul_n21->getPortA());
    df->connect(mul_n21, add_n32, add_n32->getPortA());
    df->connect(i3_n4, mul_lmm_6_n27, mul_lmm_6_n27->getPortA());
    df->connect(mul_lmm_6_n27, mul_n28, mul_n28->getPortB());

    df->connect(i0_n1, mul_lmm_4_n22, mul_lmm_4_n22->getPortA());
    df->connect(mul_lmm_4_n22, mul_n23, mul_n23->getPortA());
    df->connect(mul_n23, mul_n24, mul_n24->getPortA());

    df->connect(i6_n7, mul_n14, mul_n14->getPortB());
    df->connect(mul_n14, mul_n15, mul_n15->getPortA());
    df->connect(mul_n15, mul_n12, mul_n12->getPortA());
    df->connect(mul_n12, mul_n26, mul_n26->getPortA());

    df->connect(mul_n26, add_n31, add_n31->getPortA());

    df->connect(i5_n6, mul_n18, mul_n18->getPortB());
    df->connect(i5_n6, mul_n10, mul_n10->getPortA());
    df->connect(i5_n6, mul_n14, mul_n14->getPortB());

    df->connect(i5_n6, reg21, reg21->getPortA());
    df->connect(reg21, mul_n15, mul_n15->getPortB());

    df->connect(i5_n6, reg22, reg22->getPortA());
    df->connect(reg22, reg23, reg23->getPortA());
    df->connect(reg23, mul_n12, mul_n12->getPortB());

    df->connect(i5_n6, reg24, reg24->getPortA());
    df->connect(reg24, reg25, reg25->getPortA());
    df->connect(reg25, reg27, reg27->getPortA());
    df->connect(reg27, mul_n26, mul_n26->getPortB());

    df->connect(i5_n6, reg28, reg28->getPortA());
    df->connect(reg28, reg29, reg29->getPortA());
    df->connect(reg29, reg30, reg30->getPortA());
    df->connect(reg30, reg31, reg31->getPortA());
    df->connect(reg31, mul_n21, mul_n21->getPortB());

    df->connect(i5_n6, reg32, reg32->getPortA());
    df->connect(reg32, reg33, reg33->getPortA());
    df->connect(reg33, mul_n19, mul_n19->getPortA());

    df->connect(i5_n6, reg34, reg34->getPortA());
    df->connect(reg34, mul_n17, mul_n17->getPortA());

    df->connect(mul_n19, mul_n20, mul_n20->getPortB());
    df->connect(mul_n17, mul_n19, mul_n19->getPortB());

    df->connect(i2_n3, mul_lmm_4_n16, mul_lmm_4_n16->getPortA());
    df->connect(mul_lmm_4_n16, mul_n17, mul_n17->getPortB());

    df->connect(mul_n10, mul_n23, mul_n23->getPortB());

    return df;
}


