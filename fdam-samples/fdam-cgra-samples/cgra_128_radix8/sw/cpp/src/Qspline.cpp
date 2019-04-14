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

DataFlow *Qspline::createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "qspline");
    int idx = 0;
    // level 1
    std::vector<Operator *> i0_n1;
    std::vector<Operator *> i1_n2;
    std::vector<Operator *> i2_n3;
    std::vector<Operator *> i3_n4;
    std::vector<Operator *> i4_n5;
    std::vector<Operator *> i5_n6;
    std::vector<Operator *> i6_n7;
    std::vector<Operator *> o0_n34;
    for (int i = 0; i < copies; ++i) {
        i0_n1.push_back(new InputStream(idx++));//0
        i1_n2.push_back(new InputStream(idx++));//1
        i2_n3.push_back(new InputStream(idx++));//2
        i3_n4.push_back(new InputStream(idx++));//3
        i4_n5.push_back(new InputStream(idx++));//4
        i5_n6.push_back(new InputStream(idx++));//5
        i6_n7.push_back(new InputStream(idx++));//6
        o0_n34.push_back(new OutputStream(idx++));//7
    }
    for (int j = 0; j < copies; ++j) {
        auto reg1 = new PassA(idx++);//8
        auto reg2 = new PassA(idx++);//9
        auto reg3 = new PassA(idx++);//10
        auto reg4 = new PassA(idx++);//11
        auto reg5 = new PassA(idx++);//12
        auto reg6 = new PassA(idx++);//13
        auto reg7 = new PassA(idx++);//14
        auto reg8 = new PassA(idx++);//15
        auto reg9 = new PassA(idx++);//16
        auto reg10 = new PassA(idx++);//17
        auto reg11 = new PassA(idx++);//18
        auto reg12 = new PassA(idx++);//19
        auto reg13 = new PassA(idx++);//20
        auto reg14 = new PassA(idx++);//21
        auto reg15 = new PassA(idx++);//22
        auto reg16 = new PassA(idx++);//23
        auto reg17 = new PassA(idx++);//24
        auto reg18 = new PassA(idx++);//25
        auto reg19 = new PassA(idx++);//26
        auto reg20 = new PassA(idx++);//27
        auto reg21 = new PassA(idx++);//28
        auto reg22 = new PassA(idx++);//29
        auto reg23 = new PassA(idx++);//30
        auto reg24 = new PassA(idx++);//31
        auto reg25 = new PassA(idx++);//32
        auto reg27 = new PassA(idx++);//33
        auto reg28 = new PassA(idx++);//34
        auto reg29 = new PassA(idx++);//35
        auto reg30 = new PassA(idx++);//36
        auto reg31 = new PassA(idx++);//37
        auto reg32 = new PassA(idx++);//38
        auto reg33 = new PassA(idx++);//39
        auto reg34 = new PassA(idx++);//40
        auto reg35 = new PassA(idx++);//41
        auto reg36 = new PassA(idx++);//42
        auto mul_n8 = new Mult(idx++);//43
        auto mul_n9 = new Mult(idx++);//44
        auto mul_n11 = new Mult(idx++);//45
        auto mul_n13 = new Mult(idx++);//46
        auto mul_n29 = new Mult(idx++);//47
        auto mul_n25 = new Mult(idx++);//48
        auto mul_n23 = new Mult(idx++);//49
        auto mul_n24 = new Mult(idx++);//50
        auto mul_n18 = new Mult(idx++);//51
        auto mul_n10 = new Mult(idx++);//52
        auto mul_n28 = new Mult(idx++);//53
        auto mul_n20 = new Mult(idx++);//54
        auto mul_n21 = new Mult(idx++);//55
        auto mul_n14 = new Mult(idx++);//56
        auto mul_n15 = new Mult(idx++);//57
        auto mul_n12 = new Mult(idx++);//58
        auto mul_n26 = new Mult(idx++);//59
        auto mul_n19 = new Mult(idx++);//60
        auto mul_n17 = new Mult(idx++);//61
        auto add_n30 = new Add(idx++);//62
        auto add_n31 = new Add(idx++);//63
        auto add_n32 = new Add(idx++);//64
        auto add_n33 = new Add(idx++);//65
        auto mul_lmm_6_n27 = new Multi(idx++, 6);//66
        auto mul_lmm_4_n22 = new Multi(idx++, 4);//67
        auto mul_lmm_4_n16 = new Multi(idx++, 4);//68


        df->connect(i4_n5[j], mul_n13, mul_n13->getPortA());
        df->connect(i1_n2[j], mul_n13, mul_n13->getPortB());

        df->connect(i1_n2[j], reg1, reg1->getPortA());
        df->connect(reg1, reg2, reg2->getPortA());
        df->connect(reg2, reg3, reg3->getPortA());
        df->connect(reg3, mul_n8, mul_n8->getPortA());

        df->connect(i1_n2[j], reg4, reg4->getPortA());
        df->connect(reg4, mul_n11, mul_n11->getPortA());

        df->connect(i1_n2[j], reg5, reg5->getPortA());
        df->connect(reg5, reg6, reg6->getPortA());
        df->connect(reg6, mul_n29, mul_n29->getPortA());

        df->connect(i1_n2[j], reg7, reg7->getPortA());
        df->connect(reg7, reg8, reg8->getPortA());
        df->connect(reg8, reg9, reg9->getPortA());
        df->connect(reg9, mul_n25, mul_n25->getPortA());

        df->connect(i1_n2[j], reg10, reg10->getPortA());
        df->connect(reg10, reg11, reg11->getPortA());
        df->connect(reg11, reg12, reg12->getPortA());
        df->connect(reg12, mul_n20, mul_n20->getPortA());

        df->connect(i1_n2[j], reg13, reg13->getPortA());
        df->connect(reg13, reg14, reg14->getPortA());
        df->connect(reg14, mul_n24, mul_n24->getPortA());

        df->connect(i1_n2[j], mul_n18, mul_n18->getPortA());

        df->connect(i1_n2[j], reg15, reg15->getPortA());
        df->connect(reg15, mul_n10, mul_n11->getPortB());

        df->connect(mul_n13, mul_n11, mul_n11->getPortB());

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
        df->connect(add_n30, o0_n34[j], o0_n34[j]->getPortA());
        df->connect(add_n33, add_n30, add_n30->getPortB());
        df->connect(mul_n28, mul_n9, mul_n9->getPortA());

        df->connect(mul_n9, reg20, reg20->getPortA());
        df->connect(reg20, add_n31, add_n31->getPortA());

        df->connect(add_n31, add_n32, add_n32->getPortA());
        df->connect(add_n32, add_n33, add_n33->getPortB());

        df->connect(mul_n18, reg19, reg19->getPortA());
        df->connect(reg19, mul_n9, mul_n9->getPortB());

        df->connect(mul_n20, mul_n21, mul_n21->getPortA());
        df->connect(mul_n21, add_n32, add_n32->getPortB());
        df->connect(i3_n4[j], mul_lmm_6_n27, mul_lmm_6_n27->getPortA());
        df->connect(mul_lmm_6_n27, mul_n28, mul_n28->getPortB());

        df->connect(i0_n1[j], mul_lmm_4_n22, mul_lmm_4_n22->getPortA());
        df->connect(mul_lmm_4_n22, mul_n23, mul_n23->getPortA());
        df->connect(mul_n23, mul_n24, mul_n24->getPortB());

        df->connect(i6_n7[j], mul_n14, mul_n14->getPortA());
        df->connect(mul_n14, mul_n15, mul_n15->getPortA());
        df->connect(mul_n15, mul_n12, mul_n12->getPortA());
        df->connect(mul_n12, mul_n26, mul_n26->getPortA());

        df->connect(mul_n26, add_n31, add_n31->getPortB());

        df->connect(i5_n6[j], mul_n18, mul_n18->getPortB());
        df->connect(i5_n6[j], mul_n10, mul_n10->getPortA());
        df->connect(i5_n6[j], mul_n14, mul_n14->getPortB());

        df->connect(i5_n6[j], reg21, reg21->getPortA());
        df->connect(reg21, mul_n15, mul_n15->getPortB());

        df->connect(i5_n6[j], reg22, reg22->getPortA());
        df->connect(reg22, reg23, reg23->getPortA());
        df->connect(reg23, mul_n12, mul_n12->getPortB());

        df->connect(i5_n6[j], reg24, reg24->getPortA());
        df->connect(reg24, reg25, reg25->getPortA());
        df->connect(reg25, reg27, reg27->getPortA());
        df->connect(reg27, mul_n26, mul_n26->getPortB());

        df->connect(i5_n6[j], reg28, reg28->getPortA());
        df->connect(reg28, reg29, reg29->getPortA());
        df->connect(reg29, reg30, reg30->getPortA());
        df->connect(reg30, reg31, reg31->getPortA());
        df->connect(reg31, mul_n21, mul_n21->getPortB());

        df->connect(i5_n6[j], reg32, reg32->getPortA());
        df->connect(reg32, reg33, reg33->getPortA());
        df->connect(reg33, mul_n19, mul_n19->getPortA());

        df->connect(i5_n6[j], reg34, reg34->getPortA());
        df->connect(reg34, mul_n17, mul_n17->getPortA());

        df->connect(mul_n19, mul_n20, mul_n20->getPortB());
        df->connect(mul_n17, mul_n19, mul_n19->getPortB());

        df->connect(i2_n3[j], mul_lmm_4_n16, mul_lmm_4_n16->getPortA());
        df->connect(mul_lmm_4_n16, mul_n17, mul_n17->getPortB());

        df->connect(mul_n10, mul_n23, mul_n23->getPortB());
    }
    return df;
}

void
Qspline::runCGRA(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};

    Qspline::cgraHw->loadCgraProgram("../cgra_bitstreams/qspline.cgra");
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < copies; ++j) {
            Qspline::cgraHw->setCgraProgramInputStreamByID(i, j * 8, data_in[i][j][0], sizeof(short) * data_size);
            Qspline::cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 1, data_in[i][j][1], sizeof(short) * data_size);
            Qspline::cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 2, data_in[i][j][2], sizeof(short) * data_size);
            Qspline::cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 3, data_in[i][j][3], sizeof(short) * data_size);
            Qspline::cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 4, data_in[i][j][4], sizeof(short) * data_size);
            Qspline::cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 5, data_in[i][j][5], sizeof(short) * data_size);
            Qspline::cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 6, data_in[i][j][6], sizeof(short) * data_size);
            Qspline::cgraHw->setCgraProgramOutputStreamByID(i, (j * 8) + 7, data_out[i][j], sizeof(short) * data_size);
        }
    }
    s = high_resolution_clock::now();
    Qspline::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    Qspline::cgraExecTime = diff.count() * 1000;
}

void
Qspline::runCPU(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int j = 0; j < NUM_THREADS; j++) {
        for (int k = 0; k < copies; ++k) {
            for (int i = 0; i < data_size; ++i) {
                data_out[j][k][i] = (unsigned short) (pot(data_in[j][k][1][i], 4) * data_in[j][k][0][i] +
                                                      4 * pot(data_in[j][k][5][i], 3) * data_in[j][k][1][i] *
                                                      data_in[j][k][6][i] +
                                                      6 * pot(data_in[j][k][1][i], 2) * pot(data_in[j][k][5][i], 2) *
                                                      data_in[j][k][2][i] +
                                                      pot(data_in[j][k][5][i], 4) * data_in[j][k][4][i] +
                                                      4 * pot(data_in[j][k][1][i], 3) * data_in[j][k][5][i] *
                                                      data_in[j][k][2][i]);
            }
        }

    }
    diff = high_resolution_clock::now() - s;
    Qspline::cpuExecTime = diff.count() * 1000;
}

void Qspline::benchmarking(int numThreads, int data_size) {
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
            data_in[i][j] = new unsigned short *[7];
            data_in[i][j][0] = new unsigned short[data_size];
            data_in[i][j][1] = new unsigned short[data_size];
            data_in[i][j][2] = new unsigned short[data_size];
            data_in[i][j][3] = new unsigned short[data_size];
            data_in[i][j][4] = new unsigned short[data_size];
            data_in[i][j][5] = new unsigned short[data_size];
            data_in[i][j][6] = new unsigned short[data_size];
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
                data_in[t][c][3][i] = 1;//(unsigned short) (random() % 256);
                data_in[t][c][4][i] = 1;//(unsigned short) (random() % 256);
                data_in[t][c][5][i] = 1;//(unsigned short) (random() % 256);
                data_in[t][c][6][i] = 1;//(unsigned short) (random() % 256);
                data_out_cpu[t][c][i] = 0;
                data_out_cgra[t][c][i] = 0;
            }
        }
    }

    if (Qspline::compile(numThreads, copies)) {
        Qspline::runCGRA(data_in, data_out_cgra, data_size, numThreads, copies);
        Qspline::runCPU(data_in, data_out_cpu, data_size, numThreads, copies);
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
            delete data_in[i][j][3];
            delete data_in[i][j][4];
            delete data_in[i][j][5];
            delete data_in[i][j][6];
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

bool Qspline::compile(int numThreads, int copies) {
    Scheduler scheduler(Qspline::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;
    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(Qspline::createDataFlow(i, copies));
        scheduler.addDataFlow(dfs[i], i, 0);
        Qspline::cgraArch->getNetBranch(i)->createRouteTable();
        Qspline::cgraArch->getNet(i)->createRouteTable();
    }

    int r = 0;
    for (int j = 0; j < 100; ++j) {
        s = high_resolution_clock::now();
        r = scheduler.scheduling();
        diff = high_resolution_clock::now() - s;
        if(r == SCHEDULE_SUCCESS )
            break;
    }

    Qspline::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../cgra_bitstreams/%s.cgra", dfs[0]->getName().c_str());
        Qspline::cgraArch->writeCgraProgram(filename);
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

void Qspline::printStatistics() {
    int copies = 1;
    auto df = Qspline::createDataFlow(0, copies);
    MSG("INFO Qspline Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << Qspline::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << Qspline::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << Qspline::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << Qspline::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << Qspline::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << Qspline::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << Qspline::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << Qspline::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << Qspline::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Num total edges: " << df->getNumEdges());
    MSG("INFO Scheduling time: " << Qspline::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Qspline::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Qspline::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Qspline::cpuExecTime << "ms");
    Qspline::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Qspline::cgraHw->getAccManagement()->printHwInfo();

    delete df;
}

unsigned short Qspline::pot(unsigned short num, unsigned short exp) {
    unsigned short i = 0, res = 1;
    for (i = 0; i < exp; i++)
        res = res * num;
    return res;
}


