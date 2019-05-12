//
// Created by lucas on 01/02/19.
//

#include <LoopBack.h>
#include <common.h>

LoopBack::LoopBack(Cgra *cgra, CgraArch *cgraArch) {
    LoopBack::cgraHw = cgra;
    LoopBack::cgraArch = cgraArch;
    LoopBack::schedulingTime = 0;
    LoopBack::cpuExecTime = 0;
    LoopBack::cgraExecTime = 0;
    LoopBack::cgraConfTime = 0;
}

LoopBack::~LoopBack() = default;

void LoopBack::runCGRA(unsigned short ***data_in, unsigned short ***data_out, int data_size, int numThreads) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};

    LoopBack::cgraHw->loadCgraProgram("../cgra_bitstreams/loopback.cgra");
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < 8; ++j) {
            LoopBack::cgraHw->setCgraProgramInputStreamByID(i, j, data_in[i][j], sizeof(short) * data_size);
            LoopBack::cgraHw->setCgraProgramOutputStreamByID(i, j + 8, data_out[i][j], sizeof(short) * data_size);
        }
    }

    s = high_resolution_clock::now();
    LoopBack::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    LoopBack::cgraExecTime = diff.count() * 1000;

}

void LoopBack::runCPU(unsigned short ***data_in, unsigned short ***data_out, int data_size, int numThreads) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int i = 0; i < NUM_THREADS; ++i) {
        for (int j = 0; j < 8; ++j) {
            for (int k = 0; k < data_size; ++k) {
                data_out[i][j][k] = data_in[i][j][k];
            }
        }
    }
    diff = high_resolution_clock::now() - s;
    LoopBack::cpuExecTime = diff.count() * 1000;
}

bool LoopBack::compile(int numThreads) {
    Scheduler scheduler(LoopBack::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;

    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(LoopBack::createDataFlow(i));
        scheduler.addDataFlow(dfs[i], i, 0);
        LoopBack::cgraArch->getNetBranch(i)->createRouteTable();
        LoopBack::cgraArch->getNet(i)->createRouteTable();
    }
    int r = 0;
    for (int j = 0; j < 100; ++j) {
        s = high_resolution_clock::now();
        r = scheduler.scheduling();
        diff = high_resolution_clock::now() - s;
        if(r == SCHEDULE_SUCCESS )
            break;
    }

    LoopBack::schedulingTime = diff.count() * 1000;
    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../cgra_bitstreams/%s.cgra", dfs[0]->getName().c_str());
        LoopBack::cgraArch->writeCgraProgram(filename);
    } else {
        printf("Error on scheduling: Code %d\n", r);
    }
    for (auto df:dfs) {
        delete df;
    }
    return r == SCHEDULE_SUCCESS;
}

void LoopBack::benchmarking(int numThreads, int data_size) {

    unsigned short ***data_in;
    unsigned short ***data_out_cpu;
    unsigned short ***data_out_cgra;

    data_in = new unsigned short **[numThreads];
    data_out_cpu = new unsigned short **[numThreads];
    data_out_cgra = new unsigned short **[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        data_in[i] = new unsigned short *[8];
        data_out_cpu[i] = new unsigned short *[8];
        data_out_cgra[i] = new unsigned short *[8];
        for (int j = 0; j < 8; ++j) {
            data_in[i][j] = new unsigned short[data_size];
            data_out_cpu[i][j] = new unsigned short[data_size];
            data_out_cgra[i][j] = new unsigned short[data_size];
        }
    }
    for (int k = 0; k < numThreads; ++k) {
        for (int j = 0; j < 8; ++j) {
            for (int i = 0; i < data_size; ++i) {
                data_in[k][j][i] = (unsigned short) (random() % 1024);
                data_out_cpu[k][j][i] = 0;
                data_out_cgra[k][j][i] = 0;

            }
        }
    }

    if (LoopBack::compile(numThreads)) {
        LoopBack::runCGRA(data_in, data_out_cgra, data_size, numThreads);
        LoopBack::runCPU(data_in, data_out_cpu, data_size, numThreads);
        for (int k = 0; k < numThreads; ++k) {
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < data_size; ++j) {
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
        for (int j = 0; j < 8; ++j) {
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

DataFlow *LoopBack::createDataFlow(int id) {
    auto df = new DataFlow(id, "loopback");
    std::vector<Operator *> in;
    std::vector<Operator *> out;
    int idx = 0;
    int pipe = 15;
    in.reserve(8);
    for (int j = 0; j < 8; ++j) {
        in.push_back(new InputStream(idx++));
    }
    out.reserve(8);
    for (int j = 0; j < 8; ++j) {
        out.push_back(new OutputStream(idx++));
    }
    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < pipe - 1; ++i) {
            if (random() % 10 > 5) {
                auto r = new PassA(idx++);
                df->connect(in[j], r, r->getPortA());
                in[j] = r;
            } else {
                auto r = new PassB(idx++);
                df->connect(in[j], r, r->getPortB());
                in[j] = r;
            }
        }
        df->connect(in[j], out[j], out[j]->getPortA());
    }
    return df;
}

void LoopBack::printStatistics() {
    auto df = LoopBack::createDataFlow(0);
    MSG("INFO LoopBack Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << LoopBack::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << LoopBack::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << LoopBack::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << LoopBack::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << LoopBack::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << LoopBack::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << LoopBack::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << LoopBack::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << LoopBack::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Num total edges: " << df->getNumEdges());
    MSG("INFO Scheduling time: " << LoopBack::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << LoopBack::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << LoopBack::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << LoopBack::cpuExecTime << "ms");
    LoopBack::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    LoopBack::cgraHw->getAccManagement()->printHwInfo();
    delete df;
}




