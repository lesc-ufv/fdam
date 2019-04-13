//
// Created by lucas on 11/04/19.
//

#ifndef MAIN_POLY8_H
#define MAIN_POLY8_H

#include <chrono>
#include <fdam/cgra/cgra.h>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/cgra_arch.h>
#include <fdam/cgra/scheduler.h>
#include <fdam/cgra/scheduler_defs.h>
#include <common.h>
#include <conf.h>

using namespace std::chrono;

class Poly8 {
private:
    Cgra *cgraHw;
    CgraArch *cgraArch;
    double schedulingTime;
    double cpuExecTime;
    double cgraExecTime;
    double cgraConfTime;

    std::vector<unsigned short> constants;

    void runCGRA(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies);

    void runCPU(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies);

public:
    Poly8(Cgra *cgra, CgraArch *cgraArch);

    ~Poly8();

    DataFlow *createDataFlow(int id, int copies);

    void benchmarking(int numThreads, int data_size);

    bool compile(int numThreads, int copies);

    void printStatistics();

    unsigned short pot(unsigned short num, unsigned short exp);
};


#endif //MAIN_POLY8_H
