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

using namespace std::chrono;

#define NUM_THREADS 8

class FIR {
private:
    Cgra * cgraHw;
    CgraArch *cgraArch;
    int size;
    unsigned short **coef;
    double schedulingTime;
    double cpuExecTime;
    double cgraExecTime;
    double cgraConfTime;

    void setCoef(unsigned short **coef);

    void runCGRA(unsigned short **data_in,unsigned short ** data_out,int data_size,int numThreads);

    void runCPU(unsigned short **data_in,unsigned short ** data_out,int data_size,int numThreads);

    DataFlow *createDataFlow(int thread);

    void compile(int numThreads);

public:
    FIR(Cgra *cgraHw, CgraArch *cgraArch, int size);

    ~FIR();

    void benchmarking(int numThreads);

    void printStatistics();

};


#endif //MAIN_FIR_H
