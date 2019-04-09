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
    auto df = new DataFlow(id, "Qspline");
    int idx = 0;
    auto in1 = new InputStream(idx++);
    auto in2 = new InputStream(idx++);
    auto in3 = new InputStream(idx++);
    auto in4 = new InputStream(idx++);
    auto in5 = new InputStream(idx++);
    auto in6 = new InputStream(idx++);
    auto in7 = new InputStream(idx++);

    auto reg1 = new PassB(idx++);
    auto reg2 = new PassB(idx++);
    auto reg3 = new PassB(idx++);

    auto reg4 = new PassB(idx++);
    auto reg5 = new PassB(idx++);
    auto reg6 = new PassB(idx++);
    auto reg7 = new PassB(idx++);

    auto mult1 = new Mult(idx++);
    auto mult2 = new Mult(idx++);
    auto mult3 = new Multi(idx++, 6);
    auto mult4 = new Mult(idx++);
    auto mult5 = new Multi(idx++, 4);
    auto mult6 = new Mult(idx++);
    auto mult7 = new Multi(idx++, 4);

    auto mult8 = new Mult(idx++);
    auto mult9 = new Mult(idx++);
    auto mult10 = new Mult(idx++);
    auto mult11 = new Mult(idx++);
    auto mult12 = new Mult(idx++);

    auto mult13 = new Mult(idx++);
    auto mult14 = new Mult(idx++);
    auto mult15 = new Mult(idx++);
    auto mult16 = new Mult(idx++);
    auto mult17 = new Mult(idx++);

    auto mult18 = new Mult(idx++);
    auto mult19 = new Mult(idx++);
    auto mult20 = new Mult(idx++);
    auto mult21 = new Mult(idx++);

    auto add1 = new PassB(idx++);
    auto mult22 = new Mult(idx++);

    auto add2 = new PassB(idx++);
    auto add3 = new PassB(idx++);
    auto add4 = new PassB(idx++);

    auto out = new OutputStream(idx++);

    df->connect(in1, mult5, mult5->getPortA());
    df->connect(in2, mult1, mult1->getPortA());
    df->connect(in2, mult2, mult2->getPortA());
    df->connect(in2, mult4, mult4->getPortA());
    df->connect(in2, reg1, reg1->getPortA());
    df->connect(reg1, reg2, reg2->getPortA());
    df->connect(reg2, reg3, reg3->getPortA());
    df->connect(in3, mult7, mult7->getPortA());
    df->connect(in4, mult3, mult3->getPortA());
    df->connect(in5, mult1, mult1->getPortB());
    df->connect(in6, mult6, mult6->getPortA());
    df->connect(in6, mult2, mult2->getPortB());
    df->connect(in6, mult4, mult4->getPortB());
    df->connect(in6, reg4, reg4->getPortA());
    df->connect(reg4, reg5, reg5->getPortA());
    df->connect(reg5, reg6, reg6->getPortA());
    df->connect(reg6, reg7, reg7->getPortA());
    df->connect(in7, mult7, mult7->getPortA());

    df->connect(reg1, mult8, mult8->getPortA());
    df->connect(mult1, mult8, mult8->getPortB());
    df->connect(mult3, mult9, mult9->getPortA());
    df->connect(mult4, mult9, mult9->getPortB());
    df->connect(mult4, mult10, mult10->getPortA());
    df->connect(mult5, mult10, mult10->getPortB());
    df->connect(mult6, mult11, mult11->getPortA());
    df->connect(reg4, mult11, mult11->getPortB());
    df->connect(mult7, mult12, mult12->getPortA());
    df->connect(reg4, mult12, mult12->getPortB());

    df->connect(reg2, mult13, mult13->getPortA());
    df->connect(mult8, mult13, mult13->getPortB());
    df->connect(mult10, mult14, mult14->getPortA());
    df->connect(reg2, mult14, mult14->getPortB());
    df->connect(mult2, mult15, mult15->getPortA());
    df->connect(mult9, mult15, mult15->getPortB());
    df->connect(mult11, mult16, mult16->getPortA());
    df->connect(reg5, mult16, mult16->getPortB());
    df->connect(mult12, mult17, mult17->getPortA());
    df->connect(reg5, mult17, mult17->getPortB());

    df->connect(reg3, mult18, mult18->getPortA());
    df->connect(mult13, mult18, mult18->getPortB());
    df->connect(reg3, mult19, mult19->getPortA());
    df->connect(mult14, mult19, mult19->getPortB());
    df->connect(reg7, mult20, mult20->getPortA());
    df->connect(mult16, mult20, mult20->getPortB());
    df->connect(mult17, mult21, mult21->getPortA());
    df->connect(reg3, mult21, mult21->getPortB());

    df->connect(mult15, add1, add1->getPortA());
    df->connect(mult20, add1, add1->getPortB());
    df->connect(mult21, mult22, mult22->getPortA());
    df->connect(reg7, mult22, mult22->getPortB());

    df->connect(mult22, add2, add2->getPortA());
    df->connect(add1, add2, add2->getPortB());
    df->connect(mult18, add3, add3->getPortA());
    df->connect(add2, add3, add3->getPortB());

    df->connect(add3, out, out->getPortA());

    return df;
}


