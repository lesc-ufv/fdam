//
// Created by lucas on 12/28/17.
//
#include "AFUManager.h"

AFUManager::AFUManager() {

    AFUManager::fpga = new OPAE_SVC_WRAPPER(UUID_AFU_MANAGER);
    assert(AFUManager::fpga->isOk());
    AFUManager::csrs = new CSR_MGR(*AFUManager::fpga);
    AFUManager::readInfoHwAfu();
    assert(AFUManager::numAFUs > 0);
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
    for (unsigned long i : inf) {
        for (int j = 0; j < 8; ++j) {
            AFUManager::afuInfo[index] = (int) ((i >> j * 8) & 0xFF);
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

AFU *AFUManager::getAFU(afuid_t id) {
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
    return AFUManager::fpga->allocBuffer(numBytes);
}

void AFUManager::fpgaFreeBuffer(void *ptr) {
    AFUManager::fpga->freeBuffer(ptr);
}

void AFUManager::createAFUs() {
    for (int i = 0; i < AFUManager::numAFUs; ++i) {
        int numInputBuffers = afuInfo[i * 2];
        int numOutputBuffers = afuInfo[i * 2 + 1];
        auto id = (afuid_t) i;
        AFUManager::AFUs[id] = new AFU(*this, id, numInputBuffers, numOutputBuffers);
    }
}

void AFUManager::clear() {
    for (auto &it : AFUManager::AFUs)
        delete (it.second);

    delete AFUManager::csrs;
    delete AFUManager::fpga;
}

void AFUManager::startAFUs(uint64_t startAfus) {
    AFUManager::writeCSR(REG_START_AFUs, startAfus);
}

void AFUManager::stopAFUs(uint64_t stopAfus) {
    AFUManager::writeCSR(REG_STOP_AFUs, stopAfus);
}

void AFUManager::resetAFUs(uint64_t resetAfus) {
    AFUManager::writeCSR(REG_RESET_AFUs, resetAfus);
}

void AFUManager::waitAllDone(int64_t timeWaitMax) {
    bool flagNoMaxTime = false;
    struct timespec pause{};
    pause.tv_sec = 0;
    pause.tv_nsec = 500000;
    if (timeWaitMax == 0) {
        flagNoMaxTime = true;
        timeWaitMax = 1;
    }
    timeWaitMax *= 2;
    while (timeWaitMax > 0 && !AFUManager::isDoneAll()) {
        if (!flagNoMaxTime)timeWaitMax--;
        nanosleep(&pause, nullptr);
    };
}

bool AFUManager::isDoneAll() {
    for (auto &afu : AFUManager::AFUs)
        if(!afu.second->isDone())
            return false;
    return true;
}

int AFUManager::getNumAFUs() const {
    return numAFUs;
}

const map<afuid_t, AFU *> &AFUManager::getAFUs() const {
    return AFUs;
}

int AFUManager::getNumInputBuffers() const {
    return numInputBuffers;
}

int AFUManager::getNumOutputBuffers() const {
    return numOutputBuffers;
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
    MSG("Total read bytes: " << CL(total_cl_rd));
    MSG("Total write bytes: " <<  CL(total_cl_wr));
    MSG("Total Clock cicles: " << std::dec << total_cicles);
    MSG("AFUManager frequency: " << std::dec << AFUManager::csrs->getAFUMHz() << " MHz");
    MSG("AFUManager TARGET:" << (AFUManager::fpga->hwIsSimulated() ? " [simulated]" : "[real device]"));
    MSG("Transfer speed:" << speed << " GB/s");

    // MFP VTP (virtual to physical) statistics
    if (mpfVtpIsAvailable(AFUManager::fpga->mpf_handle)) {
        mpf_vtp_stats vtp_stats{};
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
        MSG("AFU " << i << ": Input queues: " << AFUManager::afuInfo[i] << " Output queues: " << AFUManager::afuInfo[i + 1]);
    }
    END_COLOR();
}


