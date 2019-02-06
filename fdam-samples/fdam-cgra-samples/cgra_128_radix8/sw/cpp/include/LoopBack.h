//
// Created by lucas on 01/02/19.
//

#ifndef MAIN_LOOPBACK_H
#define MAIN_LOOPBACK_H

#include <chrono>
#include <fdam/cgra/cgra.h>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/cgra_arch.h>
#include <fdam/cgra/scheduler.h>
#include <fdam/cgra/scheduler_defs.h>

using namespace std::chrono;

#define NUM_THREADS 8

class LoopBack {
private:
    Cgra *cgraHw;
    CgraArch *cgraArch;
    double schedulingTime;
    double cpuExecTime;
    double cgraExecTime;
    double cgraConfTime;

    DataFlow *createDataFlow(int id);

    void runCGRA(unsigned short ***data_in, unsigned short ***data_out, int data_size, int numThreads);

    void runCPU(unsigned short ***data_in, unsigned short ***data_out, int data_size, int numThreads);

    void compile(int numThreads);

public:

    LoopBack(Cgra *cgra, CgraArch *cgraArch);

    ~LoopBack();

    void benchmarking(int numThreads, int data_size);

    void printStatistics();
};


#endif //MAIN_LOOPBACK_H
