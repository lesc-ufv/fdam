//
// Created by lucas on 22/05/19.
//

#ifndef KMEANS_KMEANS_H
#define KMEANS_KMEANS_H

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include <algorithm>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/scheduler.h>
#include <fdam/cgra/cgra.h>
#include <fdam/cgra/scheduler_defs.h>

using namespace std;
using namespace std::chrono;

#define DATA_SIZE (1 << 23)
#define NUM_THREAD (8)
#define SAMPLES (100)

int kmeans(int idx, int num_clusters, int num_dim);

int kmeans_openmp(int idx, int num_clusters, int num_dim);

int kmeans_cgra(int idx, int num_clusters, int num_dim);

bool compare(Operator *a, Operator *b);

DataFlow *createDataFlow(int id,int num_clusters, int num_dim);

void helpKmeansUpdateConstants(cgra_program_t *cgra_program, int thread, unsigned short *constants, int num_clusters, int num_dim);

int main(int argc, char *argv[]);


#endif //KMEANS_KMEANS_H

