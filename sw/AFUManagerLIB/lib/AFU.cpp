//
// Created by lucas on 12/28/17.
//

#include <cmath>
#include "AFU.h"
#include "AFUManager.h"

AFU::AFU(AFUManager_t &afuManager, afuid_t id, int numInputBuffer, int numOutputBuffer) : afuManager(afuManager),
                                                                                          ID(id),
                                                                                          numInputBuffer(
                                                                                                  numInputBuffer),
                                                                                          numOutputBuffer(
                                                                                                  numOutputBuffer) {

    AFU::Inputbuffers = new void *[numInputBuffer];
    AFU::Outputbuffers = new void *[numOutputBuffer];
    AFU::sizeOfInputBuffers = new int[numInputBuffer];
    AFU::sizeOfOutputBuffers = new int[numOutputBuffer];

    for (int i = 0; i < numInputBuffer; ++i) {
        Inputbuffers[i] = nullptr;
    }
    for (int i = 0; i < numOutputBuffer; ++i) {
        Outputbuffers[i] = nullptr;
    }
    AFU::createDSM();
    AFU::sendConfDSM();
}

AFU::~AFU() {
    AFU::clear();
}

void AFU::start() {
    AFU::clearDSM();
    AFU::afuManager.startAFUs((uint64_t) (1L << AFU::ID));
}

void AFU::stop() {
    AFU::afuManager.stopAFUs((uint64_t) (1L << AFU::ID));
}

void AFU::reset() {
    AFU::clearDSM();
    AFU::afuManager.resetAFUs((uint64_t) (1L << AFU::ID));
    AFU::afuManager.resetAFUs(0);
}

void AFU::sendConfIn(int BufferID) {
    int bufferIdGlobal = AFU::getInputBufferGlobalID(BufferID);
    auto numCl = static_cast<uint64_t>(AFU::getSizeOfInputBuffer(BufferID) / 64);
    auto val_low = static_cast<uint64_t>(numCl << 32 | bufferIdGlobal);
    auto val_high = static_cast<uint64_t>(intptr_t(AFU::getInputBuffer(BufferID)) / 64);
    AFU::afuManager.writeCSR(REG_CONF_IN_LOW, val_low);
    AFU::afuManager.writeCSR(REG_CONF_IN_HIGH, val_high);
}

void AFU::sendConfOut(int BufferID) {
    int bufferIdGlobal = AFU::getOutputBufferGlobalID(BufferID);
    auto numCl = static_cast<uint64_t>(AFU::getSizeOfOutputBuffer(BufferID) / 64);
    auto val_low = static_cast<uint64_t>(numCl << 32 | bufferIdGlobal);
    auto val_high = static_cast<uint64_t>(intptr_t(AFU::getOutputBuffer(BufferID)) / 64);
    AFU::afuManager.writeCSR(REG_CONF_OUT_LOW, val_low);
    AFU::afuManager.writeCSR(REG_CONF_OUT_HIGH, val_high);
}

void AFU::createDSM() {
    AFU::dsm_size = static_cast<size_t>(CL(1.0 + ceil(AFU::numInputBuffer / 16.0) + ceil(AFU::numOutputBuffer / 16.0)));
    AFU::dsm = AFU::afuManager.fpgaAllocBuffer(AFU::dsm_size);
}

void AFU::sendConfDSM() {

    int bufferIdGlobal = AFU::getID();
    auto numCl = static_cast<uint64_t>(AFU::getDsmSize() / 64);
    auto val_low = static_cast<uint64_t>(numCl << 32 | bufferIdGlobal);
    auto val_high = static_cast<uint64_t>(intptr_t(AFU::getDsm()) / 64);
    AFU::afuManager.writeCSR(REG_CONF_DSM_LOW, val_low);
    AFU::afuManager.writeCSR(REG_CONF_DSM_HIGH, val_high);
}

int  AFU::getDsmSize() {
    return AFU::dsm_size;
}

void AFU::clearDSM() {
    memset(AFU::getDsm(), 0, AFU::getDsmSize());
}

bool AFU::isDone() {
    auto dsmNumCL = AFU::dsm_size / 64;
    auto *done = (int *) AFU::dsm;
    return (done[GET_INDEX(dsmNumCL - 1, 0, 16)] & 1) == 1L;
}

bool AFU::isDoneInputBuffer(int BufferID){
    auto dsmNumCL = AFU::getDsmSize() / 64;
    auto *done = (int *)(AFU::getDsm());
    MSG(done[GET_INDEX(dsmNumCL - 1, 0, 16)]);
    return (done[GET_INDEX(dsmNumCL - 1, 0, 16)] & (2 << BufferID)) == (2 << BufferID);
}
    
bool AFU::isDoneOutputBuffer(int BufferID){
    auto dsmNumCL = AFU::getDsmSize() / 64;
    auto *done = (int *)(AFU::getDsm());
    return (done[GET_INDEX(dsmNumCL - 1, 0, 16)] & (2 << (BufferID+AFU::getNumInputBuffer()))) == (2 << (BufferID+AFU::getNumInputBuffer()));
}

void AFU::waitDone(int64_t timeWaitMax) {
    bool flagNoMaxTime = false;
    struct timespec pause{};
    pause.tv_sec = 0;
    pause.tv_nsec = 500000;
    if (timeWaitMax == 0) {
        flagNoMaxTime = true;
        timeWaitMax = 1;
    }
    timeWaitMax *= 2;
    while (timeWaitMax > 0 && !AFU::isDone()) {
        if (!flagNoMaxTime)timeWaitMax--;
        nanosleep(&pause, nullptr);
    };
    AFU::stop();
}

bool AFU::createInputBufferSW(int BufferID, size_t nBytes, void *dataToCopy) {
    if (BufferID >= 0 && BufferID < AFU::numInputBuffer) {
        if (AFU::Inputbuffers[BufferID] != nullptr) {
            return false;
        } else {
            double nb(nBytes);
            uint64_t nBytesAlign = ((uint64_t) (std::ceil(nb / 256))) * 256;
            AFU::Inputbuffers[BufferID] = AFU::afuManager.fpgaAllocBuffer(nBytesAlign);
            if (AFU::Inputbuffers[BufferID] != nullptr) {
                memset(AFU::Inputbuffers[BufferID], 0, nBytesAlign);
                AFU::sizeOfInputBuffers[BufferID] = (int) nBytesAlign;
                if (dataToCopy != nullptr) {
                    memcpy(AFU::Inputbuffers[BufferID], dataToCopy, nBytes);
                }
                AFU::sendConfIn(BufferID);
                return true;
            }
        }
    }
    return false;
}

bool AFU::createOutputBufferSW(int BufferID, size_t nBytes) {
    if (BufferID >= 0 && BufferID < AFU::numOutputBuffer) {
        if (AFU::Outputbuffers[BufferID] != nullptr) {
            return false;
        } else {
            double nb(nBytes);
            uint64_t nBytesAlign = ((uint64_t) (std::ceil(nb / 64))) * 64;
            AFU::Outputbuffers[BufferID] = AFU::afuManager.fpgaAllocBuffer(nBytes);
            if (AFU::Outputbuffers[BufferID] != nullptr) {
                memset(AFU::Outputbuffers[BufferID], 0, nBytesAlign);
                AFU::sizeOfOutputBuffers[BufferID] = (int) nBytesAlign;
                AFU::sendConfOut(BufferID);
                return true;
            }
        }
    }
    return false;
}

void AFU::clear() {

    delete AFU::sizeOfInputBuffers;
    delete AFU::sizeOfOutputBuffers;
    AFU::sizeOfInputBuffers = nullptr;
    AFU::sizeOfOutputBuffers = nullptr;
    for (int i = 0; i < AFU::numInputBuffer; ++i) {
        if (nullptr != AFU::Inputbuffers[i]) {
            AFU::afuManager.fpgaFreeBuffer(AFU::Inputbuffers[i]);
            AFU::Inputbuffers[i] = nullptr;
        }
    }
    for (int i = 0; i < AFU::numOutputBuffer; ++i) {
        if (nullptr != AFU::Outputbuffers[i]) {
            AFU::afuManager.fpgaFreeBuffer(AFU::Outputbuffers[i]);
            AFU::Outputbuffers[i] = nullptr;
        }
    }
}

int AFU::getNumInputBuffer() const {
    return numInputBuffer;
}

int AFU::getNumOutputBuffer() const {
    return numOutputBuffer;
}

void *AFU::getInputBuffer(int BufferID) {
    if (BufferID >= 0 && BufferID < AFU::numInputBuffer) {
        return AFU::Inputbuffers[BufferID];
    }
    return nullptr;
}

void *AFU::getOutputBuffer(int BufferID) {
    if (BufferID >= 0 && BufferID < AFU::numOutputBuffer) {
        return AFU::Outputbuffers[BufferID];
    }
    return nullptr;
}

afuid_t AFU::getID() const {
    return ID;
}

int AFU::getSizeOfInputBuffer(int BufferID) {
    if (BufferID >= 0 && BufferID < AFU::numInputBuffer) {
        return AFU::sizeOfInputBuffers[BufferID];
    }
    return 0;
}

int AFU::getSizeOfOutputBuffer(int BufferID) {
    if (BufferID >= 0 && BufferID < AFU::numOutputBuffer) {
        return AFU::sizeOfOutputBuffers[BufferID];
    }
    return 0;
}

bool AFU::copyFromInputBuffer(int BufferID, void *data, size_t nBytes) {
    if (BufferID >= 0 && BufferID < AFU::numInputBuffer) {
        if (AFU::Inputbuffers[BufferID] != nullptr) {
            memcpy(data, AFU::Inputbuffers[BufferID], nBytes);
            return true;
        }
    }
    return false;
}

bool AFU::copyFromOutputBuffer(int BufferID, void *data, size_t nBytes) {
    if (BufferID >= 0 && BufferID < AFU::numOutputBuffer) {
        if (AFU::Outputbuffers[BufferID] != nullptr) {
            memcpy(data, AFU::Outputbuffers[BufferID], nBytes);
            return true;
        }
    }
    return false;
}

bool AFU::copyToInputBuffer(int BufferID, void *data, size_t nBytes) {
    if (BufferID >= 0 && BufferID < AFU::numInputBuffer) {
        if (AFU::Inputbuffers[BufferID] != nullptr) {
            memcpy(AFU::Inputbuffers[BufferID],data,nBytes);
            return true;
        }
    }
    return false;
}

bool AFU::copyToOutputBuffer(int BufferID, void *data, size_t nBytes) {
    if (BufferID >= 0 && BufferID < AFU::numOutputBuffer) {
        if (AFU::Outputbuffers[BufferID] != nullptr) {
            memcpy(AFU::Outputbuffers[BufferID],data, nBytes);
            return true;
        }
    }
    return false;
}


void *AFU::getDsm() {
    return AFU::dsm;
}

void AFU::printDSM() {
    auto dsmNumCL = AFU::dsm_size / 64;
    auto * dsm = static_cast<uint32_t *>(AFU::getDsm());
    MSG("AFU " << AFU::getID() << " DSM:");
    for (int i = 0; i < dsmNumCL-1; ++i) {
        cout << "  [APP]  ";
        for (int j = 15; j >= 0; --j) {
            cout << dsm[GET_INDEX(i, j, 16)] << " ";
        }
        cout << endl;
    }
    cout << "  [APP]  ";
    for (int j = 15; j >= 0; --j) {
        cout << std::hex << dsm[GET_INDEX(dsmNumCL-1, j, 16)] << " ";
    }
    cout << endl;
}

int AFU::getInputBufferGlobalID(int BufferID){
    int globalID = 0;
    for (auto &it : AFU::afuManager.getAFUs()){
        if(it.first == AFU::getID()) break;
        globalID += it.second->getNumInputBuffer();
    }
    return globalID+BufferID;
}

int AFU::getOutputBufferGlobalID(int BufferID){
    int globalID = 0;
    for (auto &it : AFU::afuManager.getAFUs()){
        if(it.first == AFU::getID()) break;
        globalID += it.second->getNumOutputBuffer();
    }
    return globalID+BufferID;  
}

AFU &AFU::operator=(const AFU &AFU) {
    AFU::afuManager = AFU.afuManager;
    AFU::ID = AFU.ID;
    AFU::numInputBuffer = AFU.numInputBuffer;
    AFU::numOutputBuffer = AFU.numOutputBuffer;
    AFU::Inputbuffers = AFU.Inputbuffers;
    AFU::Outputbuffers = AFU.Outputbuffers;
    return *this;
}
