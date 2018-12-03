#ifndef SW_ACCELERATOR_H
#define SW_ACCELERATOR_H


#include <cstdint>
#include <cstring>
#include <cmath>
#include <fdam/acc/acc_management_defs.h>

class Accelerator {

private:
    AccManagement_t &accManagement;

    unsigned short id;

    void *ptrDsm;

    size_t dsmSize;

    int numInputQueue;

    int numOutputQueue;

    void **inputQueue;

    void **outputQueue;

    size_t *sizeOfInputQueues;

    size_t *sizeOfOutputQueues;

private:

    void createDsm();

    void *getDsm() const;

    void sendConfDsm();

    void clearDsm();

    size_t getDsmSize() const;

    void setDsmSize(size_t size);

    void sendConfIn(unsigned char idQueue);

    void sendConfOut(unsigned char idQueue);

    int getInputQueueGlobalID(unsigned char idQueue);

    int getOutputQueueGlobalID(unsigned char idQueue);

    Accelerator &operator=(const Accelerator &Accelerator);

public:
    Accelerator(AccManagement_t &accManagement, unsigned short id, int numInputQueue, int numOutputQueue);

    ~Accelerator();

    void start();

    void stop();

    void reset();

    void *getInputQueue(unsigned char idQueue);

    void *getOutputQueue(unsigned char idQueue);

    void setInputQueue(unsigned char idQueue, void *ptrQueue, long long numBytes);

    void setOutputQueue(unsigned char idQueue, void *ptrQueue, long long numBytes);

    bool createInputQueue(unsigned char idQueue, long long nBytes);

    bool createOutputQueue(unsigned char idQueue, long long nBytes);

    bool copyToInputQueue(unsigned char idQueue, const signed char *data, long long ndata);

    bool copyToInputQueue(unsigned char idQueue, const short *data, long long ndata);

    bool copyToInputQueue(unsigned char idQueue, const int *data, long long ndata);

    bool copyToInputQueue(unsigned char idQueue, const long long *data, long long ndata);

    bool copyFromInputQueue(unsigned char idQueue, signed char *data, long long ndata);

    bool copyFromInputQueue(unsigned char idQueue, short *data, long long ndata);

    bool copyFromInputQueue(unsigned char idQueue, int *data, long long ndata);

    bool copyFromInputQueue(unsigned char idQueue, long long *data, long long ndata);

    bool copyToOutputQueue(unsigned char idQueue, const signed char *data, long long ndata);

    bool copyToOutputQueue(unsigned char idQueue, const short *data, long long ndata);

    bool copyToOutputQueue(unsigned char idQueue, const int *data, long long ndata);

    bool copyToOutputQueue(unsigned char idQueue, const long long *data, long long ndata);

    bool copyFromOutputQueue(unsigned char idQueue, signed char *data, long long ndata);

    bool copyFromOutputQueue(unsigned char idQueue, short *data, long long ndata);

    bool copyFromOutputQueue(unsigned char idQueue, int *data, long long ndata);

    bool copyFromOutputQueue(unsigned char idQueue, long long *data, long long ndata);

    bool setNumBytesRead(unsigned char idQueue, long long nBytes);

    bool setNumBytesWrite(unsigned char idQueue, long long nBytes);

    long long getSizeOfInputQueue(unsigned char idQueue) const;

    long long getSizeOfOutputQueue(unsigned char idQueue) const;

    int getNumInputQueue() const;

    int getNumOutputQueue() const;

    size_t getNumBytesRead(unsigned char idQueue) const;

    size_t getNumBytesWrite(unsigned char idQueue) const;

    bool isDoneInputQueue(unsigned char idQueue) const;

    bool isDoneOutputQueue(unsigned char idQueue) const;

    void waitDone(long long timeWaitMax);

    bool isDone() const;

    unsigned short getId() const;

    long getInputQueueBytesRead(unsigned char idQueue);

    long getOutputQueueWrittenBytes(unsigned char idQueue);

    void printHwInfo();

    void clear();
};


#endif //SW_ACCELERATOR_H
