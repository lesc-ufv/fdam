//
// Created by lucas on 01/02/19.
//

#ifndef MAIN_PAETH_H
#define MAIN_PAETH_H

#include <chrono>
#include <fdam/cgra/cgra.h>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/cgra_arch.h>
#include <fdam/cgra/scheduler.h>
#include <fdam/cgra/scheduler_defs.h>
#include <conf.h>

using namespace std::chrono;

class Paeth {
private:
    Cgra *cgraHw;
    CgraArch *cgraArch;
    double schedulingTime;
    double cpuExecTime;
    double cgraExecTime;
    double cgraConfTime;

    void runCGRA(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads);

    void runCPU(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads);

    bool compile(int numThreads);

public:

    Paeth(Cgra *cgra, CgraArch *cgraArch);

    ~Paeth();

    DataFlow *createDataFlow(int id);

    void benchmarking(int numThreads, int data_size);

    void printStatistics();
};


#endif //MAIN_PAETH_H
