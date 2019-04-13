//
// Created by lucas on 11/04/19.
//

#ifndef MAIN_POLY6_H
#define MAIN_POLY6_H

#include <chrono>
#include <fdam/cgra/cgra.h>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/cgra_arch.h>
#include <fdam/cgra/scheduler.h>
#include <fdam/cgra/scheduler_defs.h>
#include <common.h>
#include <conf.h>

using namespace std::chrono;

class Poly6 {
private:
    Cgra *cgraHw;
    CgraArch *cgraArch;
    double schedulingTime;
    double cpuExecTime;
    double cgraExecTime;
    double cgraConfTime;
public:
    Poly6(Cgra *cgra, CgraArch *cgraArch);

    ~Poly6();

    DataFlow *createDataFlow(int id, int copies);
};


#endif //MAIN_POLY6_H
