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
    afuid_t ID;
    int numInputBuffer;
    int numOutputBuffer;
    void **Inputbuffers;
    void **Outputbuffers;
    void *dsm;
    size_t  dsm_size;
    int *sizeOfInputBuffers;
    int *sizeOfOutputBuffers;

private:
    void createDSM();
    
    void sendConfDSM();
    
    void clearDSM();
    
    size_t getSizeDsm();
    
public:
    AFU(AFUManager_t &afuManager, afuid_t id, int numInputBuffer, int numOutputBuffer);

    ~AFU();

    void start();

    void stop();

    void reset();

    void sendConfIn(int BufferID);

    void sendConfOut(int BufferID);

    bool createInputBufferSW(int BufferID, size_t nBytes, void *dataToCopy);

    bool createOutputBufferSW(int BufferID, size_t nBytes);

    void *getInputBuffer(int BufferID);

    void *getOutputBuffer(int BufferID);

    void *getDsm();

    bool copyInputBuffer(int BufferID, void * data);

    bool copyOutputBuffer(int BufferID, void * data);

    int getSizeOfInputBuffer(int BufferID);

    int getSizeOfOutputBuffer(int BufferID);

    void waitDone(int64_t timeWaitMax);
    
    bool isDone();

    void clear();

    int getNumInputBuffer() const;

    int getNumOutputBuffer() const;
    
    afuid_t getID() const;
    
    void printDSM();
    
    int getInputBufferGlobalID(int BufferID);
    
    int getOutputBufferGlobalID(int BufferID);
    
    AFU &operator=(const AFU &AFU);
};


#endif //SW_AFU_H
