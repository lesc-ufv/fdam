#ifndef SW_ACCELERATOR_MANAGER_H
#define SW_ACCELERATOR_MANAGER_H

#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <opae_svc_wrapper.h>
#include <csr_mgr.h>
#include <AccManagementDEFS.h>
#include <Accelerator.h>

using namespace std;

class AccManagement {

private:

    OPAE_SVC_WRAPPER *fpga;

    CSR_MGR *csrs;

    int accInfo[ACC_INF_SIZE];

    int numInputQueue;

    int numOutputQueue;

    int numAccelerators;

    std::map<accid_t ,Accelerator *> accelerators;

    void createAccelerators();

    void setNumInputQueue(int numInputQueue);

    void setNumOutputQueue(int numOutputQueue);

    void setNumAccelerators(int numAccelerators);

    void getInfoHw();

public:

    explicit AccManagement();

    ~AccManagement();

    void clear();

    void writeCSR(uint32_t regID, uint64_t val);

    uint64_t readCSR(uint32_t regID);

    int getNumInputQueue() const;

    int getNumOutputQueue() const;

    int getNumAccelerators() const;

    void *fpgaAllocQueue(size_t numBytes);

    void fpgaFreeQueue(void *ptr);

    void startAccelerators(uint64_t accelerators);

    void stopAccelerators(uint64_t accelerators);

    void resetAccelerators(uint64_t accelerators);

    void waitDone(uint64_t accelerators, int64_t timeWaitMax);

    bool isSimulated();

    Accelerator &getAccelerator(accid_t id);

    const std::map<accid_t ,Accelerator *> &getAccelerators() const;

    bool isAccDone(uint64_t accelerators);

    void printHwInfo();
};

#endif //SW_ACCELERATOR_MANAGER_H

