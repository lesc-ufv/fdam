//
// Created by lucas on 12/28/17.
//

#ifndef SW_AFU_MANAGER_H
#define SW_AFU_MANAGER_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <cmath>

#include "opae_svc_wrapper.h"
#include "csr_mgr.h"
#include "AFUManagerDEFS.h"
#include "AFU.h"


using namespace std;

class AFUManager {

private:
    OPAE_SVC_WRAPPER *fpga;
    CSR_MGR *csrs;
    int numInputBuffers;
    int numOutputBuffers;
    int numAFUs;
    map<afuid_t, AFU *> AFUs;

    void readInfoHwAfu();
    void createAFUs();

public:
    int afuInfo[AFU_INF_SIZE];

    explicit AFUManager();

    ~AFUManager();

    void clear();

    int getNumInputBuffers() const;

    int getNumOutputBuffers() const;

    int getNumAFUs() const;

    const map<afuid_t, AFU *> &getAFUs() const;

    void writeCSR(uint32_t regID, uint64_t val);

    uint64_t readCSR(uint32_t regID);

    void *fpgaAllocBuffer(size_t numBytes);

    void fpgaFreeBuffer(void *ptr);

    void startAFUs(uint64_t startAfus);

    void stopAFUs(uint64_t stopAfus);

    void resetAFUs(uint64_t resetAfus);

    void waitAllDone(int64_t timeWaitMax);

    bool AFUIsSimulated();

    AFU *getAFU(afuid_t id);

    bool isDoneAll();

    void printStatics();

    void printInfoAFUManager();
};

#endif //SW_AFU_MANAGER_H

