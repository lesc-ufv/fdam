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

    auto i0_n1 = new InputStream(idx++);
    auto i1_n2 = new InputStream(idx++);
    auto i2_n3 = new InputStream(idx++);

    auto mul_n4 = new Mult(idx++);
    auto mul_n6 = new Mult(idx++);
    auto mul_n7 = new Mult(idx++);
    auto mul_n8 = new Mult(idx++);
    auto mul_n9 = new Mult(idx++);
    auto mul_n10 = new Mult(idx++);
    auto mul_n16 = new Mult(idx++);
    auto mul_n27 = new Mult(idx++);
    auto mul_n30 = new Mult(idx++);
    auto mul_n31 = new Mult(idx++);

    auto sub_n15 = new Sub(idx++);
    auto sub_n19 = new Sub(idx++);
    
    auto add_n17 = new Add(idx++);
    auto add_n18 = new Add(idx++);
    auto add_n20 = new Add(idx++);
    auto add_n24 = new Add(idx++);
    auto add_n32 = new Add(idx++);
    auto add_n35 = new Add(idx++);

    auto add_lmm_72_n5 = new Addi(idx++, 72);
    auto add_lmm_6912_n23 = new Addi(idx++, 6912);
    auto add_lmm_1728_n22 = new Addi(idx++, 1728);
    auto add_lmm_55296_n33 = new Addi(idx++, 55296);
    
    auto mul_lmm_4_n12 = new Multi(idx++, 4);
    auto mul_lmm_432_n21 = new Multi(idx++, 432);
    auto mul_lmm_360_n28 = new Multi(idx++, 360);
    auto mul_lmm_71_n14 = new Multi(idx++, 71);
    auto mul_lmm_6_n11 = new Multi(idx++, 6);
    auto mul_lmm_464_n13 = new Multi(idx++, 464);
    
    auto sub_lmm_13824_n26 = new Subi(idx++, 13824);
    auto sub_lmm_4312_n25 = new Subi(idx++, 4312);
    auto sub_lmm_13824_n34 = new Subi(idx++, 13824);
    auto sub_lmm_13824_n29 = new Subi(idx++, 13824);

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
    auto reg37 = new PassA(idx++);
    auto reg38 = new PassA(idx++);
    auto reg39 = new PassA(idx++);
    auto reg40 = new PassA(idx++);
    auto reg41 = new PassA(idx++);
    auto reg42 = new PassA(idx++);
    auto reg43 = new PassA(idx++);
    auto reg44 = new PassA(idx++);
    auto reg45 = new PassA(idx++);
    auto reg46 = new PassA(idx++);
    auto reg47 = new PassA(idx++);
    auto reg48 = new PassA(idx++);
    auto reg49 = new PassA(idx++);
    auto reg50 = new PassA(idx++);

    auto o0_n36 = new OutputStream(idx++);

    df->connect(i0_n1, reg1, reg1->getPortA());
    df->connect(reg1, reg2, reg2->getPortA());
    df->connect(reg2, reg3, reg3->getPortA());
    df->connect(reg3, reg4, reg4->getPortA());
    df->connect(reg4, reg5, reg5->getPortA());
    df->connect(reg5, mul_n6, mul_n6->getPortA());

    df->connect(i0_n1, reg6, reg6->getPortA());
    df->connect(reg6, reg7, reg7->getPortA());
    df->connect(reg7, reg8, reg8->getPortA());
    df->connect(reg8, reg9, reg9->getPortA());
    df->connect(reg9, reg10, reg10->getPortA());
    df->connect(reg10, reg11, reg11->getPortA());
    df->connect(reg11, reg12, reg12->getPortA());
    df->connect(reg12, mul_n10, mul_n10->getPortA());

    df->connect(i0_n1, reg13, reg13->getPortA());
    df->connect(reg13, reg14, reg14->getPortA());
    df->connect(reg14, reg15, reg15->getPortA());
    df->connect(reg15, reg16, reg16->getPortA());
    df->connect(reg16, reg17, reg17->getPortA());
    df->connect(reg17, reg18, reg18->getPortA());
    df->connect(reg18, reg19, reg19->getPortA());
    df->connect(reg19, reg20, reg20->getPortA());
    df->connect(reg20, reg21, reg21->getPortA());
    df->connect(reg21, mul_n4, mul_n4->getPortA());

    df->connect(mul_n6, add_n18, add_n18->getPortA());
    df->connect(mul_n10, add_n35, add_n35->getPortA());
    df->connect(add_n18, mul_n10, mul_n10->getPortB());
    df->connect(add_n35, mul_n4, mul_n4->getPortB());
    df->connect(mul_n4, add_n17, add_n17->getPortA());
    df->connect(add_n17, o0_n36, o0_n36->getPortA());

    df->connect(i1_n2, reg22, reg22->getPortA());
    df->connect(reg22, reg23, reg23->getPortA());
    df->connect(reg23, reg24, reg24->getPortA());
    df->connect(reg24, reg25, reg25->getPortA());
    df->connect(reg25, mul_n9, mul_n9->getPortA());

    df->connect(mul_n9, add_n32, add_n32->getPortA());
    df->connect(add_n32, add_n18, add_n18->getPortB());

    df->connect(i1_n2, add_lmm_72_n5, add_lmm_72_n5->getPortA());
    df->connect(add_lmm_72_n5, mul_lmm_4_n12, mul_lmm_4_n12->getPortA());
    df->connect(mul_lmm_4_n12, mul_n27, mul_n27->getPortA());
    df->connect(mul_n27, add_n20, add_n20->getPortA());
    df->connect(add_n20, mul_n9, mul_n9->getPortB());

    df->connect(i1_n2, mul_lmm_432_n21, mul_lmm_432_n21->getPortA());
    df->connect(mul_lmm_432_n21, sub_lmm_13824_n26, sub_lmm_13824_n26->getPortA());
    df->connect(sub_lmm_13824_n26, mul_n8, mul_n8->getPortA());

    df->connect(mul_n8, reg28, reg28->getPortA());
    df->connect(reg28, reg29, reg29->getPortA());
    df->connect(reg29, reg30, reg30->getPortA());
    df->connect(reg30, reg31, reg31->getPortA());
    df->connect(reg31, reg32, reg32->getPortA());
    df->connect(reg32, add_n35, add_n35->getPortB());

    df->connect(i1_n2, reg26, reg26->getPortA());
    df->connect(reg26, reg27, reg27->getPortA());
    df->connect(reg27, mul_n7, mul_n7->getPortA());

    df->connect(mul_n7, sub_n15, sub_n15->getPortA());
    df->connect(sub_n15, mul_n6, mul_n6->getPortB());
    
    df->connect(i1_n2, reg33, reg33->getPortA());
    df->connect(reg33, mul_n16, mul_n16->getPortA());

    df->connect(mul_n16, add_n24, add_n24->getPortA());
    df->connect(add_n24, mul_n7, mul_n7->getPortB());

    df->connect(i1_n2, sub_n19, sub_n19->getPortA());
    df->connect(sub_n19, mul_n16, mul_n16->getPortB());

    df->connect(i2_n3, sub_n19, sub_n19->getPortB());
    df->connect(i2_n3, reg35, reg35->getPortA());
    df->connect(reg35, reg36, reg36->getPortA());
    df->connect(reg36, mul_n8, mul_n8->getPortB());

    df->connect(i2_n3, mul_lmm_360_n28, mul_lmm_360_n28->getPortA());

    df->connect(mul_lmm_360_n28, reg34, reg34->getPortA());
    df->connect(reg34, add_lmm_6912_n23, add_lmm_6912_n23->getPortA());

    df->connect(add_lmm_6912_n23, add_n20, add_n20->getPortB());

    df->connect(i2_n3, mul_lmm_71_n14, mul_lmm_71_n14->getPortA());
    df->connect(mul_lmm_71_n14, add_lmm_1728_n22, add_lmm_1728_n22->getPortA());
    df->connect(add_lmm_1728_n22, add_n24, add_n24->getPortB());

    df->connect(i2_n3, mul_lmm_6_n11, mul_lmm_6_n11->getPortA());
    df->connect(mul_lmm_6_n11, sub_lmm_4312_n25, sub_lmm_4312_n25->getPortA());
    df->connect(sub_lmm_4312_n25, mul_n31, mul_n31->getPortA());
    df->connect(mul_n31, add_lmm_55296_n33, add_lmm_55296_n33->getPortA());

    df->connect(add_lmm_55296_n33, reg39, reg39->getPortA());
    df->connect(reg39, add_n32, add_n32->getPortA());

    df->connect(i2_n3, reg37, reg37->getPortA());
    df->connect(reg37, reg38, reg38->getPortA());
    df->connect(reg38, mul_n31, mul_n31->getPortB());

    df->connect(i2_n3, mul_lmm_464_n13, mul_lmm_464_n13->getPortA());
    df->connect(mul_lmm_464_n13, sub_lmm_13824_n29, sub_lmm_13824_n29->getPortA());

    df->connect(sub_lmm_13824_n29, reg40, reg40->getPortA());
    df->connect(reg40, reg41, reg41->getPortA());
    df->connect(reg41, sub_n15, sub_n15->getPortB());

    df->connect(i2_n3, reg42, reg42->getPortA());
    df->connect(reg42, mul_n30, mul_n30->getPortA());

    df->connect(mul_n30, reg43, reg43->getPortA());
    df->connect(reg43, reg44, reg44->getPortA());
    df->connect(reg44, reg45, reg45->getPortA());
    df->connect(reg45, reg46, reg46->getPortA());
    df->connect(reg46, reg47, reg47->getPortA());
    df->connect(reg47, reg48, reg48->getPortA());
    df->connect(reg48, reg49, reg49->getPortA());
    df->connect(reg49, reg50, reg50->getPortA());
    df->connect(reg50, add_n17, add_n17->getPortB());

    df->connect(i2_n3, sub_lmm_13824_n34, sub_lmm_13824_n34->getPortA());
    df->connect(sub_lmm_13824_n34, mul_n30, mul_n30->getPortB());

    return df;
}
