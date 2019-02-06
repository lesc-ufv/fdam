//
// Created by lucas on 31/01/19.
//

#ifndef MAIN_SOBEL_H
#define MAIN_SOBEL_H

#include <chrono>
#include <fdam/cgra/cgra.h>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/cgra_arch.h>
#include <fdam/cgra/scheduler.h>
#include <fdam/cgra/scheduler_defs.h>

using namespace std::chrono;

#define NUM_THREADS 8

#define SOBEL_OP_SIZE 9
#define pow2(x) (x)*(x)
typedef int byte;

class SobelFilter {

private:
    Cgra *cgraHw;
    CgraArch *cgraArch;
    double schedulingTime;
    double cpuExecTime;
    double cgraExecTime;
    double cgraConfTime;

    void runCGRA(byte **gray, byte **contour_img, int width, int gray_size, int num_img);

    void runCPU(byte **gray, byte **contour_img, int width, int gray_size, int num_img);

    void rgbToGray(byte *rgb, byte *gray, int gray_size);

    void makeOpMemCGRA(byte *buffer, int buffer_size, int width, short **op_mem);

    void readFile(char *file_name, byte *buffer, int buffer_size);

    void writeFile(char *file_name, byte *buffer, int buffer_size);

    void itConv(byte *buffer, int buffer_size, int width, int *op, byte *res);

    int convolution(byte *X, byte *Y, int c_size);

    void makeOpMemCPU(const byte *buffer, int buffer_size, int width, int cindex, byte *op_mem);

    void contour(const byte *sobel_h, const byte *sobel_v, int gray_size, byte *contour_img);

    DataFlow *createDataFlow(int id);

    void compile(int numThreads);

public:
    SobelFilter(Cgra *cgraHw, CgraArch *cgraArch);

    ~SobelFilter();

    void benchmarking(int numThreads, int img_width, int img_height);

    void printStatistics();
};


#endif //MAIN_SOBEL_H
