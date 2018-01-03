//
// Created by lucas on 12/28/17.
//

#ifndef SW_SUBAFU_H
#define SW_SUBAFU_H


#include <cstdint>
#include <cstring>
#include "AFUManagerDEFS.h"
#include <exception>

class AFU {

private:
    Afu &afu;
    afu_id ID;
    int numInputBuffer;
    int numOutputBuffer;
    bool done;
public:
    AFU(Afu &afu, afu_id id, int numInputBuffer, int numOutputBuffer);

    ~AFU();

    void start();

    void stop();

    void *createInputBufferSW(int indexInputBuffer, size_t nBytes, void *dataToCopy);

    void *createOutputBufferSW(int indexInputBuffer, size_t nBytes);

    void waitDone(int64_t timeWaitMax);

    bool isDone();

    void setDone(bool done);

    AFU& operator=(const AFU& subAFU);

    void clear();

};


#endif //SW_SUBAFU_H
