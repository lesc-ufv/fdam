//
// Created by lucas on 31/01/19.
//

#ifndef MAIN_FIR_H
#define MAIN_FIR_H


#include <chrono>
#include <fdam/cgra/cgra.h>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/cgra_arch.h>
#include <fdam/cgra/scheduler.h>
#include <fdam/cgra/scheduler_defs.h>
#include <conf.h>

using namespace std::chrono;

class FIR {
private:
    Cgra *cgraHw;
    CgraArch *cgraArch;
    int size;
    unsigned short **coef;
    double schedulingTime;
    double cpuExecTime;
    double cgraExecTime;
    double cgraConfTime;

    void setCoef(unsigned short **coef);

    void runCGRA(unsigned short **data_in, unsigned short **data_out, int data_size, int numThreads);

    void runCPU(unsigned short **data_in, unsigned short **data_out, int data_size, int numThreads);

    bool compile(int numThreads);

public:
    FIR(Cgra *cgraHw, CgraArch *cgraArch, int size);

    ~FIR();

    DataFlow *createDataFlow(int thread);

    void benchmarking(int numThreads, int data_size);

    void printStatistics();

};


#endif //MAIN_FIR_H
