//
// Created by lucas on 22/05/19.
//

#ifndef PAETH_PAETH_H
#define PAETH_PAETH_H

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

#define DATA_SIZE (1 << 24)
#define NUM_THREAD (8)
#define SAMPLES (100)

int paeth(int idx);

int paeth_openmp(int idx);

int paeth_cgra(int idx, int copies);

DataFlow *createDataFlow(int id, int copies);

int main(int argc, char *argv[]);

#endif //PAETH_PAETH_H
