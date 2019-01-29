//
// Created by lucas on 13/01/19.
//

#ifndef MAIN_SAMPLES_H
#define MAIN_SAMPLES_H

#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/input_stream.h>
#include <fdam/cgra/add.h>
#include <fdam/cgra/output_stream.h>
#include <fdam/cgra/sub.h>
#include <fdam/cgra/abs.h>
#include <fdam/cgra/pass_a.h>
#include <fdam/cgra/slt.h>
#include <fdam/cgra/mux.h>
#include <fdam/cgra/pass_b.h>
#include <fdam/cgra/mult.h>
#include <fdam/cgra/max.h>
#include <fdam/cgra/and.h>

class Samples {

public:
    static DataFlow *loopback(int id, int pipe, int copies);

    static DataFlow *vetorialSum(int id, int copies);

    static DataFlow *fir(int id, short **coef, int n, int copies);

    static DataFlow *kmeans(int id, int k, int d);

    static DataFlow *sobelFilter();

    static DataFlow *test();

};

#endif //MAIN_SAMPLES_H
