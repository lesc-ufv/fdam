//
// Created by lucas on 22/05/19.
//

#ifndef FIR_FIR_H
#define FIR_FIR_H

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/scheduler.h>
#include <fdam/cgra/cgra.h>
#include <fdam/cgra/scheduler_defs.h>

using namespace std;
using namespace std::chrono;

#define DATA_SIZE (1 << 23)
#define NUM_THREAD (8)
#define SAMPLES (100)
#define TAPS 63


int fir(int idx, unsigned short *coef, int taps);

int fir_openmp(int idx, unsigned short *coef, int taps);

int fir_cgra(int idx, int copies, unsigned short *coef,int taps);

DataFlow *createDataFlow(int id, int copies, unsigned short *coef, int taps);

int main(int argc, char *argv[]);

#endif //FIR_FIR_H
