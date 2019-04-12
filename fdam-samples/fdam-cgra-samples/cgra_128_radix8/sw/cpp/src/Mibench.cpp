//
// Created by lucas on 08/04/19.
//

#include <Mibench.h>

Mibench::Mibench(Cgra *cgra, CgraArch *cgraArch) {
    Mibench::cgraHw = cgra;
    Mibench::cgraArch = cgraArch;
    Mibench::schedulingTime = 0;
    Mibench::cpuExecTime = 0;
    Mibench::cgraExecTime = 0;
    Mibench::cgraConfTime = 0;
}

Mibench::~Mibench() = default;

DataFlow *Mibench::createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "mibench");
    int idx = 0;
    std::vector<Operator *> in1;
    std::vector<Operator *> in2;
    std::vector<Operator *> in3;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in1.push_back(new InputStream(idx++));
        in2.push_back(new InputStream(idx++));
        in3.push_back(new InputStream(idx++));
        out.push_back(new OutputStream(idx++));
    }
    for (int i = 0; i < copies; ++i) {
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

        auto mult1 = new Multi(idx++, 9);
        auto mult2 = new Multi(idx++, 6);
        auto mult3 = new Multi(idx++, 2);

        auto add1 = new Addi(idx++, 1);
        auto add2 = new Addi(idx++, 43);
        auto add3 = new Add(idx++);

        auto mult4 = new Mult(idx++);
        auto add4 = new Add(idx++);
        auto add5 = new Add(idx++);

        auto mult5 = new Mult(idx++);
        auto mult6 = new Mult(idx++);
        auto add6 = new Add(idx++);
        auto add7 = new Add(idx++);

        df->connect(in3[i], mult1, mult1->getPortA());
        df->connect(in3[i], mult2, mult2->getPortA());
        df->connect(in3[i], reg1, reg1->getPortA());
        df->connect(reg1, reg2, reg2->getPortA());

        df->connect(in1[i], mult3, mult3->getPortA());
        df->connect(in1[i], reg3, reg3->getPortA());
        df->connect(reg3, reg4, reg4->getPortA());
        df->connect(reg4, reg5, reg5->getPortA());

        df->connect(in2[i], reg6, reg6->getPortA());
        df->connect(reg6, reg7, reg7->getPortA());
        df->connect(reg7, reg8, reg8->getPortA());

        df->connect(mult1, add1, add1->getPortA());
        df->connect(mult2, add2, add2->getPortA());
        df->connect(mult3, add3, add3->getPortA());
        df->connect(reg6, add3, add3->getPortB());

        df->connect(add1, mult4, mult4->getPortA());
        df->connect(reg2, mult4, mult4->getPortB());

        df->connect(add2, add4, add4->getPortA());
        df->connect(reg4, add4, add4->getPortB());

        df->connect(add3, add5, add5->getPortA());
        df->connect(add2, add5, add5->getPortB());

        df->connect(add4, mult5, mult5->getPortA());
        df->connect(reg5, mult5, mult5->getPortB());

        df->connect(add5, mult6, mult6->getPortA());
        df->connect(reg8, mult6, mult6->getPortB());

        df->connect(mult5, add6, add6->getPortA());
        df->connect(mult6, add6, add6->getPortB());

        df->connect(add6, add7, add7->getPortA());
        df->connect(mult4, reg9, reg9->getPortA());
        df->connect(reg9, reg10, reg10->getPortA());
        df->connect(reg10, add7, add7->getPortB());

        df->connect(add7, out[i], out[i]->getPortA());
    }
    return df;
}

void
Mibench::runCGRA(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};

    Mibench::cgraHw->loadCgraProgram("../cgra_bitstreams/mibench.cgra");
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < copies; ++j) {
            Mibench::cgraHw->setCgraProgramInputStreamByID(i, (j * 4), data_in[i][j][0], sizeof(short) * data_size);
            Mibench::cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 1, data_in[i][j][1], sizeof(short) * data_size);
            Mibench::cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 2, data_in[i][j][2], sizeof(short) * data_size);
            Mibench::cgraHw->setCgraProgramOutputStreamByID(i, (j * 4) + 3, data_out[i][j], sizeof(short) * data_size);
        }
    }
    s = high_resolution_clock::now();
    Mibench::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    Mibench::cgraExecTime = diff.count() * 1000;
}

unsigned short Mibench::pot(unsigned short num, unsigned short exp) {
    unsigned short i = 0, res = 1;
    for (i = 0; i < exp; i++)
        res = res * num;
    return res;
}

void
Mibench::runCPU(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int j = 0; j < NUM_THREADS; j++) {
        for (int k = 0; k < copies; ++k) {
            for (int i = 0; i < data_size; ++i) {
                data_out[j][k][i] = (unsigned short) (data_in[j][k][2][i] + 9 * pot(data_in[j][k][2][i], 2) +
                                                      pot(data_in[j][k][0][i], 2) + 43 * data_in[j][k][0][i] +
                                                      6 * data_in[j][k][0][i] * data_in[j][k][2][i] +
                                                      43 * data_in[j][k][1][i] +
                                                      6 * data_in[j][k][1][i] * data_in[j][k][2][i] +
                                                      pot(data_in[j][k][1][i], 2) +
                                                      2 * data_in[j][k][0][i] * data_in[j][k][1][i]);
            }
        }

    }
    diff = high_resolution_clock::now() - s;
    Mibench::cpuExecTime = diff.count() * 1000;
}

void Mibench::benchmarking(int numThreads, int data_size) {

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
                data_in[t][c][0][i] = (unsigned short) (random() % 256);
                data_in[t][c][1][i] = (unsigned short) (random() % 256);
                data_in[t][c][2][i] = (unsigned short) (random() % 256);

                data_out_cpu[t][c][i] = 0;
                data_out_cgra[t][c][i] = 0;
            }
        }
    }

    if (Mibench::compile(numThreads, copies)) {
        Mibench::runCGRA(data_in, data_out_cgra, data_size, numThreads, copies);
        Mibench::runCPU(data_in, data_out_cpu, data_size, numThreads, copies);
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

bool Mibench::compile(int numThreads, int copies) {
    Scheduler scheduler(Mibench::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;
    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(Mibench::createDataFlow(i, copies));
        scheduler.addDataFlow(dfs[i], i, 0);
        Mibench::cgraArch->getNetBranch(i)->createRouteTable();
        Mibench::cgraArch->getNet(i)->createRouteTable();
    }

    s = high_resolution_clock::now();
    int r = scheduler.scheduling();
    diff = high_resolution_clock::now() - s;

    Mibench::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../cgra_bitstreams/%s.cgra", dfs[0]->getName().c_str());
        Mibench::cgraArch->writeCgraProgram(filename);
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

void Mibench::printStatistics() {
    int copies = 2;
    auto df = Mibench::createDataFlow(0, copies);
    MSG("INFO Paeth Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << Mibench::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << Mibench::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << Mibench::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << Mibench::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << Mibench::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << Mibench::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << Mibench::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << Mibench::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << Mibench::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Scheduling time: " << Mibench::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Mibench::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Mibench::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Mibench::cpuExecTime << "ms");
    Mibench::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Mibench::cgraHw->getAccManagement()->printHwInfo();

    delete df;
}


