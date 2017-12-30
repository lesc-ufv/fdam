//
// Created by lucas on 12/28/17.
//

#ifndef SW_SUBAFU_H
#define SW_SUBAFU_H


#include <cstdint>
#include <cstring>
#include "AFU_DEFS.h"
#include <exception>

class SubAFU {

private:
    Afu &afu;
    subafu_id ID;
    int numInputBuffer;
    int numOutputBuffer;
    bool done;
public:
    SubAFU(Afu &afu, subafu_id id, int numInputBuffer, int numOutputBuffer);

    ~SubAFU();

    void start();

    void stop();

    void *createInputBufferSW(int indexInputBuffer, size_t nBytes, void *dataToCopy);

    void *createOutputBufferSW(int indexInputBuffer, size_t nBytes);

    void waitDone(int64_t timeWaitMax);

    bool isDone();

    void setDone(bool done);

    SubAFU& operator=(const SubAFU& subAFU);

    void clear();

};


#endif //SW_SUBAFU_H
