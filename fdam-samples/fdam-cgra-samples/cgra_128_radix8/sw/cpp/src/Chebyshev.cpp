//
// Created by lucas on 08/04/19.
//

#include <Chebyshev.h>

Chebyshev::Chebyshev(Cgra *cgra, CgraArch *cgraArch) {
    Chebyshev::cgraHw = cgra;
    Chebyshev::cgraArch = cgraArch;
    Chebyshev::schedulingTime = 0;
    Chebyshev::cpuExecTime = 0;
    Chebyshev::cgraExecTime = 0;
    Chebyshev::cgraConfTime = 0;
}

Chebyshev::~Chebyshev() = default;

DataFlow *Chebyshev::createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "chebyshev");
    int idx = 0;
    std::vector<Operator *> in;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in.push_back(new InputStream(idx++));
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
        auto mult1 = new Multi(idx++, 16);
        auto mult2 = new Mult(idx++);
        auto sub1 = new Subi(idx++, 20);
        auto mult3 = new Mult(idx++);
        auto mult4 = new Mult(idx++);
        auto add1 = new Addi(idx++, 5);
        auto mult5 = new Mult(idx++);

        df->connect(in[i], mult1, mult1->getPortA());
        df->connect(in[i], reg1, reg1->getPortA());
        df->connect(reg1, reg2, reg2->getPortA());
        df->connect(reg2, reg5, reg5->getPortA());
        df->connect(reg5, reg3, reg3->getPortA());
        df->connect(reg3, reg6, reg6->getPortA());
        df->connect(reg6, reg4, reg4->getPortA());
        df->connect(reg1, mult2, mult2->getPortA());
        df->connect(mult1, mult2, mult2->getPortB());
        df->connect(mult2, sub1, sub1->getPortA());
        df->connect(reg2, reg7, reg7->getPortA());
        df->connect(reg7, mult3, mult3->getPortA());
        df->connect(sub1, mult3, mult3->getPortB());
        df->connect(reg3, mult4, mult4->getPortA());
        df->connect(mult3, mult4, mult4->getPortB());
        df->connect(mult4, add1, add1->getPortA());
        df->connect(reg4, mult5, mult5->getPortA());
        df->connect(add1, mult5, mult5->getPortB());
        df->connect(mult5, out[i], out[i]->getPortA());
    }
    return df;
}

void
Chebyshev::runCGRA(unsigned short ***data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};

    Chebyshev::cgraHw->loadCgraProgram("../cgra_bitstreams/chebyshev.cgra");
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0, c = 0; j < copies; ++j) {
            Chebyshev::cgraHw->setCgraProgramInputStreamByID(i, c, data_in[i][j], sizeof(short) * data_size);
            Chebyshev::cgraHw->setCgraProgramOutputStreamByID(i, c + 1, data_out[i][j], sizeof(short) * data_size);
            c = c + 2;
        }
    }

    s = high_resolution_clock::now();
    Chebyshev::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    Chebyshev::cgraExecTime = diff.count() * 1000;

}

unsigned short Chebyshev::pot(unsigned short num, unsigned short exp) {
    unsigned short i = 0, res = 1;
    for (i = 0; i < exp; i++)
        res = res * num;
    return res;
}

void
Chebyshev::runCPU(unsigned short ***data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int i = 0; i < NUM_THREADS; ++i) {
        for (int j = 0; j < copies; ++j) {
            for (int k = 0; k < data_size; ++k) {
                data_out[i][j][k] = (unsigned short) (
                        5 * data_in[i][j][k] - 20 * pot(data_in[i][j][k], 3) + 16 * pot(data_in[i][j][k], 5));
            }
        }
    }
    diff = high_resolution_clock::now() - s;
    Chebyshev::cpuExecTime = diff.count() * 1000;
}

bool Chebyshev::compile(int numThreads, int copies) {

    Scheduler scheduler(Chebyshev::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;

    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(Chebyshev::createDataFlow(i, copies));
        scheduler.addDataFlow(dfs[i], i, 0);
        Chebyshev::cgraArch->getNetBranch(i)->createRouteTable();
        Chebyshev::cgraArch->getNet(i)->createRouteTable();
    }
    s = high_resolution_clock::now();
    int r = scheduler.scheduling();
    diff = high_resolution_clock::now() - s;
    Chebyshev::schedulingTime = diff.count() * 1000;
    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../cgra_bitstreams/%s.cgra", dfs[0]->getName().c_str());
        Chebyshev::cgraArch->writeCgraProgram(filename);
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

void Chebyshev::benchmarking(int numThreads, int data_size) {
    int copies = 6;
    unsigned short ***data_in;
    unsigned short ***data_out_cpu;
    unsigned short ***data_out_cgra;

    data_in = new unsigned short **[numThreads];
    data_out_cpu = new unsigned short **[numThreads];
    data_out_cgra = new unsigned short **[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        data_in[i] = new unsigned short *[copies];
        data_out_cpu[i] = new unsigned short *[copies];
        data_out_cgra[i] = new unsigned short *[copies];
        for (int j = 0; j < copies; ++j) {
            data_in[i][j] = new unsigned short[data_size];
            data_out_cpu[i][j] = new unsigned short[data_size];
            data_out_cgra[i][j] = new unsigned short[data_size];
        }
    }
    for (int k = 0; k < numThreads; ++k) {
        for (int j = 0; j < copies; ++j) {
            for (int i = 0; i < data_size; ++i) {
                data_in[k][j][i] = (unsigned short) (random() % 1024);
                data_out_cpu[k][j][i] = 0;
                data_out_cgra[k][j][i] = 0;

            }
        }
    }
    if (Chebyshev::compile(numThreads, copies)) {
        Chebyshev::runCGRA(data_in, data_out_cgra, data_size, numThreads, copies);
        Chebyshev::runCPU(data_in, data_out_cpu, data_size, numThreads, copies);
        for (int k = 0; k < numThreads; ++k) {
            for (int i = 0; i < copies; ++i) {
                for (int j = 0; j < data_size-7; ++j) {
                    if (data_out_cpu[k][i][j] != data_out_cgra[k][i][j]) {
                        printf("Error: Thread %d, queue %d, index %d, expected %d found %d!\n", k, i, j,
                               data_out_cpu[k][i][j],
                               data_out_cgra[k][i][j]);
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

void Chebyshev::printStatistics() {
    auto df = Chebyshev::createDataFlow(0, 7);
    MSG("INFO Chebyshev Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << Chebyshev::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << Chebyshev::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << Chebyshev::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << Chebyshev::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << Chebyshev::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << Chebyshev::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << Chebyshev::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << Chebyshev::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << Chebyshev::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Num total edges: " << df->getNumEdges());
    MSG("INFO Scheduling time: " << Chebyshev::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Chebyshev::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Chebyshev::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Chebyshev::cpuExecTime << "ms");
    Chebyshev::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Chebyshev::cgraHw->getAccManagement()->printHwInfo();

    delete df;
}
