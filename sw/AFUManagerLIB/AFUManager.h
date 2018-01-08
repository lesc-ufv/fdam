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
    int numClConfs;
    int numClDSM;
    bool commitedWorkspace;
    map<afu_id, AFU *> AFUs;

    bool flagDoneAll;

    void readInfoHwAfu();

    void createWorkspace();

    void updateWorkspace();

    void createAFUs();

public:
    uint64_t *workspace;
    uint64_t *dsm;
    int afuInfo[AFU_INF_SIZE];

    AFUManager(const char *accel_uuid);

    ~AFUManager();

    void clear();

    int getNumInputBuffers() const;

    int getNumOutputBuffers() const;

    int getNumAFUs() const;

    const map<afu_id, AFU *> &getAFUs() const;

    void writeCSR(uint32_t regID, uint64_t val);

    uint64_t readCSR(uint32_t regID);

    void *fpgaAllocBuffer(size_t numBytes);

    void fpgaFreeBuffer(void *ptr);

    void commitWorkspace();

    void startAFUs(uint64_t startAfus);

    void stopAFUs(uint64_t stopAfus);

    void waitAllDone(int64_t timeWaitMax);

    bool AFUIsSimulated();

    AFU *getAFU(afu_id id);

    bool workspaceIscommited();

    bool isDoneAll();

    void setDoneAll(bool doneAll);

    int getNumClConf();

    int getNumClDSM();

    void printWorkspace();

    void printDSM();

    void printStatics();

    void printInfoAFUManager();
};

#endif //SW_AFU_MANAGER_H

