//
// Created by lucas on 08/04/19.
//

#ifndef MAIN_MIBENCH_H
#define MAIN_MIBENCH_H

#include <chrono>
#include <fdam/cgra/cgra.h>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/cgra_arch.h>
#include <fdam/cgra/scheduler.h>
#include <fdam/cgra/scheduler_defs.h>
#include <common.h>
#include <conf.h>

using namespace std::chrono;

class Mibench {
private:
    Cgra *cgraHw;
    CgraArch *cgraArch;
    double schedulingTime;
    double cpuExecTime;
    double cgraExecTime;
    double cgraConfTime;
public:
    Mibench(Cgra *cgra, CgraArch *cgraArch);

    ~Mibench();

    DataFlow *createDataFlow(int id);
};


#endif //MAIN_MIBENCH_H
