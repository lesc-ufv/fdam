//
// Created by lucas on 12/28/17.
//

#include <assert.h>
#include <cmath>
#include "AFU.h"
#include "AFUManager.h"

AFU::AFU(AFUManager_t &afuManager, afu_id id, int numInputBuffer, int numOutputBuffer) : afuManager(afuManager),
                                                                                ID(id),
                                                                                numInputBuffer(numInputBuffer),
                                                                                numOutputBuffer(numOutputBuffer) {

    AFU::Inputbuffers = new void*[numInputBuffer];
    AFU::Outputbuffers = new void*[numOutputBuffer];
    AFU::sizeOfInputBuffers = new int[numInputBuffer];
    AFU::sizeOfOutputBuffers = new int[numOutputBuffer];

    for (int i = 0; i < numInputBuffer; ++i) {
        Inputbuffers[i] = nullptr;
    }
    for (int i = 0; i < numOutputBuffer; ++i) {
        Outputbuffers[i] = nullptr;
    }

}

AFU::~AFU() {
    AFU::clear();
}

void AFU::start() {
    AFU::afuManager.writeCSR(REG_START_AFUs, (uint64_t) (1L << AFU::ID));
}

void AFU::stop() {
    AFU::afuManager.writeCSR(REG_STOP_AFUs, (uint64_t) (1L << AFU::ID));
}

void AFU::setDone(bool done) {
    AFU::done = done;
}

bool AFU::isDone() {
    return AFU::done;
}

void AFU::waitDone(int64_t timeWaitMax) {

    uint64_t *done = &AFU::afuManager.dsm[GET_INDEX(AFU::afuManager.getNumClDSM() - 1, 0, 8)];
    bool flagDone = false;
    assert(done != nullptr);
    struct timespec pause;
    pause.tv_sec = 0;
    pause.tv_nsec = 500000;
    timeWaitMax *=2;
    while (timeWaitMax > 0 && !flagDone) {
        uint64_t afuDoneBit = (uint64_t) (1L << (AFU::ID + 1L));
        flagDone = (done[0] & afuDoneBit) != 0;
        timeWaitMax--;
        nanosleep(&pause, NULL);
    };
    AFU::stop();
    AFU::setDone(flagDone);
}
bool AFU::createInputBufferSW(int BufferID, size_t nBytes, void *dataToCopy) {
    if(BufferID >= 0 && BufferID < AFU::numInputBuffer){
        if(AFU::Inputbuffers[BufferID] != nullptr){
            return false;
        }else{
            double nb(nBytes);
            uint64_t nBytesAlign = ((uint64_t) (std::ceil(nb / 64))) * 64;
            AFU::Inputbuffers[BufferID] = AFU::afuManager.fpgaAllocBuffer(nBytesAlign);
            if(AFU::Inputbuffers[BufferID] != nullptr){
                memset(AFU::Inputbuffers[BufferID],0,nBytesAlign);
                AFU::sizeOfInputBuffers[BufferID] = (int)nBytesAlign;
                if(dataToCopy != nullptr){
                    memcpy(AFU::Inputbuffers[BufferID],dataToCopy,nBytes);
                }
                return true;
            }
        }
    }
    return false;
}



bool AFU::createOutputBufferSW(int BufferID, size_t nBytes) {
    if(BufferID >= 0 && BufferID < AFU::numOutputBuffer){
        if(AFU::Outputbuffers[BufferID] != nullptr){
            return false;
        }else{
            double nb(nBytes);
            uint64_t nBytesAlign = ((uint64_t) (std::ceil(nb / 64))) * 64;
            AFU::Outputbuffers[BufferID] = AFU::afuManager.fpgaAllocBuffer(nBytes);
            if(AFU::Outputbuffers[BufferID] != nullptr){
                memset(AFU::Outputbuffers[BufferID],0,nBytesAlign);
                AFU::sizeOfOutputBuffers[BufferID] = (int)nBytesAlign;
                return true;
            }
        }
    }
    return false;
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

void AFU::clear() {

    delete AFU::sizeOfInputBuffers;
    delete AFU::sizeOfOutputBuffers;
    for (int i = 0; i < AFU::numInputBuffer; ++i) {
        if(nullptr != AFU::Inputbuffers[i]){
            AFU::afuManager.fpgaFreeBuffer(AFU::Inputbuffers[i]);
        }
    }
    for (int i = 0; i < AFU::numOutputBuffer; ++i) {
        if(nullptr != AFU::Outputbuffers[i]){
            AFU::afuManager.fpgaFreeBuffer(AFU::Outputbuffers[i]);
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
    if(BufferID >= 0 && BufferID < AFU::numInputBuffer){
        return  AFU::Inputbuffers[BufferID];
    }
    return nullptr;
}

void *AFU::getOutputBuffer(int BufferID) {
    if(BufferID >= 0 && BufferID < AFU::numOutputBuffer){
        return  AFU::Outputbuffers[BufferID];
    }
    return nullptr;
}

afu_id AFU::getID() const {
    return ID;
}

int AFU::getSizeOfInputBuffer(int BufferID) {
    if(BufferID >= 0 && BufferID < AFU::numInputBuffer){
        return  AFU::sizeOfInputBuffers[BufferID];
    }
    return 0;
}

int AFU::getSizeOfOutputBuffer(int BufferID) {
    if(BufferID >= 0 && BufferID < AFU::numOutputBuffer){
        return  AFU::sizeOfOutputBuffers[BufferID];
    }
    return 0;
}
