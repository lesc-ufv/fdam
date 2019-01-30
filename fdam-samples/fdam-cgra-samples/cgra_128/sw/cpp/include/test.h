//
// Created by lucas on 14/01/19.
//

#ifndef MAIN_TEST_H
#define MAIN_TEST_H

#include <fdam/cgra/cgra.h>
#include <fdam/cgra/cgra_arch.h>
#include <fdam/cgra/scheduler.h>
#include <samples.h>
#include <common.h>

typedef unsigned short byte;

class Test {
public:
    static void runLoopBack();

    static void runFir();

    static void runVetSum();

    static void runKmeans();

    static void
    helpKmeansUpdateCentroids(short **data_in, short *data_out, short *centroids, int k, int d, int data_size);

    static void helpKmeansUpdateConstants(cgra_program_t *cgra_program, int thread, short *constants, int k, int d);

    static int runSobelFilter();

    static int sobelFilter(byte *rgb, byte **gray, byte **contour_img, int width, int height);

    static int rgbToGray(byte *rgb, byte **gray, int buffer_size);

    static void makeOpMem(byte *buffer, int buffer_size, int width, short **op_mem);

    static void readFile(char *file_name, byte **buffer, int buffer_size);

    static void writeFile(char *file_name, byte *buffer, int buffer_size);
};


#endif //MAIN_TEST_H
