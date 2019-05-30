#include "poly6.h"

int main(int argc, char *argv[]) {

    int idx = 0;
    int test = 0;
     
    if(argc > 1)
        test = atoi(argv[1]);
     
    if(argc > 2)
        idx = atoi(argv[2]);

    if(test & 1)
        poly6(idx);
    if(test & 2)
        poly6_openmp(idx);
    if(test & 4)
        poly6_cgra(idx, 1);
    
    return 0;
}

int poly6(int idx) {

    unsigned short *a, *b, *c, *out;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k;
        b[k] = k;
        c[k] = k;
        out[k] = 0;
    }

    high_resolution_clock::time_point s;
    duration<double> diff = {};

    for (int i = 0; i < 100; i++) {
        s = high_resolution_clock::now();
        for (int k = 0; k < DATA_SIZE; ++k) {
            out[k] = b[k] * (8 * c[k] * (b[k] + 216) + 124416 * c[k]) + 2985984 * c[k] - a[k] * (b[k] * b[k] * c[k] *
                                                                                                 (6 * b[k] + 4 * b[k] -
                                                                                                  432) - c[k] *
                                                                                                         (414 * c[k] -
                                                                                                          20736) -
                                                                                                 c[k] * (3456 * c[k] -
                                                                                                         1492992) -
                                                                                                 a[k] * ((b[k] * b[k] *
                                                                                                          (b[k] *
                                                                                                           (c[k] - 32) +
                                                                                                           c[k] *
                                                                                                           (c[k] -
                                                                                                            72)) -
                                                                                                          c[k] *
                                                                                                          (87 * c[k] +
                                                                                                           2592)) +
                                                                                                         c[k] * (c[k] *
                                                                                                                 (c[k] -
                                                                                                                  864) -
                                                                                                                 186624)));
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / 100;

    printf("Time(ms) CPU 1 Thread: %5.2lf\n", cpuExecTime);

    int v = out[idx];

    delete a;
    delete b;
    delete c;
    delete out;

    return v;
}

int poly6_openmp(int idx) {

    unsigned short *a, *b, *c, *out;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k;
        b[k] = k;
        c[k] = k;
        out[k] = 0;
    }

    omp_set_num_threads(NUM_THREAD);
    high_resolution_clock::time_point s;
    duration<double> diff = {};

    for (int i = 0; i < 100; i++) {
        s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
        for (int k = 0; k < DATA_SIZE; ++k) {
            out[k] = b[k] * (8 * c[k] * (b[k] + 216) + 124416 * c[k]) + 2985984 * c[k] - a[k] * (b[k] * b[k] * c[k] *
                                                                                                 (6 * b[k] + 4 * b[k] -
                                                                                                  432) - c[k] *
                                                                                                         (414 * c[k] -
                                                                                                          20736) -
                                                                                                 c[k] * (3456 * c[k] -
                                                                                                         1492992) -
                                                                                                 a[k] * ((b[k] * b[k] *
                                                                                                          (b[k] *
                                                                                                           (c[k] - 32) +
                                                                                                           c[k] *
                                                                                                           (c[k] -
                                                                                                            72)) -
                                                                                                          c[k] *
                                                                                                          (87 * c[k] +
                                                                                                           2592)) +
                                                                                                         c[k] * (c[k] *
                                                                                                                 (c[k] -
                                                                                                                  864) -
                                                                                                                 186624)));
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / 100;

    printf("Time(ms) CPU %d Thread: %5.2lf\n", NUM_THREAD, cpuExecTime);

    int v = out[idx];

    delete a;
    delete b;
    delete c;
    delete out;

    return v;
}

int poly6_cgra(int idx, int copies) {
    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
    auto cgraHw = new Cgra();
    Scheduler scheduler(cgraArch);
    std::vector<DataFlow *> dfs;
    unsigned short *a, *b, *c, *out;
    int r = 0, v = 0, tries = 0;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k % 1024;
        b[k] = k % 1024;
        c[k] = k % 1024;
        out[k] = 0;
    }

    for (int i = 0; i < NUM_THREAD; ++i) {
        dfs.push_back(createDataFlow(i, copies));
        scheduler.addDataFlow(dfs[i], i, 0);
        cgraArch->getNetBranch(i)->createRouteTable();
        cgraArch->getNet(i)->createRouteTable();
    }
    
    do {
        r = scheduler.scheduling();
        tries++;
    } while (r != SCHEDULE_SUCCESS && tries < 1000);
    
    if (r == SCHEDULE_SUCCESS) {

        cgraHw->loadCgraProgram(cgraArch->getCgraProgram());

        auto data_size = (size_t) (DATA_SIZE / ((NUM_THREAD) * copies));
        auto data_size_bytes = sizeof(unsigned short) * data_size;
        int k = 0;
        for (int i = 0; i < NUM_THREAD; ++i) {
            for (int j = 0; j < copies; ++j) {
                cgraHw->setCgraProgramInputStreamByID(i, (j * 4), &a[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 1, &b[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 2, &c[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramOutputStreamByID(i, (j * 4) + 3, &out[k * data_size], data_size_bytes);
                k++;
            }
        }
        double cgraExecTime = 0;
        for (int i = 0; i < SAMPLES; i++){
            cgraHw->syncExecute(0);
            cgraExecTime += cgraHw->getTimeExec();
        }
        cgraExecTime /= SAMPLES;
        printf("Time(ms) CGRA: %5.2lf\n", cgraExecTime);
        v = out[idx];

    } else {
        printf("Scheduler Error: %d\n", r);
    }

    delete cgraArch;
    delete cgraHw;
    delete[] a;
    delete[] b;
    delete[] c;
    delete[] out;

    for (auto df:dfs) {
        delete df;
    }
    return v;

}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "poly6");
    int idx = 0;

    std::vector<Operator *> i0_n1;
    std::vector<Operator *> i1_n2;
    std::vector<Operator *> i2_n3;
    std::vector<Operator *> o0_n48;

    for (int i = 0; i < copies; ++i) {
        i0_n1.push_back(new InputStream(idx++)); // 0
        i1_n2.push_back(new InputStream(idx++)); // 1
        i2_n3.push_back(new InputStream(idx++)); // 2
        o0_n48.push_back(new OutputStream(idx++));// 3
    }
    for (int i = 0; i < copies; ++i) {
        auto mul_n4 = new Mult(idx++); // 4
        auto mul_n5 = new Mult(idx++); // 5
        auto mul_n6 = new Mult(idx++); // 6
        auto mul_n7 = new Mult(idx++); // 7
        auto mul_n11 = new Mult(idx++); // 8
        auto mul_n12 = new Mult(idx++); // 9
        auto mul_n13 = new Mult(idx++); // 10
        auto mul_n15 = new Mult(idx++); // 11
        auto mul_n16 = new Mult(idx++); // 12
        auto mul_n29 = new Mult(idx++); // 13
        auto mul_n33 = new Mult(idx++); // 14
        auto mul_n35 = new Mult(idx++); // 15
        auto mul_n36 = new Mult(idx++); // 16
        auto mul_n37 = new Mult(idx++); // 17
        auto mul_n38 = new Mult(idx++); // 18
        auto mul_n39 = new Mult(idx++); // 20
        auto mul_n45 = new Mult(idx++); // 21
        auto add_n27 = new Mult(idx++); // 22
        auto add_n30 = new Mult(idx++); // 23
        auto add_n42 = new Mult(idx++); // 24
        auto sub_n21 = new Sub(idx++); // 25
        auto sub_n28 = new Sub(idx++); // 26
        auto sub_n10 = new Sub(idx++); // 27
        auto sub_n8 = new Sub(idx++); // 28
        auto sub_n44 = new Sub(idx++); // 29
        auto add_n25 = new Add(idx++); // 30
        auto add_n47 = new Add(idx++); // 31
        auto add_lmm_216_n14 = new Addi(idx++, 216); // 31
        auto add_lmm_2592_n32 = new Addi(idx++, 2592); // 32
        auto mul_lmm_6_n17 = new Multi(idx++, 6); // 33
        auto mul_lmm_8_n43 = new Multi(idx++, 8); // 34
        auto mul_lmm_87_n19 = new Multi(idx++, 87); // 35
        auto mul_lmm_3456_n24 = new Multi(idx++, 3456); // 36
        auto mul_lmm_414_n18 = new Multi(idx++, 414); // 37
        auto mul_lmm_2985984_n26 = new Multi(idx++, 2985984); // 38
        auto mul_lmm_124416_n34 = new Multi(idx++, 124416); // 39
        auto mul_lmm_4_n9 = new Multi(idx++, 4); // 40
        auto sub_lmm_432_n22 = new Subi(idx++, 432); // 41
        auto sub_lmm_864_n31 = new Subi(idx++, 864); // 42
        auto sub_lmm_186624_n23 = new Subi(idx++, 186624); // 43
        auto sub_lmm_1492992_n20 = new Subi(idx++, 1492992); // 44
        auto sub_lmm_32_n40 = new Subi(idx++, 32); // 45
        auto sub_lmm_20736_n41 = new Subi(idx++, 20736); // 46
        auto sub_lmm_72_n46 = new Subi(idx++, 72); // 47
        auto reg3 = new PassA(idx++); // 48
        auto reg4 = new PassA(idx++); // 49
        auto reg5 = new PassA(idx++); // 50
        auto reg6 = new PassA(idx++); // 51
        auto reg7 = new PassA(idx++); // 52
        auto reg8 = new PassA(idx++); // 53
        auto reg9 = new PassA(idx++); // 54
        auto reg10 = new PassA(idx++); // 55
        auto reg11 = new PassA(idx++); // 56
        auto reg12 = new PassA(idx++); // 57
        auto reg13 = new PassA(idx++); // 58
        auto reg14 = new PassA(idx++); // 59
        auto reg15 = new PassA(idx++); // 60
        auto reg16 = new PassA(idx++); // 61
        auto reg17 = new PassA(idx++); // 62
        auto reg18 = new PassA(idx++); // 63
        auto reg19 = new PassA(idx++); // 64
        auto reg20 = new PassA(idx++); // 65
        auto reg21 = new PassA(idx++); // 66
        auto reg22 = new PassA(idx++); // 67
        auto reg23 = new PassA(idx++); // 68
        auto reg24 = new PassA(idx++); // 69
        auto reg25 = new PassA(idx++); // 70
        auto reg26 = new PassA(idx++); // 71
        auto reg27 = new PassA(idx++); // 72
        auto reg28 = new PassA(idx++); // 73
        auto reg29 = new PassA(idx++); // 74
        auto reg30 = new PassA(idx++); // 75
        auto reg31 = new PassA(idx++); // 76
        auto reg32 = new PassA(idx++); // 77
        auto reg33 = new PassA(idx++); // 78
        auto reg34 = new PassA(idx++); // 79
        auto reg35 = new PassA(idx++); // 80
        auto reg36 = new PassA(idx++); // 81
        auto reg37 = new PassA(idx++); // 82
        auto reg38 = new PassA(idx++); // 83
        auto reg39 = new PassA(idx++); // 83
        auto reg40 = new PassA(idx++); // 84
        auto reg41 = new PassA(idx++); // 85
        auto reg42 = new PassA(idx++); // 86
        auto reg43 = new PassA(idx++); // 87
        auto reg44 = new PassA(idx++); // 88
        auto reg45 = new PassA(idx++); // 89
        auto reg46 = new PassA(idx++); // 90
        auto reg47 = new PassA(idx++); // 91
        auto reg48 = new PassA(idx++); // 92
        auto reg49 = new PassA(idx++); // 93
        auto reg50 = new PassA(idx++); // 94
        auto reg51 = new PassA(idx++); // 95
        auto reg52 = new PassA(idx++); // 96
        auto reg53 = new PassA(idx++); // 97
        auto reg54 = new PassA(idx++); // 98
        auto reg55 = new PassA(idx++); // 99

        df->connect(reg11, mul_n36, mul_n36->getPortA());
        df->connect(mul_n36, add_n27, add_n27->getPortA());
        df->connect(add_n27, reg38, reg38->getPortA());
        df->connect(reg38, reg39, reg39->getPortA());
        df->connect(reg39, reg40, reg40->getPortA());
        df->connect(reg40, reg41, reg41->getPortA());
        df->connect(reg41, reg42, reg42->getPortA());
        df->connect(reg42, sub_n21, sub_n21->getPortA());
        df->connect(sub_n21, o0_n48[i], o0_n48[i]->getPortA());
        df->connect(i1_n2[i], reg7, reg7->getPortA());
        df->connect(reg7, reg8, reg8->getPortA());
        df->connect(reg8, reg9, reg9->getPortA());
        df->connect(reg9, reg10, reg10->getPortA());
        df->connect(reg10, reg11, reg11->getPortA());
        df->connect(reg11, reg16, reg16->getPortA());
        df->connect(reg16, mul_n5, mul_n5->getPortA());
        df->connect(mul_n5, sub_n10, sub_n10->getPortA());
        df->connect(sub_n10, reg43, reg43->getPortA());
        df->connect(reg43, reg44, reg44->getPortA());
        df->connect(reg44, sub_n8, sub_n8->getPortA());
        df->connect(sub_n8, mul_n7, mul_n7->getPortA());
        df->connect(mul_n7, sub_n21, sub_n21->getPortB());
        df->connect(reg10, mul_n35, mul_n35->getPortA());
        df->connect(mul_n35, sub_n28, sub_n28->getPortA());
        df->connect(sub_n28, mul_n5, mul_n5->getPortB());
        df->connect(reg9, mul_n16, mul_n16->getPortA());
        df->connect(mul_n16, add_n25, add_n25->getPortA());
        df->connect(add_n25, mul_n36, mul_n36->getPortB());
        df->connect(i1_n2[i], add_lmm_216_n14, add_lmm_216_n14->getPortA());
        df->connect(add_lmm_216_n14, mul_lmm_8_n43, mul_lmm_8_n43->getPortA());
        df->connect(mul_lmm_8_n43, mul_n12, mul_n12->getPortA());
        df->connect(mul_n12, mul_n16, mul_n16->getPortB());
        df->connect(reg16, reg17, reg17->getPortA());
        df->connect(reg17, mul_n33, mul_n33->getPortA());
        df->connect(mul_n33, add_n42, add_n42->getPortA());
        df->connect(add_n42, mul_n4, mul_n4->getPortA());
        df->connect(mul_n4, sub_n8, sub_n8->getPortB());
        df->connect(i1_n2[i], mul_lmm_6_n17, mul_lmm_6_n17->getPortA());
        df->connect(mul_lmm_6_n17, add_n30, add_n30->getPortA());
        df->connect(add_n30, sub_lmm_432_n22, sub_lmm_432_n22->getPortA());
        df->connect(sub_lmm_432_n22, mul_n39, mul_n39->getPortA());
        df->connect(reg9, mul_n39, mul_n39->getPortB());
        df->connect(mul_n39, mul_n35, mul_n35->getPortB());
        df->connect(reg11, mul_n15, mul_n15->getPortA());
        df->connect(mul_n15, sub_n44, sub_n44->getPortA());
        df->connect(sub_n44, mul_n33, mul_n33->getPortB());
        df->connect(reg7, mul_n29, mul_n29->getPortA());
        df->connect(mul_n29, reg20, reg20->getPortA());
        df->connect(reg20, reg21, reg21->getPortA());
        df->connect(reg21, add_n47, add_n47->getPortA());
        df->connect(reg14, mul_n12, mul_n12->getPortB());
        df->connect(i2_n3[i], mul_lmm_124416_n34, mul_lmm_124416_n34->getPortA());
        df->connect(mul_lmm_124416_n34, reg24, reg24->getPortA());
        df->connect(reg24, reg25, reg25->getPortA());
        df->connect(reg25, reg26, reg26->getPortA());
        df->connect(reg26, add_n25, add_n25->getPortB());
        df->connect(i2_n3[i], mul_lmm_4_n9, mul_lmm_4_n9->getPortA());
        df->connect(mul_lmm_4_n9, add_n30, add_n30->getPortB());
        df->connect(i2_n3[i], mul_lmm_2985984_n26, mul_lmm_2985984_n26->getPortA());
        df->connect(mul_lmm_2985984_n26, reg27, reg27->getPortA());
        df->connect(reg27, reg28, reg28->getPortA());
        df->connect(reg28, reg29, reg29->getPortA());
        df->connect(reg29, reg30, reg30->getPortA());
        df->connect(reg30, reg31, reg31->getPortA());
        df->connect(reg31, add_n27, add_n27->getPortB());
        df->connect(reg14, mul_n11, mul_n11->getPortA());
        df->connect(mul_n11, reg22, reg22->getPortA());
        df->connect(reg22, reg23, reg23->getPortA());
        df->connect(reg23, sub_n28, sub_n28->getPortB());
        df->connect(i2_n3[i], mul_lmm_414_n18, mul_lmm_414_n18->getPortA());
        df->connect(mul_lmm_414_n18, sub_lmm_20736_n41, sub_lmm_20736_n41->getPortA());
        df->connect(sub_lmm_20736_n41, mul_n11, mul_n11->getPortB());
        df->connect(i2_n3[i], sub_lmm_32_n40, sub_lmm_32_n40->getPortA());
        df->connect(sub_lmm_32_n40, mul_n29, mul_n29->getPortB());
        df->connect(reg14, mul_n45, mul_n45->getPortA());
        df->connect(mul_n45, reg3, reg3->getPortA());
        df->connect(reg3, reg4, reg4->getPortA());
        df->connect(reg4, reg5, reg5->getPortA());
        df->connect(reg5, reg6, reg6->getPortA());
        df->connect(reg6, sub_n10, sub_n10->getPortB());
        df->connect(i2_n3[i], mul_lmm_3456_n24, mul_lmm_3456_n24->getPortA());
        df->connect(mul_lmm_3456_n24, sub_lmm_1492992_n20, sub_lmm_1492992_n20->getPortA());
        df->connect(sub_lmm_1492992_n20, mul_n45, mul_n45->getPortB());
        df->connect(i2_n3[i], sub_lmm_72_n46, sub_lmm_72_n46->getPortA());
        df->connect(sub_lmm_72_n46, reg18, reg18->getPortA());
        df->connect(reg18, reg19, reg19->getPortA());
        df->connect(reg19, mul_n37, mul_n37->getPortA());
        df->connect(mul_n37, add_n47, add_n47->getPortB());
        df->connect(add_n47, mul_n15, mul_n15->getPortB());
        df->connect(i2_n3[i], reg13, reg13->getPortA());
        df->connect(reg13, reg14, reg14->getPortA());
        df->connect(reg14, reg15, reg15->getPortA());
        df->connect(reg15, mul_n13, mul_n13->getPortA());
        df->connect(reg15, mul_n37, mul_n37->getPortB());
        df->connect(reg15, mul_n38, mul_n38->getPortA());
        df->connect(mul_n13, reg36, reg36->getPortA());
        df->connect(reg36, reg37, reg37->getPortA());
        df->connect(reg37, sub_n44, sub_n44->getPortB());
        df->connect(i2_n3[i], mul_lmm_87_n19, mul_lmm_87_n19->getPortA());
        df->connect(mul_lmm_87_n19, add_lmm_2592_n32, add_lmm_2592_n32->getPortA());
        df->connect(add_lmm_2592_n32, reg12, reg12->getPortA());
        df->connect(reg12, mul_n13, mul_n13->getPortB());
        df->connect(mul_n38, reg32, reg32->getPortA());
        df->connect(reg32, reg33, reg33->getPortA());
        df->connect(reg33, reg34, reg34->getPortA());
        df->connect(reg34, reg35, reg35->getPortA());
        df->connect(reg35, add_n42, add_n42->getPortB());
        df->connect(reg13, mul_n6, mul_n6->getPortA());
        df->connect(mul_n6, sub_lmm_186624_n23, sub_lmm_186624_n23->getPortA());
        df->connect(sub_lmm_186624_n23, mul_n38, mul_n38->getPortB());
        df->connect(i2_n3[i], sub_lmm_864_n31, sub_lmm_864_n31->getPortA());
        df->connect(sub_lmm_864_n31, mul_n6, mul_n6->getPortB());
        df->connect(reg53, mul_n4, mul_n4->getPortB());
        df->connect(i0_n1[i], reg45, reg45->getPortA());
        df->connect(reg45, reg46, reg46->getPortA());
        df->connect(reg46, reg47, reg47->getPortA());
        df->connect(reg47, reg48, reg48->getPortA());
        df->connect(reg48, reg49, reg49->getPortA());
        df->connect(reg49, reg50, reg50->getPortA());
        df->connect(reg50, reg51, reg51->getPortA());
        df->connect(reg51, reg52, reg52->getPortA());
        df->connect(reg52, reg53, reg53->getPortA());
        df->connect(reg53, reg54, reg54->getPortA());
        df->connect(reg54, reg55, reg55->getPortA());
        df->connect(reg55, mul_n7, mul_n7->getPortB());
    }

    return df;
}
