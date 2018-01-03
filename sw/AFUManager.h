//
// Created by lucas on 12/28/17.
//

#ifndef SW_AFU_H
#define SW_AFU_H

#include <iostream>
#include <map>
#include "opae_files/opae_svc_wrapper.h"
#include "opae_files/csr_mgr.h"
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

public:
    AFUManager(const char *accel_uuid);

    ~AFUManager();

    uint64_t *workspace;
    uint64_t *dsm;
    int afuInfo[AFU_INF_SIZE];

    void commitWorkspace();

    void printStatics();

    void printInfoAFU();

    void writeCSR(uint32_t regID, uint64_t val);

    uint64_t readCSR(uint32_t regID);

    void *fpgaAllocBuffer(size_t numBytes);

    void fpgaFreeBuffer(void *ptr);

    bool AFUIsSimulated();

    AFU *getAFU(afu_id id);

    bool workspaceIscommited();

    void printWorkspace();

    void printDSM();

    int getNumClConf();

    int getNumClDSM();

private:
    void readInfoHwAfu();

    void createWorkspace();

    void createAFUs();

    void clear();
};

#endif //SW_AFU_H

