#ifndef SW_ACCELERATOR_H
#define SW_ACCELERATOR_H


#include <cstdint>
#include <cstring>
#include <cmath>
#include <AccManagementDEFS.h>

class Accelerator {

private:
    AccManagement_t &accManagement;

    accid_t id;

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

    void sendConfIn(uint8_t idQueue);

    void sendConfOut(uint8_t idQueue);


public:
    Accelerator(AccManagement_t &accManagement, accid_t id, int numInputQueue, int numOutputQueue);

    ~Accelerator();

    void start();

    void stop();

    void reset();

    bool createInputQueue(uint8_t idQueue, size_t nBytes, void *dataToCopy);

    bool createOutputQueue(uint8_t idQueue, size_t nBytes);

    void *getInputQueue(uint8_t idQueue);

    void *getOutputQueue(uint8_t idQueue);

    void setInputQueue(uint8_t idQueue, void *ptrQueue, size_t numBytes);

    void setOutputQueue(uint8_t idQueue, void *ptrQueue, size_t numBytes);

    bool copyFromInputQueue(uint8_t idQueue, void *data, size_t nBytes);

    bool copyFromOutputQueue(uint8_t idQueue, void *data, size_t nBytes);

    bool copyToInputQueue(uint8_t idQueue, void *data, size_t nBytes);

    bool copyToOutputQueue(uint8_t idQueue, void *data, size_t nBytes);

    bool setNumBytesRead(uint8_t idQueue, size_t nBytes);

    bool setNumBytesWrite(uint8_t idQueue, size_t nBytes);

    size_t getSizeOfInputQueue(uint8_t idQueue) const;

    size_t getSizeOfOutputQueue(uint8_t idQueue) const;

    int getNumInputQueue() const;

    int getNumOutputQueue() const;

    size_t getNumBytesRead(uint8_t idQueue) const;

    size_t getNumBytesWrite(uint8_t idQueue) const;

    bool isDoneInputQueue(uint8_t idQueue) const;

    bool isDoneOutputQueue(uint8_t idQueue) const;

    void waitDone(int64_t timeWaitMax);

    bool isDone() const;

    accid_t getId() const;

    void printInf();

    void clear();

    int getInputQueueGlobalID(uint8_t idQueue);

    int getOutputQueueGlobalID(uint8_t idQueue);

    Accelerator &operator=(const Accelerator &Accelerator);
};


#endif //SW_ACCELERATOR_H
