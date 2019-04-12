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
        Poly5::constants.push_back((random() % 256 + 1));
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
        auto reg1 = new PassB(idx++);
        auto reg2 = new PassB(idx++);
        auto reg3 = new PassB(idx++);
        auto reg4 = new PassB(idx++);
        auto reg5 = new PassB(idx++);
        auto reg6 = new PassB(idx++);
        auto reg7 = new PassB(idx++);
        auto reg8 = new PassB(idx++);
        auto reg9 = new PassB(idx++);
        auto reg10 = new PassB(idx++);
        auto reg11 = new PassB(idx++);
        auto reg12 = new PassB(idx++);
        auto reg13 = new PassB(idx++);
        auto reg14 = new PassB(idx++);
        auto reg15 = new PassB(idx++);
        auto reg16 = new PassB(idx++);
        auto reg17 = new PassB(idx++);
        auto reg18 = new PassB(idx++);
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
        auto reg34 = new PassB(idx++);
        auto reg36 = new PassB(idx++);
        auto reg37 = new PassB(idx++);
        auto reg38 = new PassB(idx++);
        auto sub1 = new Subi(idx++, Poly5::constants[0]);
        auto sub2 = new Subi(idx++, Poly5::constants[1]);
        auto mult1 = new Multi(idx++, Poly5::constants[2]);
        auto mult2 = new Multi(idx++, Poly5::constants[3]);
        auto mult3 = new Multi(idx++, Poly5::constants[4]);
        auto add1 = new Addi(idx++, Poly5::constants[5]);
        auto sub3 = new Subi(idx++, Poly5::constants[6]);
        auto sub4 = new Subi(idx++, Poly5::constants[7]);
        auto mult4 = new Mult(idx++);
        auto mult5 = new Mult(idx++);
        auto sub41 = new Sub(idx++);
        auto add2 = new Addi(idx++, Poly5::constants[8]);
        auto mult6 = new Mult(idx++);
        auto mult7 = new Mult(idx++);
        auto add3 = new Addi(idx++, Poly5::constants[9]);
        auto mult9 = new Mult(idx++);
        auto sub5 = new Sub(idx++);
        auto sub6 = new Subi(idx++, Poly5::constants[10]);
        auto sub7 = new Subi(idx++, Poly5::constants[11]);
        auto mult10 = new Mult(idx++);
        auto mult11 = new Mult(idx++);
        auto add5 = new Add(idx++);
        auto add6 = new Add(idx++);
        auto add7 = new Add(idx++);
        auto mult12 = new Mult(idx++);
        auto mult13 = new Mult(idx++);


        // level 1
        df->connect(in2[i], reg1, reg1->getPortB());
        df->connect(in2[i], reg2, reg2->getPortB());
        df->connect(in2[i], sub1, sub1->getPortA());
        df->connect(in2[i], reg3, reg3->getPortB());
        df->connect(in2[i], mult1, mult1->getPortA());
        df->connect(in2[i], mult2, mult2->getPortA());
        df->connect(in2[i], reg4, reg4->getPortB());
        df->connect(in2[i], sub2, sub2->getPortA());
        df->connect(in2[i], mult3, mult3->getPortA());
        df->connect(in0[i], reg5, reg5->getPortB());
        df->connect(in0[i], reg6, reg6->getPortB());
        df->connect(in0[i], reg7, reg7->getPortB());
        df->connect(in0[i], reg8, reg8->getPortB());
        df->connect(in1[i], add1, add1->getPortA());
        df->connect(in1[i], reg9, reg9->getPortB());
        df->connect(in1[i], reg10, reg10->getPortB());
        // level Poly5::constants[4]
        df->connect(reg1, reg11, reg11->getPortB());
        df->connect(reg2, mult4, mult4->getPortB());
        df->connect(sub1, mult4, mult4->getPortA());
        df->connect(reg3, reg12, reg12->getPortB());
        df->connect(mult1, sub3, sub3->getPortA());
        df->connect(mult2, sub4, sub4->getPortA());
        df->connect(reg4, mult4, mult4->getPortB());
        df->connect(sub2, mult4, mult4->getPortA());
        df->connect(mult3, sub41, sub41->getPortB());
        df->connect(reg5, reg13, reg13->getPortB());
        df->connect(reg6, reg14, reg14->getPortB());
        df->connect(reg7, reg15, reg15->getPortB());
        df->connect(reg8, reg16, reg16->getPortB());
        df->connect(add1, sub41, sub41->getPortA());
        df->connect(reg9, reg17, reg17->getPortB());
        df->connect(reg10, reg18, reg18->getPortB());
        // level 3
        df->connect(reg11, reg20, reg20->getPortB());
        df->connect(mult4, add2, add2->getPortA());
        df->connect(reg12, mult5, mult5->getPortB());
        df->connect(sub3, mult5, mult5->getPortA());
        df->connect(sub4, mult6, mult6->getPortA());
        df->connect(mult4, add3, add3->getPortA());
        df->connect(sub41, mult7, mult7->getPortA());
        df->connect(reg13, mult6, mult6->getPortB());
        df->connect(reg14, reg21, reg21->getPortB());
        df->connect(reg15, mult7, mult7->getPortB());
        df->connect(reg16, reg22, reg22->getPortB());
        df->connect(reg17, reg23, reg23->getPortB());
        df->connect(reg18, reg24, reg24->getPortB());
        // level 4
        df->connect(reg20, mult9, mult9->getPortA());
        df->connect(add2, mult9, mult9->getPortB());
        df->connect(mult5, add5, add5->getPortA());
        df->connect(mult6, add5, add5->getPortB());
        df->connect(add3, sub5, sub5->getPortA());
        df->connect(mult7, sub5, sub5->getPortB());
        df->connect(reg21, reg25, reg25->getPortB());
        df->connect(reg22, reg26, reg26->getPortB());
        df->connect(reg23, reg27, reg27->getPortB());
        df->connect(reg24, reg28, reg28->getPortB());
        // level 5
        df->connect(mult9, sub6, sub6->getPortA());
        df->connect(add5, sub7, sub7->getPortA());
        df->connect(sub5, mult10, mult10->getPortA());
        df->connect(reg25, mult10, mult10->getPortB());
        df->connect(reg26, reg29, reg29->getPortB());
        df->connect(reg27, reg30, reg30->getPortB());
        df->connect(reg28, reg31, reg31->getPortB());
        // level 6
        df->connect(sub6, reg32, reg32->getPortB());
        df->connect(sub7, mult11, mult11->getPortA());
        df->connect(mult10, mult12, mult12->getPortA());
        df->connect(reg29, mult11, mult11->getPortB());
        df->connect(reg31, reg34, reg34->getPortB());
        // level 7
        df->connect(reg30, mult12, mult12->getPortB());
        df->connect(reg32, add6, add6->getPortB());
        df->connect(mult11, add6, add6->getPortA());
        df->connect(mult12, reg36, reg36->getPortB());
        df->connect(add6, add7, add7->getPortA());
        df->connect(reg36, add7, add7->getPortB());

        df->connect(add7, mult13, mult13->getPortA());
        df->connect(reg34, reg37, reg37->getPortB());
        df->connect(reg37, reg38, reg38->getPortB());

        df->connect(reg38, mult13, mult13->getPortB());
        df->connect(mult13, out[i], out[i]->getPortA());
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
                data_out[j][k][i] = (unsigned short) (data_in[j][k][0][i] * pot(data_in[j][k][1][i], 2) *
                                                      (pot(data_in[j][k][2][i], 2) -
                                                       Poly5::constants[1] * data_in[j][k][2][i] + Poly5::constants[9] -
                                                       Poly5::constants[4] * data_in[j][k][0][i] * data_in[j][k][2][i] +
                                                       Poly5::constants[5] * data_in[j][k][0][i] +
                                                       data_in[j][k][0][i] * data_in[j][k][1][i]) +
                                                      data_in[j][k][0][i] * data_in[j][k][1][i] *
                                                      (Poly5::constants[2] * pot(data_in[j][k][2][i], 2) -
                                                       Poly5::constants[6] * data_in[j][k][2][i] +
                                                       Poly5::constants[3] * data_in[j][k][0][i] * data_in[j][k][2][i] -
                                                       Poly5::constants[7] * data_in[j][k][0][i] -
                                                       Poly5::constants[11]) +
                                                      data_in[j][k][1][i] * pot(data_in[j][k][2][i], 3) +
                                                      Poly5::constants[0] * data_in[j][k][1][i] *
                                                      pot(data_in[j][k][2][i], 2) +
                                                      Poly5::constants[8] * data_in[j][k][1][i] * data_in[j][k][2][i] -
                                                      Poly5::constants[10] * data_in[j][k][1][i]);
            }
        }

    }
    diff = high_resolution_clock::now() - s;
    Poly5::cpuExecTime = diff.count() * 1000;
}

void Poly5::benchmarking(int numThreads, int data_size) {
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
                data_in[t][c][0][i] = (unsigned short) (random() % 256);
                data_in[t][c][1][i] = (unsigned short) (random() % 256);
                data_in[t][c][2][i] = (unsigned short) (random() % 256);

                data_out_cpu[t][c][i] = 0;
                data_out_cgra[t][c][i] = 0;
            }
        }
    }

    if (Poly5::compile(numThreads, copies)) {
        Poly5::runCGRA(data_in, data_out_cgra, data_size, numThreads, copies);
        Poly5::runCPU(data_in, data_out_cpu, data_size, numThreads, copies);
        for (int t = 0; t < numThreads; ++t) {
            for (int c = 0; c < copies; ++c) {
                for (int i = 0; i < data_size; ++i) {
                    if (data_out_cpu[t][c][i] != data_out_cgra[t][c][i]) {
                        printf("Error: Thread %d, copy %d, index %d, expected %d found %d!\n", t, c, i,
                               data_out_cpu[t][c][i],
                               data_out_cgra[t][c][i]);
                        break;
                    }
                }
            }
        }
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

    s = high_resolution_clock::now();
    int r = scheduler.scheduling();
    diff = high_resolution_clock::now() - s;

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
    int copies = 1;
    auto df = Poly5::createDataFlow(0, copies);
    MSG("INFO Paeth Statictics");
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
    MSG("INFO Scheduling time: " << Poly5::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Poly5::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Poly5::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Poly5::cpuExecTime << "ms");
    Poly5::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Poly5::cgraHw->getAccManagement()->printHwInfo();

    delete df;
}