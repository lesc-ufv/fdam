//
// Created by lucas on 11/04/19.
//

#include <Poly8.h>

Poly8::Poly8(Cgra *cgra, CgraArch *cgraArch) {
    Poly8::cgraHw = cgra;
    Poly8::cgraArch = cgraArch;
    Poly8::schedulingTime = 0;
    Poly8::cpuExecTime = 0;
    Poly8::cgraExecTime = 0;
    Poly8::cgraConfTime = 0;
    for (int i = 0; i < 12; ++i) {
        //Poly5::constants.push_back(static_cast<unsigned short &&>(random() % 256 + 1));
        Poly8::constants.push_back(1);
    }
}

Poly8::~Poly8() = default;

DataFlow *Poly8::createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "Poly8");
    int idx = 0;

    std::vector<Operator *> i0_n1;
    std::vector<Operator *> i1_n2;
    std::vector<Operator *> i2_n3;
    std::vector<Operator *> o0_n36;

    for (int i = 0; i < copies; ++i) {
        i0_n1.push_back(new InputStream(idx++));//0
        i1_n2.push_back(new InputStream(idx++));//1
        i2_n3.push_back(new InputStream(idx++));//2
        o0_n36.push_back(new OutputStream(idx++));//3
    }
    for (int i = 0; i < copies; ++i) {

        auto mul_n4 = new Mult(idx++);//4
        auto mul_n6 = new Mult(idx++);//5
        auto mul_n7 = new Mult(idx++);//6
        auto mul_n8 = new Mult(idx++);//7
        auto mul_n9 = new Mult(idx++);//8
        auto mul_n10 = new Mult(idx++);//9
        auto mul_n16 = new Mult(idx++);//10
        auto mul_n27 = new Mult(idx++);//11
        auto mul_n30 = new Mult(idx++);//12
        auto mul_n31 = new Mult(idx++);//13
        auto sub_n15 = new Sub(idx++);//14
        auto sub_n19 = new Sub(idx++);//15
        auto add_n17 = new Add(idx++);//16
        auto add_n18 = new Add(idx++);//17
        auto add_n20 = new Add(idx++);//18
        auto add_n24 = new Add(idx++);//19
        auto add_n32 = new Add(idx++);//20
        auto add_n35 = new Add(idx++);//21
        auto add_lmm_72_n5 = new Addi(idx++, Poly8::constants[0]);//22
        auto add_lmm_6912_n23 = new Addi(idx++, Poly8::constants[1]);//23
        auto add_lmm_1728_n22 = new Addi(idx++, Poly8::constants[2]);//24
        auto add_lmm_55296_n33 = new Addi(idx++, Poly8::constants[3]);//25
        auto mul_lmm_4_n12 = new Multi(idx++, Poly8::constants[4]);//26
        auto mul_lmm_432_n21 = new Multi(idx++, Poly8::constants[5]);//27
        auto mul_lmm_360_n28 = new Multi(idx++, Poly8::constants[6]);//28
        auto mul_lmm_71_n14 = new Multi(idx++, Poly8::constants[7]);//29
        auto mul_lmm_6_n11 = new Multi(idx++, Poly8::constants[8]);//30
        auto mul_lmm_464_n13 = new Multi(idx++, Poly8::constants[9]);//31
        auto sub_lmm_13824_n26 = new Subi(idx++, Poly8::constants[10]);//32
        auto sub_lmm_13824_n34 = new Subi(idx++, Poly8::constants[10]);//33
        auto sub_lmm_13824_n29 = new Subi(idx++, Poly8::constants[10]);//34
        auto sub_lmm_4312_n25 = new Subi(idx++, Poly8::constants[11]);//35
        auto reg13 = new PassA(idx++);//36
        auto reg14 = new PassA(idx++);//37
        auto reg15 = new PassA(idx++);//38
        auto reg16 = new PassA(idx++);//39
        auto reg17 = new PassA(idx++);//40
        auto reg18 = new PassA(idx++);//41
        auto reg19 = new PassA(idx++);//42
        auto reg20 = new PassA(idx++);//43
        auto reg21 = new PassA(idx++);//44
        auto reg22 = new PassA(idx++);//45
        auto reg23 = new PassA(idx++);//46
        auto reg24 = new PassA(idx++);//47
        auto reg25 = new PassA(idx++);//48
        auto reg28 = new PassA(idx++);//49
        auto reg29 = new PassA(idx++);//50
        auto reg30 = new PassA(idx++);//51
        auto reg31 = new PassA(idx++);//52
        auto reg32 = new PassA(idx++);//53
        auto reg34 = new PassA(idx++);//54
        auto reg35 = new PassA(idx++);//55
        auto reg36 = new PassA(idx++);//56
        auto reg39 = new PassA(idx++);//57
        auto reg40 = new PassA(idx++);//58
        auto reg41 = new PassA(idx++);//59
        auto reg43 = new PassA(idx++);//60
        auto reg44 = new PassA(idx++);//61
        auto reg45 = new PassA(idx++);//62
        auto reg46 = new PassA(idx++);//63
        auto reg47 = new PassA(idx++);//64
        auto reg48 = new PassA(idx++);//65
        auto reg49 = new PassA(idx++);//66
        auto reg50 = new PassA(idx++);//67

        df->connect(reg17, mul_n6, mul_n6->getPortA());
        df->connect(reg19, mul_n10, mul_n10->getPortA());
        df->connect(i0_n1[i], reg13, reg13->getPortA());
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
        df->connect(add_n17, o0_n36[i], o0_n36[i]->getPortA());
        df->connect(i1_n2[i], reg22, reg22->getPortA());
        df->connect(reg22, reg23, reg23->getPortA());
        df->connect(reg23, reg24, reg24->getPortA());
        df->connect(reg24, reg25, reg25->getPortA());
        df->connect(reg25, mul_n9, mul_n9->getPortA());
        df->connect(mul_n9, add_n32, add_n32->getPortA());
        df->connect(add_n32, add_n18, add_n18->getPortB());
        df->connect(i1_n2[i], add_lmm_72_n5, add_lmm_72_n5->getPortA());
        df->connect(add_lmm_72_n5, mul_lmm_4_n12, mul_lmm_4_n12->getPortA());
        df->connect(mul_lmm_4_n12, mul_n27, mul_n27->getPortA());
        df->connect(mul_n27, add_n20, add_n20->getPortA());
        df->connect(add_n20, mul_n9, mul_n9->getPortB());
        df->connect(i1_n2[i], mul_lmm_432_n21, mul_lmm_432_n21->getPortA());
        df->connect(mul_lmm_432_n21, sub_lmm_13824_n26, sub_lmm_13824_n26->getPortA());
        df->connect(sub_lmm_13824_n26, mul_n8, mul_n8->getPortA());
        df->connect(mul_n8, reg28, reg28->getPortA());
        df->connect(reg28, reg29, reg29->getPortA());
        df->connect(reg29, reg30, reg30->getPortA());
        df->connect(reg30, reg31, reg31->getPortA());
        df->connect(reg31, reg32, reg32->getPortA());
        df->connect(reg32, add_n35, add_n35->getPortB());
        df->connect(reg24, mul_n7, mul_n7->getPortA());
        df->connect(mul_n7, sub_n15, sub_n15->getPortA());
        df->connect(sub_n15, mul_n6, mul_n6->getPortB());
        df->connect(reg22, mul_n16, mul_n16->getPortA());
        df->connect(mul_n16, add_n24, add_n24->getPortA());
        df->connect(add_n24, mul_n7, mul_n7->getPortB());
        df->connect(i1_n2[i], sub_n19, sub_n19->getPortA());
        df->connect(sub_n19, mul_n16, mul_n16->getPortB());
        df->connect(i2_n3[i], sub_n19, sub_n19->getPortB());
        df->connect(i2_n3[i], reg35, reg35->getPortA());
        df->connect(reg35, reg36, reg36->getPortA());
        df->connect(reg36, mul_n8, mul_n8->getPortB());
        df->connect(i2_n3[i], mul_lmm_360_n28, mul_lmm_360_n28->getPortA());
        df->connect(mul_lmm_360_n28, reg34, reg34->getPortA());
        df->connect(reg34, add_lmm_6912_n23, add_lmm_6912_n23->getPortA());
        df->connect(add_lmm_6912_n23, add_n20, add_n20->getPortB());
        df->connect(i2_n3[i], mul_lmm_71_n14, mul_lmm_71_n14->getPortA());
        df->connect(mul_lmm_71_n14, add_lmm_1728_n22, add_lmm_1728_n22->getPortA());
        df->connect(add_lmm_1728_n22, add_n24, add_n24->getPortB());
        df->connect(i2_n3[i], mul_lmm_6_n11, mul_lmm_6_n11->getPortA());
        df->connect(mul_lmm_6_n11, sub_lmm_4312_n25, sub_lmm_4312_n25->getPortA());
        df->connect(sub_lmm_4312_n25, mul_n31, mul_n31->getPortA());
        df->connect(mul_n31, add_lmm_55296_n33, add_lmm_55296_n33->getPortA());
        df->connect(add_lmm_55296_n33, reg39, reg39->getPortA());
        df->connect(reg39, add_n32, add_n32->getPortA());
        df->connect(reg36, mul_n31, mul_n31->getPortB());
        df->connect(i2_n3[i], mul_lmm_464_n13, mul_lmm_464_n13->getPortA());
        df->connect(mul_lmm_464_n13, sub_lmm_13824_n29, sub_lmm_13824_n29->getPortA());
        df->connect(sub_lmm_13824_n29, reg40, reg40->getPortA());
        df->connect(reg40, reg41, reg41->getPortA());
        df->connect(reg41, sub_n15, sub_n15->getPortB());
        df->connect(reg35, mul_n30, mul_n30->getPortA());
        df->connect(mul_n30, reg43, reg43->getPortA());
        df->connect(reg43, reg44, reg44->getPortA());
        df->connect(reg44, reg45, reg45->getPortA());
        df->connect(reg45, reg46, reg46->getPortA());
        df->connect(reg46, reg47, reg47->getPortA());
        df->connect(reg47, reg48, reg48->getPortA());
        df->connect(reg48, reg49, reg49->getPortA());
        df->connect(reg49, reg50, reg50->getPortA());
        df->connect(reg50, add_n17, add_n17->getPortB());
        df->connect(i2_n3[i], sub_lmm_13824_n34, sub_lmm_13824_n34->getPortA());
        df->connect(sub_lmm_13824_n34, mul_n30, mul_n30->getPortB());
    }

    return df;
}

void Poly8::runCGRA(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};

    Poly8::cgraHw->loadCgraProgram("../cgra_bitstreams/poly8.cgra");
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < copies; ++j) {
            Poly8::cgraHw->setCgraProgramInputStreamByID(i, j * 4, data_in[i][j][0], sizeof(short) * data_size);
            Poly8::cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 1, data_in[i][j][1], sizeof(short) * data_size);
            Poly8::cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 2, data_in[i][j][2], sizeof(short) * data_size);
            Poly8::cgraHw->setCgraProgramOutputStreamByID(i, (j * 4) + 3, data_out[i][j], sizeof(short) * data_size);
        }
    }
    s = high_resolution_clock::now();
    Poly8::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    Poly8::cgraExecTime = diff.count() * 1000;
}

void Poly8::runCPU(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int j = 0; j < NUM_THREADS; j++) {
        for (int k = 0; k < copies; ++k) {
            for (int i = 0; i < data_size; ++i) {
                unsigned short A, B, C;
                A = data_in[j][k][0][i];
                B = data_in[j][k][1][i];
                C = data_in[j][k][2][i];
                data_out[j][k][i] = A * (A * (B * (Poly8::constants[4] * B * (B + Poly8::constants[0]) +
                                                   (Poly8::constants[6] * C + Poly8::constants[1])) +
                                              ((Poly8::constants[8] * C - Poly8::constants[11]) * C +
                                               Poly8::constants[3]) +
                                              A * (B * (B * (B - C)) + (Poly8::constants[7] * C + Poly8::constants[2]) -
                                                   (Poly8::constants[9] * C - Poly8::constants[10]))) +
                                         C * (Poly8::constants[5] * B - Poly8::constants[10])) +
                                    (C * (C - Poly8::constants[10]));
            }
        }
    }
    diff = high_resolution_clock::now() - s;
    Poly8::cpuExecTime = diff.count() * 1000;
}

void Poly8::benchmarking(int numThreads, int data_size) {
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

    if (Poly8::compile(numThreads, copies)) {
        Poly8::runCGRA(data_in, data_out_cgra, data_size, numThreads, copies);
        Poly8::runCPU(data_in, data_out_cpu, data_size, numThreads, copies);
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

bool Poly8::compile(int numThreads, int copies) {
    Scheduler scheduler(Poly8::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;
    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(Poly8::createDataFlow(i, copies));
        scheduler.addDataFlow(dfs[i], i, 0);
        Poly8::cgraArch->getNetBranch(i)->createRouteTable();
        Poly8::cgraArch->getNet(i)->createRouteTable();
    }

    s = high_resolution_clock::now();
    int r = scheduler.scheduling();
    diff = high_resolution_clock::now() - s;

    Poly8::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../cgra_bitstreams/%s.cgra", dfs[0]->getName().c_str());
        Poly8::cgraArch->writeCgraProgram(filename);
        sprintf(filename, "../dot_dataflows/%s.dot", dfs[0]->getName().c_str());
        dfs[0]->toDot(filename);
    } else {
        printf("Error on scheduling: Code %d\n", r);
    }
    for (auto df:dfs) {
        delete df;
    }
    return r == SCHEDULE_SUCCESS;
}

void Poly8::printStatistics() {
    int copies = 1;
    auto df = Poly8::createDataFlow(0, copies);
    MSG("INFO Poly8 Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << Poly8::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << Poly8::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << Poly8::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << Poly8::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << Poly8::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << Poly8::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << Poly8::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << Poly8::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << Poly8::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Scheduling time: " << Poly8::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Poly8::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Poly8::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Poly8::cpuExecTime << "ms");
    Poly8::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Poly8::cgraHw->getAccManagement()->printHwInfo();

    delete df;
}

unsigned short Poly8::pot(unsigned short num, unsigned short exp) {
    unsigned short i = 0, res = 1;
    for (i = 0; i < exp; i++)
        res = res * num;
    return res;
}
