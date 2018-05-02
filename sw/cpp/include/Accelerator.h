#ifndef SW_ACCELERATOR_H
#define SW_ACCELERATOR_H


#include <cstdint>
#include <cstring>
#include <cmath>
#include <AccManagementDEFS.h>

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

    void createDsm();

    void *getDsm() const;

    void sendConfDsm();

    void clearDsm();

    size_t getDsmSize() const;

    void setDsmSize(size_t size);

    void sendConfIn(unsigned char idQueue);

    void sendConfOut(unsigned char idQueue);


public:
    Accelerator(AccManagement_t &accManagement, unsigned short id, int numInputQueue, int numOutputQueue);

    ~Accelerator();

    void start();

    void stop();

    void reset();

    bool createInputQueue(unsigned char idQueue, size_t nBytes, void *dataToCopy);

    bool createOutputQueue(unsigned char idQueue, size_t nBytes);

    void *getInputQueue(unsigned char idQueue);

    void *getOutputQueue(unsigned char idQueue);

    void setInputQueue(unsigned char idQueue, void *ptrQueue, size_t numBytes);

    void setOutputQueue(unsigned char idQueue, void *ptrQueue, size_t numBytes);

    bool copyFromInputQueue(unsigned char idQueue, void *data, size_t nBytes);

    bool copyFromOutputQueue(unsigned char idQueue, void *data, size_t nBytes);

    bool copyToInputQueue(unsigned char idQueue, void *data, size_t nBytes);

    bool copyToOutputQueue(unsigned char idQueue, void *data, size_t nBytes);

    bool setNumBytesRead(unsigned char idQueue, size_t nBytes);

    bool setNumBytesWrite(unsigned char idQueue, size_t nBytes);

    size_t getSizeOfInputQueue(unsigned char idQueue) const;

    size_t getSizeOfOutputQueue(unsigned char idQueue) const;

    int getNumInputQueue() const;

    int getNumOutputQueue() const;

    size_t getNumBytesRead(unsigned char idQueue) const;

    size_t getNumBytesWrite(unsigned char idQueue) const;

    bool isDoneInputQueue(unsigned char idQueue) const;

    bool isDoneOutputQueue(unsigned char idQueue) const;

    void waitDone(long timeWaitMax);

    bool isDone() const;

    unsigned short getId() const;

    void printInfo();

    void clear();

    int getInputQueueGlobalID(unsigned char idQueue);

    int getOutputQueueGlobalID(unsigned char idQueue);

    Accelerator &operator=(const Accelerator &Accelerator);
};


#endif //SW_ACCELERATOR_H
