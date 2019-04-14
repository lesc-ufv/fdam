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

    for (int i = 0; i < 12; ++i) {
        Poly5::constants.push_back(static_cast<unsigned short &&>(random() % 256 + 1));
    }
}

Poly5::~Poly5() {
    Poly5::constants.clear();
};

DataFlow *Poly5::createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "poly5");
    int idx = 0;

    std::vector<Operator *> in0;
    std::vector<Operator *> in1;
    std::vector<Operator *> in2;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in0.push_back(new InputStream(idx++));
        in1.push_back(new InputStream(idx++));
        in2.push_back(new InputStream(idx++));
        out.push_back(new OutputStream(idx++));
    }
    for (int i = 0; i < copies; ++i) {

        auto in0_3 = new PassA(idx++);
        auto in0_4 = new PassA(idx++);
        auto in0_5 = new PassA(idx++);

        auto in1_1 = new PassA(idx++);
        auto in1_2 = new PassA(idx++);
        auto in1_3 = new PassA(idx++);
        auto in1_4 = new PassA(idx++);
        auto in1_5 = new PassA(idx++);
        auto in1_6 = new PassA(idx++);
        auto in1_7 = new PassA(idx++);
        auto in1_8 = new PassA(idx++);

        auto in2_1 = new PassA(idx++);
        auto in2_2 = new PassA(idx++);
        auto in2_3 = new PassA(idx++);

        //level 1
        auto sub_imm_432_n28 = new Subi(idx++, Poly5::constants[0]);
        auto sub_imm_207_n26 = new Subi(idx++, Poly5::constants[1]);
        auto mul_imm_78_n9 = new Multi(idx++, Poly5::constants[2]);
        auto mul_imm_288_n8 = new Multi(idx++, Poly5::constants[3]);
        auto mul_imm_2_n21 = new Multi(idx++, Poly5::constants[4]);
        auto add_imm_144_n12 = new Addi(idx++, Poly5::constants[5]);
        //level 2
        auto mul_n17 = new Mult(idx++);
        auto sub_imm_9504_n16 = new Subi(idx++, Poly5::constants[6]);
        auto sub_imm_5184_n29 = new Subi(idx++, Poly5::constants[7]);
        auto mul_n7 = new Mult(idx++);
        auto sub_n18 = new Sub(idx++);
        //level 3
        auto add_imm_62208_n15 = new Addi(idx++, Poly5::constants[8]);
        auto mul_n11 = new Mult(idx++);
        auto mul_n19 = new Mult(idx++);
        auto add_imm_3456_n14 = new Addi(idx++, Poly5::constants[9]);
        auto mul_n6 = new Mult(idx++);
        //level 4
        auto mul_n20 = new Mult(idx++);
        auto add_n13 = new Add(idx++);
        auto sub_n10 = new Sub(idx++);
        //level 5
        auto sub_imm_2985984_n23 = new Subi(idx++, Poly5::constants[10]);
        auto sub_imm_248832_n22 = new Subi(idx++, Poly5::constants[11]);
        auto mul_n24 = new Mult(idx++);
        //level 6
        auto mul_n5 = new Mult(idx++);
        auto mul_n4 = new Mult(idx++);
        auto sub_imm_2985984_n23_reg1 = new PassA(idx++);
        //level 7
        auto mul_n4_reg1 = new PassA(idx++);
        auto add_n30 = new Add(idx++);
        auto add_n27 = new Add(idx++);
        //level 8
        auto mul_n25 = new Mult(idx++);

        //level 1
        df->connect(in2[i], sub_imm_432_n28, sub_imm_432_n28->getPortA());
        df->connect(in2[i], mul_imm_78_n9, mul_imm_78_n9->getPortA());
        df->connect(in2[i], mul_imm_288_n8, mul_imm_288_n8->getPortA());
        df->connect(in2[i], sub_imm_207_n26, sub_imm_207_n26->getPortA());
        df->connect(in2[i], mul_imm_2_n21, mul_imm_78_n9->getPortA());
        df->connect(in2[i], in2_1, in2_1->getPortA());
        df->connect(in1[i], add_imm_144_n12, add_imm_144_n12->getPortA());
        df->connect(in1[i], in1_1, in1_1->getPortA());
        //level 2
        df->connect(in2_1, mul_n17, mul_n17->getPortA());
        df->connect(sub_imm_432_n28, mul_n17, mul_n17->getPortB());
        df->connect(mul_imm_78_n9, sub_imm_9504_n16, sub_imm_9504_n16->getPortA());
        df->connect(mul_imm_288_n8, sub_imm_5184_n29, sub_imm_5184_n29->getPortA());
        df->connect(sub_imm_207_n26, mul_n7, mul_n7->getPortA());
        df->connect(in2_1, mul_n7, mul_n7->getPortB());
        df->connect(mul_imm_2_n21, sub_n18, sub_n18->getPortA());
        df->connect(add_imm_144_n12, sub_n18, sub_n18->getPortB());
        df->connect(in2_1, in2_2, in2_2->getPortA());
        df->connect(in1_1, in1_2, in1_2->getPortA());
        // level 3
        df->connect(in0[i], in0_3, in0_3->getPortA());
        df->connect(mul_n17, add_imm_62208_n15, add_imm_62208_n15->getPortA());
        df->connect(sub_imm_9504_n16, mul_n11, mul_n11->getPortA());
        df->connect(in2_2, mul_n11, mul_n11->getPortB());
        df->connect(sub_imm_5184_n29, mul_n19, mul_n19->getPortA());
        df->connect(in0[i], mul_n19, mul_n19->getPortB());
        df->connect(mul_n7, add_imm_3456_n14, add_imm_3456_n14->getPortA());
        df->connect(sub_n18, mul_n6, mul_n6->getPortA());
        df->connect(in0[i], mul_n6, mul_n6->getPortB());
        df->connect(in2_2, in2_3, in2_3->getPortA());
        df->connect(in1_2, in1_3, in1_3->getPortA());
        // level 4
        df->connect(in0_3, in0_4, in0_4->getPortA());
        df->connect(in2_3, mul_n20, mul_n20->getPortA());
        df->connect(add_imm_62208_n15, mul_n20, mul_n20->getPortB());
        df->connect(mul_n11, add_n13, add_n13->getPortA());
        df->connect(mul_n19, add_n13, add_n13->getPortB());
        df->connect(add_imm_3456_n14, sub_n10, sub_n10->getPortA());
        df->connect(mul_n6, sub_n10, sub_n10->getPortB());
        df->connect(in1_3, in1_4, in1_4->getPortA());
        // level 5
        df->connect(mul_n20, sub_imm_2985984_n23, sub_imm_2985984_n23->getPortA());
        df->connect(add_n13, sub_imm_248832_n22, sub_imm_248832_n22->getPortA());
        df->connect(sub_n10, mul_n24, mul_n24->getPortA());
        df->connect(in0_4, mul_n24, mul_n24->getPortB());
        df->connect(in1_4, in1_5, in1_5->getPortA());
        df->connect(in0_4, in0_5, in0_5->getPortA());
        // level 6
        df->connect(sub_imm_2985984_n23, sub_imm_2985984_n23_reg1, sub_imm_2985984_n23_reg1->getPortA());
        df->connect(in1_5, in1_6, in1_6->getPortA());

        df->connect(mul_n24, mul_n4, mul_n4->getPortA());
        df->connect(in0_5, mul_n4, mul_n4->getPortB());

        df->connect(sub_imm_248832_n22, mul_n5, mul_n5->getPortA());
        df->connect(in0_5, mul_n5, mul_n5->getPortB());

        //level 7
        df->connect(mul_n4, mul_n4_reg1, mul_n4_reg1->getPortA());
        df->connect(sub_imm_2985984_n23_reg1, add_n30, add_n30->getPortA());
        df->connect(mul_n5, add_n30, add_n30->getPortB());
        df->connect(in1_6, in1_7, in1_7->getPortA());
        // level 8
        df->connect(add_n30, add_n27, add_n27->getPortA());
        df->connect(mul_n4_reg1, add_n27, add_n27->getPortB());
        df->connect(in1_7, in1_8, in1_8->getPortA());
        //level 9
        df->connect(add_n27, mul_n25, mul_n25->getPortA());
        df->connect(in1_8, mul_n25, mul_n25->getPortB());
        //level 10
        df->connect(mul_n25, out[i], out[i]->getPortA());
    }

    return df;
}

void Poly5::runCGRA(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};

    Poly5::cgraHw->loadCgraProgram("../cgra_bitstreams/poly5.cgra");
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < copies; ++j) {
            Poly5::cgraHw->setCgraProgramInputStreamByID(i, j * 4, data_in[i][j][0], sizeof(short) * data_size);
            Poly5::cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 1, data_in[i][j][1], sizeof(short) * data_size);
            Poly5::cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 2, data_in[i][j][2], sizeof(short) * data_size);
            Poly5::cgraHw->setCgraProgramOutputStreamByID(i, (j * 4) + 3, data_out[i][j], sizeof(short) * data_size);
        }
    }
    s = high_resolution_clock::now();
    Poly5::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    Poly5::cgraExecTime = diff.count() * 1000;
}

unsigned short Poly5::pot(unsigned short num, unsigned short exp) {
    unsigned short i = 0, res = 1;
    for (i = 0; i < exp; i++)
        res = res * num;
    return res;
}

void Poly5::runCPU(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int j = 0; j < NUM_THREADS; j++) {
        for (int k = 0; k < copies; ++k) {
            for (int i = 0; i < data_size; ++i) {
                data_out[j][k][i] = data_in[j][k][0][i] * pot(data_in[j][k][1][i], 2) *
                                    (pot(data_in[j][k][2][i], 2) - Poly5::constants[1] * data_in[j][k][2][i] +
                                     Poly5::constants[9] -
                                     Poly5::constants[4] * data_in[j][k][0][i] * data_in[j][k][2][i] +
                                     Poly5::constants[5] * data_in[j][k][0][i] +
                                     data_in[j][k][0][i] * data_in[j][k][1][i]) +
                                    data_in[j][k][0][i] * data_in[j][k][1][i] *
                                    (Poly5::constants[2] * pot(data_in[j][k][2][i], 2) -
                                     Poly5::constants[6] * data_in[j][k][2][i] +
                                     Poly5::constants[3] * data_in[j][k][0][i] * data_in[j][k][2][i] -
                                     Poly5::constants[7] * data_in[j][k][0][i] - Poly5::constants[11]) +
                                    data_in[j][k][1][i] * pot(data_in[j][k][2][i], 3) +
                                    Poly5::constants[0] * data_in[j][k][1][i] * pot(data_in[j][k][2][i], 2) +
                                    Poly5::constants[8] * data_in[j][k][1][i] * data_in[j][k][2][i] -
                                    Poly5::constants[10] * data_in[j][k][1][i];
            }
        }
    }
    diff = high_resolution_clock::now() - s;
    Poly5::cpuExecTime = diff.count() * 1000;
}

void Poly5::benchmarking(int numThreads, int data_size) {
    int copies = 2;
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

    if (Poly5::compile(numThreads, copies)) {
        Poly5::runCGRA(data_in, data_out_cgra, data_size, numThreads, copies);
        Poly5::runCPU(data_in, data_out_cpu, data_size, numThreads, copies);
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

bool Poly5::compile(int numThreads, int copies) {
    Scheduler scheduler(Poly5::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;
    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(Poly5::createDataFlow(i, copies));
        scheduler.addDataFlow(dfs[i], i, 0);
        Poly5::cgraArch->getNetBranch(i)->createRouteTable();
        Poly5::cgraArch->getNet(i)->createRouteTable();
    }

    int r = 0;
    for (int j = 0; j < 1000; ++j) {
        s = high_resolution_clock::now();
        r = scheduler.scheduling();
        diff = high_resolution_clock::now() - s;
        if(r == SCHEDULE_SUCCESS )
            break;
    }

    Poly5::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../cgra_bitstreams/%s.cgra", dfs[0]->getName().c_str());
        Poly5::cgraArch->writeCgraProgram(filename);
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

void Poly5::printStatistics() {
    int copies = 2;
    auto df = Poly5::createDataFlow(0, copies);
    MSG("INFO Poly5 Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << Poly5::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << Poly5::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << Poly5::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << Poly5::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << Poly5::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << Poly5::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << Poly5::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << Poly5::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << Poly5::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Num total edges: " << df->getNumEdges());
    MSG("INFO Scheduling time: " << Poly5::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Poly5::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Poly5::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Poly5::cpuExecTime << "ms");
    Poly5::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Poly5::cgraHw->getAccManagement()->printHwInfo();

    delete df;
}