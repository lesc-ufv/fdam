//
// Created by lucas on 22/05/19.
//

#ifndef CHEBYSHEV_CHEBYSHEV_H
#define CHEBYSHEV_CHEBYSHEV_H

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

#define DATA_SIZE (1 << 26)
#define NUM_THREAD (8)
#define SAMPLES (100)

int chebyshev(int idx);

int chebyshev_openmp(int idx);

int chebyshev_cgra(int idx, int copies);

DataFlow *createDataFlow(int id, int copies);

int main(int argc, char *argv[]);

#endif //CHEBYSHEV_CHEBYSHEV_H
