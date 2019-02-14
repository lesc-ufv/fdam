//
// Created by lucas on 31/01/19.
//

#include <FIR.h>

FIR::FIR(Cgra *cgraHw, CgraArch *cgraArch, int size) {
    FIR::cgraHw = cgraHw;
    FIR::cgraArch = cgraArch;
    FIR::size = size;
    FIR::schedulingTime = 0;
    FIR::cpuExecTime = 0;
    FIR::cgraExecTime = 0;
    FIR::cgraConfTime = 0;
    FIR::coef = new unsigned short *[cgraArch->getNumThreads()];

    for (int j = 0; j < cgraArch->getNumThreads(); ++j) {
        FIR::coef[j] = new unsigned short[size];
        for (int i = 0; i < size; ++i) {
            FIR::coef[j][i] = (unsigned short) (random() % 48);
        }
    }
}

FIR::~FIR() {
    for (int j = 0; j < cgraArch->getNumThreads(); ++j) {
        delete FIR::coef[j];
    }
    delete FIR::coef;
}

DataFlow *FIR::createDataFlow(int thread) {
    auto df = new DataFlow(thread, "fir");
    int idx = 0;
    std::vector<Operator *> in_cp;
    std::vector<Operator *> out_cp;
    size_t copies = 1;

    in_cp.reserve(copies);
    for (int j = 0; j < copies; ++j) {
        in_cp.push_back(new InputStream(idx++));
    }
    out_cp.reserve(copies);
    for (int j = 0; j < copies; ++j) {
        out_cp.push_back(new OutputStream(idx++));
    }
    for (int j = 0; j < copies; ++j) {
        Operator *op, *op1, *op2;
        std::vector<Operator *> add;
        add.reserve((unsigned long) FIR::size - 1);
        for (int i = 0; i < FIR::size; ++i) {
            auto m = new Multi(idx++, FIR::coef[thread][FIR::size - i - 1]);
            if (i == 0) {
                op = new PassA(idx++);
            } else {
                op = new Add(idx++);
            }
            add.push_back(op);
            df->connect(in_cp[j], m, m->getPortA());
            df->connect(m, op, m->getPortA());
        }
        for (int i = 0; i < FIR::size - 1; ++i) {
            op1 = add[i];
            op2 = add[i + 1];
            df->connect(op1, op2, op2->getPortB());
        }
        op1 = add[FIR::size - 1];
        df->connect(op1, out_cp[j], out_cp[j]->getPortA());
    }

    return df;

}

void FIR::setCoef(unsigned short **coef) {
    for (int i = 0; i < FIR::size; ++i) {
        FIR::coef[i] = coef[i];
    }
}

bool FIR::compile(int numThreads) {
    Scheduler scheduler(FIR::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;

    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(FIR::createDataFlow(i));
        scheduler.addDataFlow(dfs[i], i, 0);
        FIR::cgraArch->getNetBranch(i)->createRouteTable();
        FIR::cgraArch->getNet(i)->createRouteTable();
    }

    s = high_resolution_clock::now();
    int r = scheduler.scheduling();
    diff = high_resolution_clock::now() - s;
    FIR::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../fir_files/%s.cgra", dfs[0]->getName().c_str());
        FIR::cgraArch->writeCgraProgram(filename);
        sprintf(filename, "../fir_files/%s.dot", dfs[0]->getName().c_str());
        dfs[0]->toDot(filename);
    } else {
        printf("Error on scheduling: Code %d\n", r);
    }
    for (auto df:dfs) {
        delete df;
    }
    return r == SCHEDULE_SUCCESS;
}

void FIR::runCGRA(unsigned short **data_in, unsigned short **data_out, int data_size, int numThreads) {
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    FIR::cgraHw->loadCgraProgram("../fir_files/fir.cgra");
    for (int i = 0; i < numThreads; ++i) {
        FIR::cgraHw->setCgraProgramInputStreamByID(i, 0, data_in[i], sizeof(unsigned short) * data_size);
        FIR::cgraHw->setCgraProgramOutputStreamByID(i, 1, data_out[i],
                                                    sizeof(unsigned short) * (data_size - FIR::size));
    }
    s = high_resolution_clock::now();
    FIR::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    FIR::cgraExecTime = diff.count() * 1000;

}

void FIR::runCPU(unsigned short **data_in, unsigned short **data_out, int data_size, int numThreads) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();

#pragma omp parallel
#pragma omp for
    for (int i = 0; i < NUM_THREADS; i++) {
        for (int j = 0; j < data_size - FIR::size; j++) {
            unsigned short fir = 0;
            for (int k = 0; k < FIR::size; ++k) {
                fir += data_in[i][j + k] * FIR::coef[i][FIR::size - 1 - k];
            }
            data_out[i][j] = fir;
        }
    }

    diff = high_resolution_clock::now() - s;
    FIR::cpuExecTime = diff.count() * 1000;
}

void FIR::printStatistics() {

    auto df = FIR::createDataFlow(0);
    MSG("INFO FIR Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << FIR::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << FIR::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << FIR::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << FIR::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << FIR::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << FIR::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << FIR::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << FIR::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << FIR::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Scheduling time: " << FIR::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << FIR::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << FIR::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << FIR::cpuExecTime << "ms");
    FIR::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    FIR::cgraHw->getAccManagement()->printHwInfo();
    delete df;
}

void FIR::benchmarking(int numThreads, int data_size) {

    unsigned short **data_in;
    unsigned short **data_out_cgra;
    unsigned short **data_out_cpu;

    if (data_size < FIR::size) {
        data_size = FIR::size + 1;
    }

    data_in = new unsigned short *[numThreads];
    data_out_cgra = new unsigned short *[numThreads];
    data_out_cpu = new unsigned short *[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        data_in[i] = new unsigned short[data_size];
        data_out_cgra[i] = new unsigned short[data_size];
        data_out_cpu[i] = new unsigned short[data_size];
    }
    for (int k = 0; k < numThreads; ++k) {
        for (int i = 0; i < data_size; ++i) {
            data_in[k][i] = (unsigned short) (random() % 64);
            data_out_cgra[k][i] = 0;
            data_out_cpu[k][i] = 0;
        }
    }

    if (FIR::compile(numThreads)) {
        FIR::runCGRA(data_in, data_out_cgra, data_size, numThreads);
        FIR::runCPU(data_in, data_out_cpu, data_size, numThreads);
        for (int k = 0; k < numThreads; ++k) {
            for (int j = 0; j < data_size - FIR::size; ++j) {
                if (data_out_cpu[k][j] != data_out_cgra[k][j]) {
                    printf("Error: Thread %d, index %d, expected %d found %d!\n", k, j, data_out_cpu[k][j],
                           data_out_cgra[k][j]);
                    break;
                }
            }
        }
    } else {
        printf("Compilation failed!\n");
    }
    for (int i = 0; i < numThreads; ++i) {
        delete data_in[i];
        delete data_out_cgra[i];
        delete data_out_cpu[i];
    }
    delete data_in;
    delete data_out_cgra;
    delete data_out_cpu;

}
