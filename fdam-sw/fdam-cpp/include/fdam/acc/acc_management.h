#ifndef SW_ACCELERATOR_MANAGER_H
#define SW_ACCELERATOR_MANAGER_H

#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <fdam/acc/opae_svc_wrapper.h>
#include <fdam/acc/csr_mgr.h>
#include <fdam/acc/acc_management_defs.h>
#include <fdam/acc/accelerator.h>


using namespace std;

class AccManagement {

private:

    OPAE_SVC_WRAPPER *fpga;

    CSR_MGR *csrs;

    int accInfo[ACC_INF_SIZE];

    int numInputQueue;

    int numOutputQueue;

    int numAccelerators;

    std::map<unsigned short, Accelerator *> accelerators;

    void createAccelerators();

    void setNumInputQueue(int numInputQueue);

    void setNumOutputQueue(int numOutputQueue);

    void setNumAccelerators(int numAccelerators);

    void getInfoHw();

public:

    explicit AccManagement();

    ~AccManagement();

    void clear();

    void writeCSR(unsigned int regID, unsigned long val);

    unsigned long readCSR(unsigned int regID);

    int getNumInputQueue() const;

    int getNumOutputQueue() const;

    int getNumAccelerators() const;

    void *accAllocQueue(long long numBytes);

    void accFreeQueue(void *ptr);

    void startAccelerators(unsigned long accelerators);

    void stopAccelerators(unsigned long accelerators);

    void resetAccelerators(unsigned long accelerators);

    void waitDone(unsigned long accelerators, long timeWaitMax);

    bool isSimulated();

    Accelerator &getAccelerator(unsigned short id);

    const std::map<unsigned short, Accelerator *> &getAccelerators() const;

    bool isAccDone(unsigned long accelerators);

    long getTotalClocks();
    
    long getTotalWrittenCacheLine();
    
    long getTotalCacheLineRead();

    long getClockSpeed();
    
    bool IsSimulated();
    
    void printHwInfo();
};

#endif //SW_ACCELERATOR_MANAGER_H

