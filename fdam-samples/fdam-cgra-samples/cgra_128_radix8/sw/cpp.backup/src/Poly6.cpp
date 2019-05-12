//
// Created by lucas on 11/04/19.
//

#include <Poly6.h>

Poly6::Poly6(Cgra *cgra, CgraArch *cgraArch) {
    Poly6::cgraHw = cgra;
    Poly6::cgraArch = cgraArch;
    Poly6::schedulingTime = 0;
    Poly6::cpuExecTime = 0;
    Poly6::cgraExecTime = 0;
    Poly6::cgraConfTime = 0;
    for (int i = 0; i < 17; ++i) {
        Poly6::constants.push_back(static_cast<unsigned short &&>(random() % 256 + 1));
    }
}

Poly6::~Poly6() = default;

DataFlow *Poly6::createDataFlow(int id, int copies) {
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
        auto add_lmm_216_n14 = new Addi(idx++, Poly6::constants[0]); // 31
        auto add_lmm_2592_n32 = new Addi(idx++, Poly6::constants[1]); // 32
        auto mul_lmm_6_n17 = new Multi(idx++, Poly6::constants[2]); // 33
        auto mul_lmm_8_n43 = new Multi(idx++, Poly6::constants[3]); // 34
        auto mul_lmm_87_n19 = new Multi(idx++, Poly6::constants[4]); // 35
        auto mul_lmm_3456_n24 = new Multi(idx++, Poly6::constants[5]); // 36
        auto mul_lmm_414_n18 = new Multi(idx++, Poly6::constants[6]); // 37
        auto mul_lmm_2985984_n26 = new Multi(idx++, Poly6::constants[7]); // 38
        auto mul_lmm_124416_n34 = new Multi(idx++, Poly6::constants[8]); // 39
        auto mul_lmm_4_n9 = new Multi(idx++, Poly6::constants[9]); // 40
        auto sub_lmm_432_n22 = new Subi(idx++, Poly6::constants[10]); // 41
        auto sub_lmm_864_n31 = new Subi(idx++, Poly6::constants[11]); // 42
        auto sub_lmm_186624_n23 = new Subi(idx++, Poly6::constants[12]); // 43
        auto sub_lmm_1492992_n20 = new Subi(idx++, Poly6::constants[13]); // 44
        auto sub_lmm_32_n40 = new Subi(idx++, Poly6::constants[14]); // 45
        auto sub_lmm_20736_n41 = new Subi(idx++, Poly6::constants[15]); // 46
        auto sub_lmm_72_n46 = new Subi(idx++, Poly6::constants[16]); // 47
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

void Poly6::runCGRA(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};

    Poly6::cgraHw->loadCgraProgram("../cgra_bitstreams/poly6.cgra");
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < copies; ++j) {
            Poly6::cgraHw->setCgraProgramInputStreamByID(i, j * 4, data_in[i][j][0], sizeof(short) * data_size);
            Poly6::cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 1, data_in[i][j][1], sizeof(short) * data_size);
            Poly6::cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 2, data_in[i][j][2], sizeof(short) * data_size);
            Poly6::cgraHw->setCgraProgramOutputStreamByID(i, (j * 4) + 3, data_out[i][j], sizeof(short) * data_size);
        }
    }
    s = high_resolution_clock::now();
    Poly6::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    Poly6::cgraExecTime = diff.count() * 1000;
}

unsigned short Poly6::pot(unsigned short num, unsigned short exp) {
    unsigned short i = 0, res = 1;
    for (i = 0; i < exp; i++)
        res = res * num;
    return res;
}

void Poly6::runCPU(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int j = 0; j < NUM_THREADS; j++) {
        for (int k = 0; k < copies; ++k) {
            for (int i = 0; i < data_size; ++i) {
                unsigned short A, B, C, n33, n4, n27, n8;
                A = data_in[j][k][0][i];
                B = data_in[j][k][1][i];
                C = data_in[j][k][2][i];
                n33 = (C - Poly6::constants[14]) * pot(B, 3) + pot(C, 2) * pot(B, 2) -
                      Poly6::constants[16] * C * pot(B, 2) - Poly6::constants[1] * C * B +
                      Poly6::constants[4] * pot(C, 2) * B;
                n4 = (n33 + pot(C, 3) - Poly6::constants[11] * pot(C, 2) - Poly6::constants[12] * C) * A;
                n27 = Poly6::constants[3] * C * Poly6::constants[0] * pot(B, 2) + Poly6::constants[3] * C * pot(B, 3) +
                      Poly6::constants[8] * C * B + Poly6::constants[7] * C;
                n8 = (Poly6::constants[2] * B + Poly6::constants[9] * C - Poly6::constants[10]) * C * pot(B, 2) -
                     Poly6::constants[6] * pot(B, 3) - Poly6::constants[15] * C * B -
                     C * (Poly6::constants[5] * B - Poly6::constants[13]) - n4;
                data_out[j][k][i] = n27 - A * n8;
            }
        }
        diff = high_resolution_clock::now() - s;
        Poly6::cpuExecTime = diff.count() * 1000;
    }
}

void Poly6::benchmarking(int numThreads, int data_size) {
    int copies = 1;
    unsigned short ****data_in;
    unsigned short ***data_out_cpu;
    unsigned short ***data_out_cgra;

    data_in = new unsigned short ***[numThreads];
    data_out_cpu = new unsigned short **[numThreads];
    data_out_cgra = new unsigned short **[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        data_in[i] = new unsigned short **[copies];
        data_out_cpu[i] = new unsigned short *[copies];
        data_out_cgra[i] = new unsigned short *[copies];
        for (int j = 0; j < copies; ++j) {
            data_in[i][j] = new unsigned short *[3];
            data_in[i][j][0] = new unsigned short[data_size];
            data_in[i][j][1] = new unsigned short[data_size];
            data_in[i][j][2] = new unsigned short[data_size];
            data_out_cpu[i][j] = new unsigned short[data_size];
            data_out_cgra[i][j] = new unsigned short[data_size];
        }
    }

    for (int t = 0; t < numThreads; ++t) {
        for (int c = 0; c < copies; ++c) {
            for (int i = 0; i < data_size; ++i) {
                data_in[t][c][0][i] = 1;//(unsigned short) (random() % 256);
                data_in[t][c][1][i] = 1;//(unsigned short) (random() % 256);
                data_in[t][c][2][i] = 1;//(unsigned short) (random() % 256);

                data_out_cpu[t][c][i] = 0;
                data_out_cgra[t][c][i] = 0;
            }
        }
    }

    if (Poly6::compile(numThreads, copies)) {
        Poly6::runCGRA(data_in, data_out_cgra, data_size, numThreads, copies);
        Poly6::runCPU(data_in, data_out_cpu, data_size, numThreads, copies);
//        for (int t = 0; t < numThreads; ++t) {
//            for (int c = 0; c < copies; ++c) {
//                for (int i = 0; i < data_size; ++i) {
//                    if (data_out_cpu[t][c][i] != data_out_cgra[t][c][i]) {
//                        printf("Error: Thread %d, copy %d, index %d, expected %d found %d!\n", t, c, i,
//                               data_out_cpu[t][c][i],
//                               data_out_cgra[t][c][i]);
//                        break;
//                    }
//                }
//            }
//        }
    } else {
        printf("Compilation failed!\n");
    }
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < copies; ++j) {
            delete data_in[i][j][0];
            delete data_in[i][j][1];
            delete data_in[i][j][2];
            delete data_in[i][j];
            delete data_out_cpu[i][j];
            delete data_out_cgra[i][j];
        }
        delete data_in[i];
        delete data_out_cpu[i];
        delete data_out_cgra[i];
    }

    delete data_in;
    delete data_out_cpu;
    delete data_out_cgra;
}

bool Poly6::compile(int numThreads, int copies) {
    Scheduler scheduler(Poly6::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;
    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(Poly6::createDataFlow(i, copies));
        scheduler.addDataFlow(dfs[i], i, 0);
        Poly6::cgraArch->getNetBranch(i)->createRouteTable();
        Poly6::cgraArch->getNet(i)->createRouteTable();
    }

    int r = 0;
    for (int j = 0; j < 100; ++j) {
        s = high_resolution_clock::now();
        r = scheduler.scheduling();
        diff = high_resolution_clock::now() - s;
        if(r == SCHEDULE_SUCCESS )
            break;
    }

    Poly6::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../cgra_bitstreams/%s.cgra", dfs[0]->getName().c_str());
        Poly6::cgraArch->writeCgraProgram(filename);
    } else {
        printf("Error on scheduling: Code %d\n", r);
    }
    for (auto df:dfs) {
        delete df;
    }
    return r == SCHEDULE_SUCCESS;
}

void Poly6::printStatistics() {
    int copies = 1;
    auto df = Poly6::createDataFlow(0, copies);
    MSG("INFO Poly6 Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << Poly6::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << Poly6::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << Poly6::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << Poly6::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << Poly6::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << Poly6::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << Poly6::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << Poly6::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << Poly6::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Num total edges: " << df->getNumEdges());
    MSG("INFO Scheduling time: " << Poly6::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Poly6::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Poly6::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Poly6::cpuExecTime << "ms");
    Poly6::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Poly6::cgraHw->getAccManagement()->printHwInfo();

    delete df;
}