//
// Created by lucas on 08/04/19.
//

#include <Sgfilter.h>

Sgfilter::Sgfilter(Cgra *cgra, CgraArch *cgraArch) {
    Sgfilter::cgraHw = cgra;
    Sgfilter::cgraArch = cgraArch;
    Sgfilter::schedulingTime = 0;
    Sgfilter::cpuExecTime = 0;
    Sgfilter::cgraExecTime = 0;
    Sgfilter::cgraConfTime = 0;
}

Sgfilter::~Sgfilter() = default;

DataFlow *Sgfilter::createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "sgfilter");
    int idx = 0;
    std::vector<Operator *> in1;
    std::vector<Operator *> in2;
    std::vector<Operator *> out;

    for (int i = 0; i < copies; ++i) {
        in1.push_back(new InputStream(idx++));
        in2.push_back(new InputStream(idx++));
        out.push_back(new OutputStream(idx++));
    }
    for (int j = 0; j < copies; ++j) {
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
        auto mult1 = new Multi(idx++, 92);
        auto mult2 = new Multi(idx++, -984);
        auto mult3 = new Multi(idx++, -76);
        auto mult4 = new Multi(idx++, 7);
        auto sub1 = new Subi(idx++, 46);
        auto sub2 = new Subi(idx++, 39);
        auto add1 = new Add(idx++);
        auto mult5 = new Mult(idx++);
        auto mult6 = new Mult(idx++);
        auto add2 = new Addi(idx++, 7);
        auto sub3 = new Subi(idx++, 46);
        auto mult7 = new Mult(idx++);
        auto mult8 = new Mult(idx++);
        auto add3 = new Add(idx++);
        auto add4 = new Addi(idx++, 7);
        auto mult9 = new Mult(idx++);
        auto add5 = new Add(idx++);
        auto sub4 = new Subi(idx++, 75);

        df->connect(in1[j], mult1, mult1->getPortA());
        df->connect(in1[j], mult2, mult2->getPortA());
        df->connect(in1[j], mult3, mult3->getPortA());
        df->connect(in1[j], reg11, reg11->getPortA());
        df->connect(reg11, reg12, reg12->getPortA());
        df->connect(in1[j], reg1, reg1->getPortA());
        df->connect(reg1, reg2, reg2->getPortA());
        df->connect(reg2, reg3, reg3->getPortA());
        df->connect(reg3, reg4, reg4->getPortA());
        df->connect(in2[j], mult4, mult4->getPortA());
        df->connect(in2[j], reg5, reg5->getPortA());
        df->connect(reg5, reg6, reg6->getPortA());
        df->connect(reg6, reg7, reg7->getPortA());
        df->connect(reg7, reg8, reg8->getPortA());
        df->connect(reg8, reg9, reg9->getPortA());
        df->connect(reg9, reg10, reg10->getPortA());
        df->connect(mult2, sub1, sub1->getPortA());
        df->connect(mult1, sub2, sub2->getPortA());
        df->connect(mult3, add1, add1->getPortA());
        df->connect(mult4, add1, add1->getPortB());
        df->connect(reg12, mult5, mult5->getPortA());
        df->connect(sub1, mult5, mult5->getPortB());
        df->connect(reg2, mult6, mult6->getPortA());
        df->connect(sub2, mult6, mult6->getPortB());
        df->connect(add1, add2, add2->getPortA());
        df->connect(mult5, sub3, sub3->getPortA());
        df->connect(reg7, mult7, mult7->getPortA());
        df->connect(add2, mult7, mult7->getPortB());
        df->connect(reg4, mult8, mult8->getPortA());
        df->connect(sub3, mult8, mult8->getPortB());
        df->connect(mult6, reg13, reg13->getPortA());
        df->connect(reg13, add3, add3->getPortA());
        df->connect(mult7, add3, add3->getPortB());
        df->connect(add3, add4, add4->getPortA());
        df->connect(add4, mult9, mult9->getPortA());
        df->connect(reg10, mult9, mult9->getPortB());
        df->connect(mult9, add5, add5->getPortA());
        df->connect(mult8, reg14, reg14->getPortB());
        df->connect(reg14, reg15, reg15->getPortA());
        df->connect(reg15, add5, add5->getPortB());
        df->connect(add5, sub4, sub4->getPortA());
        df->connect(sub4, out[j], out[j]->getPortA());
    }
    return df;
}

void
Sgfilter::runCGRA(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};

    Sgfilter::cgraHw->loadCgraProgram("../cgra_bitstreams/sgfilter.cgra");
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < copies; ++j) {
            Sgfilter::cgraHw->setCgraProgramInputStreamByID(i, (j * 3), data_in[i][j][0], sizeof(short) * data_size);
            Sgfilter::cgraHw->setCgraProgramInputStreamByID(i, (j * 3) + 1, data_in[i][j][1],
                                                            sizeof(short) * data_size);
            Sgfilter::cgraHw->setCgraProgramOutputStreamByID(i, (j * 3) + 2, data_out[i][j], sizeof(short) * data_size);
        }
    }

    s = high_resolution_clock::now();
    Sgfilter::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    Sgfilter::cgraExecTime = diff.count() * 1000;

}

unsigned short Sgfilter::pot(unsigned short num, unsigned short exp) {
    unsigned short i = 0, res = 1;
    for (i = 0; i < exp; i++)
        res = res * num;
    return res;
}

void
Sgfilter::runCPU(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int j = 0; j < NUM_THREADS; j++) {
        for (int k = 0; k < copies; ++k) {
            for (int i = 0; i < data_size; ++i) {
                data_out[j][k][i] = (unsigned short) (7 * data_in[j][k][1][i] +
                                                      7 * pot(data_in[j][k][1][i], 2) +
                                                      7 * pot(data_in[j][k][1][i], 3) -
                                                      76 * data_in[j][k][0][i] * pot(data_in[j][k][1][i], 2) +
                                                      39 * data_in[j][k][1][i] * data_in[j][k][0][i] -
                                                      92 * data_in[j][k][1][i] * pot(data_in[j][k][0][i], 2) -
                                                      75 - 46 * data_in[j][k][0][i] -
                                                      46 * pot(data_in[j][k][0][i], 2) -
                                                      984 * pot(data_in[j][k][0][i], 3));
            }
        }
    }
    diff = high_resolution_clock::now() - s;
    Sgfilter::cpuExecTime = diff.count() * 1000;
}

bool Sgfilter::compile(int numThreads, int copies) {
    Scheduler scheduler(Sgfilter::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;
    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(Sgfilter::createDataFlow(i, copies));
        scheduler.addDataFlow(dfs[i], i, 0);
        Sgfilter::cgraArch->getNetBranch(i)->createRouteTable();
        Sgfilter::cgraArch->getNet(i)->createRouteTable();
    }

    int r = 0;
    for (int j = 0; j < 100; ++j) {
        s = high_resolution_clock::now();
        r = scheduler.scheduling();
        diff = high_resolution_clock::now() - s;
        if(r == SCHEDULE_SUCCESS )
            break;
    }

    Sgfilter::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../cgra_bitstreams/%s.cgra", dfs[0]->getName().c_str());
        Sgfilter::cgraArch->writeCgraProgram(filename);
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

void Sgfilter::benchmarking(int numThreads, int data_size) {
    int copies = 3;
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
            data_in[i][j] = new unsigned short *[2];
            data_in[i][j][0] = new unsigned short[data_size];
            data_in[i][j][1] = new unsigned short[data_size];
            data_out_cpu[i][j] = new unsigned short[data_size];
            data_out_cgra[i][j] = new unsigned short[data_size];
        }
    }

    for (int t = 0; t < numThreads; ++t) {
        for (int c = 0; c < copies; ++c) {
            for (int i = 0; i < data_size; ++i) {
                data_in[t][c][0][i] = (unsigned short) (random() % 256);
                data_in[t][c][1][i] = (unsigned short) (random() % 256);
                data_out_cpu[t][c][i] = 0;
                data_out_cgra[t][c][i] = 0;
            }
        }
    }

    if (Sgfilter::compile(numThreads, copies)) {
        Sgfilter::runCGRA(data_in, data_out_cgra, data_size, numThreads, copies);
        Sgfilter::runCPU(data_in, data_out_cpu, data_size, numThreads, copies);
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

void Sgfilter::printStatistics() {
    int copies = 3;
    auto df = Sgfilter::createDataFlow(0, copies);
    MSG("INFO Sgfilter Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << Sgfilter::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << Sgfilter::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << Sgfilter::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << Sgfilter::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << Sgfilter::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << Sgfilter::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << Sgfilter::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << Sgfilter::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << Sgfilter::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Num total edges: " << df->getNumEdges());
    MSG("INFO Scheduling time: " << Sgfilter::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Sgfilter::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Sgfilter::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Sgfilter::cpuExecTime << "ms");
    Sgfilter::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Sgfilter::cgraHw->getAccManagement()->printHwInfo();

    delete df;
}