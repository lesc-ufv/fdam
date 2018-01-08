//
// Created by lucas on 12/28/17.
//
#include "AFUManager.h"

AFUManager::AFUManager(const char *accel_uuid) {

    AFUManager::fpga = new OPAE_SVC_WRAPPER(accel_uuid);
    assert(AFUManager::fpga->isOk());
    AFUManager::csrs = new CSR_MGR(*AFUManager::fpga);
    AFUManager::readInfoHwAfu();
    assert(AFUManager::numAFUs > 0);
    AFUManager::createWorkspace();
    AFUManager::createAFUs();
}

AFUManager::~AFUManager() {
    AFUManager::clear();
}

void AFUManager::readInfoHwAfu() {
    uint64_t inf[8];
    inf[0] = AFUManager::readCSR(REG_INF_1);
    inf[1] = AFUManager::readCSR(REG_INF_2);
    inf[2] = AFUManager::readCSR(REG_INF_3);
    inf[3] = AFUManager::readCSR(REG_INF_4);
    inf[4] = AFUManager::readCSR(REG_INF_5);
    inf[5] = AFUManager::readCSR(REG_INF_6);
    inf[6] = AFUManager::readCSR(REG_INF_7);
    inf[7] = AFUManager::readCSR(REG_INF_8);
    int index = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            AFUManager::afuInfo[index] = (int) ((inf[i] >> j * 8) & 0xFF);
            index++;
        }
    }
    AFUManager::numAFUs = 0;
    AFUManager::numInputBuffers = 0;
    AFUManager::numOutputBuffers = 0;
    for (int k = 0; k < AFU_INF_SIZE; k += 2) {
        if (AFUManager::afuInfo[k] != 0 && AFUManager::afuInfo[k + 1] != 0) {
            AFUManager::numAFUs++;
        }
        AFUManager::numInputBuffers += AFUManager::afuInfo[k];
        AFUManager::numOutputBuffers += AFUManager::afuInfo[k + 1];
    }

}

AFU *AFUManager::getAFU(afu_id id) {
    if (id < AFUManager::numAFUs) {
        return AFUManager::AFUs.at(id);
    } else {
        BEGIN_COLOR(RED);
        MSG("AFU ID invalid!");
        END_COLOR();
        return nullptr;
    }
}

void AFUManager::writeCSR(uint32_t regID, uint64_t val) {
    AFUManager::csrs->writeCSR(regID, val);
}

uint64_t AFUManager::readCSR(uint32_t regID) {
    return AFUManager::csrs->readCSR(regID);
}

bool AFUManager::AFUIsSimulated() {
    return AFUManager::fpga->hwIsSimulated();
}

void *AFUManager::fpgaAllocBuffer(size_t numBytes) {
    AFUManager::commitedWorkspace = false;
    return AFUManager::fpga->allocBuffer(numBytes);
}

void AFUManager::fpgaFreeBuffer(void *ptr) {
    AFUManager::commitedWorkspace = false;
    AFUManager::fpga->freeBuffer(ptr);
}

void AFUManager::createWorkspace() {
    int num_cl_conf_in = (int) (2 * std::ceil((double) AFUManager::numInputBuffers / 8));
    int num_cl_conf_out = (int) (2 * std::ceil((double) AFUManager::numOutputBuffers / 8));
    AFUManager::numClConfs = num_cl_conf_in + num_cl_conf_out;
    AFUManager::numClDSM = 1 + (AFUManager::numClConfs / 2);
    size_t workspaceSize = (size_t) CL(AFUManager::numClConfs + AFUManager::numClDSM);
    uint64_t size = (uint64_t) AFUManager::numClDSM;
    size = size << 16L;
    size |= AFUManager::numClConfs;

    AFUManager::workspace = (uint64_t *) AFUManager::fpgaAllocBuffer(workspaceSize);
    assert(NULL != AFUManager::workspace);
    memset(AFUManager::workspace, 0x00, workspaceSize);

    AFUManager::dsm = (uint64_t *) (((char *) AFUManager::workspace) + CL(AFUManager::numClConfs));
    AFUManager::writeCSR(REG_ADDR_WORKSPACE_BASE, (uint64_t) intptr_t(AFUManager::workspace));
    AFUManager::writeCSR(REG_WORKSPACE_SIZE, size);
}

void AFUManager::createAFUs() {
    for (int i = 0; i < AFUManager::numAFUs; ++i) {
        int numInputBuffers = afuInfo[i * 2];
        int numOutputBuffers = afuInfo[i * 2 + 1];
        afu_id id = (afu_id) i;
        AFUManager::AFUs[id] = new AFU(*this, id, numInputBuffers, numOutputBuffers);
    }
}

void AFUManager::clear() {
    AFUManager::commitedWorkspace = false;
    AFUManager::writeCSR(REG_CFG, AFU_CONTROLLER_STOP);
    for (map<afu_id, AFU *>::iterator it = AFUManager::AFUs.begin(); it != AFUManager::AFUs.end(); ++it)
        delete (it->second);

    AFUManager::fpgaFreeBuffer((void *)AFUManager::workspace);
    delete AFUManager::csrs;
    delete AFUManager::fpga;
}

void AFUManager::commitWorkspace() {
    AFUManager::commitedWorkspace = false;
    AFUManager::updateWorkspace();
    AFUManager::writeCSR(REG_CFG, AFU_CONTROLLER_STOP);
    AFUManager::writeCSR(REG_CFG, AFU_CONTROLLER_UPDATE_WKP);
    int result = 1;
    struct timespec pause;
    pause.tv_sec = 0;
    pause.tv_nsec = 500000;//0.5 milis
    while (result != 0) {
        nanosleep(&pause, NULL);
        result = (int) (AFUManager::readCSR(REG_AFU_CONTROLLER_STATUS) & 1L);
    };
    AFUManager::writeCSR(REG_CFG, AFU_CONTROLLER_START);
    while (result == 0) {
        nanosleep(&pause, NULL);
        result = (int) (AFUManager::readCSR(REG_AFU_CONTROLLER_STATUS) & 1L);
    };
    AFUManager::commitedWorkspace = true;
}

bool AFUManager::workspaceIscommited() {
    return AFUManager::commitedWorkspace;
}

int AFUManager::getNumClConf() {
    return AFUManager::numClConfs;
}

int AFUManager::getNumClDSM() {
    return AFUManager::numClDSM;
}

void AFUManager::startAFUs(uint64_t startAfus) {
    if (!AFUManager::workspaceIscommited())
        AFUManager::commitWorkspace();
    AFUManager::writeCSR(REG_START_AFUs, startAfus);

}

void AFUManager::stopAFUs(uint64_t stopAfus) {
    AFUManager::writeCSR(REG_STOP_AFUs, stopAfus);
}

void AFUManager::waitAllDone(int64_t timeWaitMax) {
    uint64_t *done = &AFUManager::dsm[GET_INDEX((AFUManager::getNumClDSM() - 1), 0, 8)];
    bool flagDone = false;
    assert(done != nullptr);
    struct timespec pause;
    pause.tv_sec = 0;
    pause.tv_nsec = 500000;
    timeWaitMax *=2;
    while (timeWaitMax > 0 && !flagDone) {
        flagDone = (done[0] & 1L) != 0;
        timeWaitMax--;
        nanosleep(&pause, NULL);
    };
    AFUManager::stopAFUs(0L);
    AFUManager::setDoneAll(flagDone);

}

bool AFUManager::isDoneAll() {
    return AFUManager::flagDoneAll;
}

void AFUManager::setDoneAll(bool doneAll) {
    AFUManager::flagDoneAll = doneAll;
    for (map<afu_id, AFU *>::iterator it = AFUManager::AFUs.begin(); it != AFUManager::AFUs.end(); ++it) {
        AFU *afu = it->second;
        uint64_t *done = &AFUManager::dsm[GET_INDEX(AFUManager::getNumClDSM() - 1, 0, 8)];
        uint64_t afuDoneBit = (uint64_t) (1L << (it->first + 1L));
        bool flagDone = (done[0] & afuDoneBit) != 0L;
        afu->setDone(flagDone);
    }

}

int AFUManager::getNumAFUs() const {
    return numAFUs;
}

const map<afu_id, AFU *> &AFUManager::getAFUs() const {
    return AFUs;
}

int AFUManager::getNumInputBuffers() const {
    return numInputBuffers;
}

int AFUManager::getNumOutputBuffers() const {
    return numOutputBuffers;
}

void AFUManager::updateWorkspace() {
    AFU *afu;
    int idBufferGen;
    int row;
    int col;
    int workspaceIndexPointer;
    int workspaceIndexQtd;
    for (map<afu_id, AFU *>::iterator it = AFUManager::AFUs.begin(); it != AFUManager::AFUs.end(); ++it) {
        afu = it->second;
        for (int i = 0; i < afu->getNumInputBuffer(); ++i) {
            idBufferGen = (afu->getID() * afu->getNumInputBuffer() + i);
            row = 2*((idBufferGen - (idBufferGen % 8)) / 8);
            col = (idBufferGen % 8);
            workspaceIndexPointer = GET_INDEX(row, col, 8);
            workspaceIndexQtd = GET_INDEX((row+1), col, 8);
            AFUManager::workspace[workspaceIndexPointer] = (uint64_t) intptr_t(afu->getInputBuffer(i));
            AFUManager::workspace[workspaceIndexQtd] = (uint64_t) (afu->getSizeOfInputBuffer(i) / 64); //number in cache lines
        }
        for (int i = 0; i < afu->getNumOutputBuffer(); ++i) {
            idBufferGen = (afu->getID() * afu->getNumOutputBuffer() + i);
            row = (int) (2 * ((idBufferGen - (idBufferGen % 8)) / 8) + ((std::ceil((double)AFUManager ::getNumInputBuffers() / 8.0)) * 2));
            col = (idBufferGen % 8);
            workspaceIndexPointer = GET_INDEX(row, col, 8);
            workspaceIndexQtd = GET_INDEX((row+1), col, 8);
            AFUManager::workspace[workspaceIndexPointer] = (uint64_t) intptr_t(afu->getOutputBuffer(i));
            AFUManager::workspace[workspaceIndexQtd] = (uint64_t) (afu->getSizeOfOutputBuffer(i) / 64); //number in cache lines
        }
    }
}

void AFUManager::printStatics() {
    double total_cicles = AFUManager::readCSR(REG_CLOCK_COUNT);
    double total_cl_wr = AFUManager::readCSR(REG_CL_WR_COUNT);
    double total_cl_rd = AFUManager::readCSR(REG_CL_RD_COUNT);
    double totalBytesBuffers = CL(total_cl_wr + total_cl_rd);
    double cicle_time = 1.0 / (AFUManager::csrs->getAFUMHz() * 1000000.0);
    double time_spend = cicle_time * total_cicles;
    double speed = 0;
    if (time_spend != 0) {
        speed = totalBytesBuffers / time_spend;
        speed = speed / GB(1);
    }
    // Reads CSRs to get some statistics
    BEGIN_COLOR(GREEN);
    MSG("Total Clock cicles: " << std::dec << total_cicles);
    MSG("AFUManager frequency: " << std::dec << AFUManager::csrs->getAFUMHz() << " MHz");
    MSG("AFUManager TARGET:" << (AFUManager::fpga->hwIsSimulated() ? " [simulated]" : "[real device]"));
    MSG("Transfer speed:" << speed << " GB/s");

    // MFP VTP (virtual to physical) statistics
    if (mpfVtpIsAvailable(AFUManager::fpga->mpf_handle)) {
        mpf_vtp_stats vtp_stats;
        mpfVtpGetStats(AFUManager::fpga->mpf_handle, &vtp_stats);
        if (vtp_stats.numFailedTranslations) {
            MSG("VTP failed translating VA: 0x" << hex << uint64_t(vtp_stats.ptWalkLastVAddr) << dec);
        }
        MSG("VTP PT walk cycles: " << vtp_stats.numPTWalkBusyCycles);
        MSG("VTP L2 4KB hit / miss: " << vtp_stats.numTLBHits4KB << " / " << vtp_stats.numTLBMisses4KB);
        MSG("VTP L2 2MB hit / miss: " << vtp_stats.numTLBHits2MB << " / " << vtp_stats.numTLBMisses2MB);
    }
    END_COLOR();
}

void AFUManager::printInfoAFUManager() {
    BEGIN_COLOR(BLUE);
    MSG("AFUManager INFO:");
    MSG("NUM AFUS: " << numAFUs);
    for (int i = 0; i < numAFUs; ++i) {
        MSG("AFU " << i << ": Input Buffer: " << afuInfo[i] << " Output Buffer: " << afuInfo[i + 1]);
    }
    END_COLOR();
}

void AFUManager::printWorkspace() {
    for (int i = 0; i < AFUManager::numClConfs; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << std::hex << AFUManager::workspace[GET_INDEX(i, j, 8)] << " ";
        }
        cout << endl;
    }
}

void AFUManager::printDSM() {
    BEGIN_COLOR(GREEN);
    MSG("DSM:");
    for (int i = 0; i < AFUManager::numClDSM; ++i) {
        cout << "  [APP]  ";
        for (int j = 7; j >= 0; --j) {
            cout << std::hex << AFUManager::dsm[GET_INDEX(i, j, 8)] << " ";
        }
        cout << endl;
    }
    END_COLOR();
}



