//
// Created by lucas on 12/28/17.
//

#ifndef SW_AFU_H
#define SW_AFU_H


#include <cstdint>
#include <cstring>
#include "AFUManagerDEFS.h"
#include <exception>

class AFU {

private:
    AFUManager_t &afuManager;
    afu_id ID;
    int numInputBuffer;
    int numOutputBuffer;
    void **Inputbuffers;
    void **Outputbuffers;
    int *sizeOfInputBuffers;
    int *sizeOfOutputBuffers;
    bool done;

public:
    AFU(AFUManager_t &afuManager, afu_id id, int numInputBuffer, int numOutputBuffer);

    ~AFU();

    void start();

    void stop();
    
    bool createInputBufferSW(int BufferID, size_t nBytes, void *dataToCopy);

    bool createOutputBufferSW(int BufferID, size_t nBytes);

    void *getInputBuffer(int BufferID);

    void *getOutputBuffer(int BufferID);

    int getSizeOfInputBuffer(int BufferID);

    int getSizeOfOutputBuffer(int BufferID);

    void waitDone(int64_t timeWaitMax);

    bool isDone();

    void setDone(bool done);

    AFU &operator=(const AFU &AFU);

    void clear();

    int getNumInputBuffer() const;

    int getNumOutputBuffer() const;
    
    afu_id getID() const;

};


#endif //SW_AFU_H
