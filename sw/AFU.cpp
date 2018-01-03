//
// Created by lucas on 12/28/17.
//

#include <assert.h>
#include <cmath>
#include "AFU.h"
#include "AFUManager.h"

AFU::AFU(AFUManager &afu, afu_id id, int numInputBuffer, int numOutputBuffer) : afu(afu),
                                                                                  ID(id),
                                                                                  numInputBuffer(numInputBuffer),
                                                                                  numOutputBuffer(numOutputBuffer) {

}

AFU::~AFU() {
    AFU::clear();
}

void AFU::start() {
    AFU::afu.writeCSR(START_INTERFACES, (uint64_t) (1 << AFU::ID));
}

void AFU::stop() {
    AFU::afu.writeCSR(STOP_INTERFACES, (uint64_t) (1 << AFU::ID));
}

void AFU::setDone(bool done) {
    AFU::done = done;
}

bool AFU::isDone() {
    return AFU::done;
}

void AFU::waitDone(int64_t timeWaitMax) {

    uint64_t *done = &AFU::afu.dsm[GET_INDEX(AFU::afu.getNumClDSM()-1,0,8)];
    bool flagDone = false;
    assert(done != nullptr);
    struct timespec pause;
    pause.tv_sec = 0;
    pause.tv_nsec = 1000000;
    while (timeWaitMax > 0 && !flagDone) {
        uint64_t afuDoneBit = (uint64_t) (1 << (AFU::ID + 1));
        flagDone = (done[0] & afuDoneBit) != 0;
        timeWaitMax--;
        nanosleep(&pause, NULL);
    };
    AFU::stop();
    AFU::setDone(flagDone);
}

void *AFU::createInputBufferSW(int indexInputBuffer, size_t nBytes, void *dataToCopy) {

    void *bf = nullptr;
    if (indexInputBuffer < AFU::numInputBuffer) {
        int idBufferGen = (AFU::ID * AFU::numInputBuffer + indexInputBuffer);
        int row = ((idBufferGen - (idBufferGen % 8)) / 8);
        int col = idBufferGen % 8;
        double nb(nBytes);
        uint64_t nBytesAlign = ((uint64_t) (std::ceil(nb / 64))) * 64;
        if (AFU::afu.workspace[GET_INDEX(row, col, 8)] == 0) {
            bf = AFU::afu.fpgaAllocBuffer(nBytesAlign);
            assert(NULL != bf);
            memset(bf, 0x00, nBytesAlign);
            if (dataToCopy != NULL) {
                memcpy(bf, dataToCopy, nBytes);
            }
            AFU::afu.workspace[GET_INDEX(row, col, 8)] = (uint64_t) intptr_t(bf);
            row = row + 1;
            AFU::afu.workspace[GET_INDEX(row, col, 8)] = (nBytesAlign / 64);
        } else {
            BEGIN_COLOR(RED);
            MSG("buffer already allocated!");
            END_COLOR();
        }
    } else {
        BEGIN_COLOR(RED);
        MSG("Input Buffer ID invalid!");
        END_COLOR();
    }
    return bf;
}

void *AFU::createOutputBufferSW(int indexOutputBuffer, size_t nBytes) {

    void *bf = nullptr;
    if(indexOutputBuffer < AFU::numOutputBuffer) {
        int idBufferGen = (AFU::ID * (AFU::numOutputBuffer) + indexOutputBuffer);
        int row = ((idBufferGen - (idBufferGen % 8)) / 8) + (AFU::numInputBuffer * 2);
        int col = idBufferGen % 8;
        double nb(nBytes);
        uint64_t nBytesAlign = ((uint64_t) (std::ceil(nb / 64))) * 64;
        if (AFU::afu.workspace[GET_INDEX(row, col, 8)] == 0) {
            bf = AFU::afu.fpgaAllocBuffer(nBytesAlign);
            assert(NULL != bf);
            memset(bf, 0x00, nBytesAlign);
            AFU::afu.workspace[GET_INDEX(row, col, 8)] = (uint64_t) intptr_t(bf);
            row = row + 1;
            AFU::afu.workspace[GET_INDEX(row, col, 8)] = (nBytesAlign / 64);
        } else {
            BEGIN_COLOR(RED);
            MSG("buffer already allocated");
            END_COLOR();
        }
    } else{
        BEGIN_COLOR(RED);
        MSG("Output Buffer ID invalid!");
        END_COLOR();
    }
    return bf;
}

AFU &AFU::operator=(const AFU &subAFU) {
    AFU::afu = subAFU.afu;
    AFU::ID = subAFU.ID;
    AFU::numInputBuffer = subAFU.numInputBuffer;
    AFU::numOutputBuffer = subAFU.numOutputBuffer;
    return *this;
}

void AFU::clear() {
    for (int i = 0; i < AFU::numInputBuffer; ++i) {
        int idBufferGen = (AFU::ID * AFU::numInputBuffer + i);
        int row = ((idBufferGen - (idBufferGen % 8)) / 8);
        int col = idBufferGen % 8;
        void *ptbf = (void *) AFU::afu.workspace[GET_INDEX(row, col, 8)];
        AFU::afu.fpgaFreeBuffer(ptbf);
        AFU::afu.workspace[GET_INDEX(row, col, 8)] = 0;
        AFU::afu.workspace[GET_INDEX(row + 1, col, 8)] = 0;
    }
    for (int i = 0; i < AFU::numOutputBuffer; ++i) {
        int idBufferGen = (AFU::ID * (AFU::numInputBuffer + AFU::numOutputBuffer) + i);
        int row = ((idBufferGen - (idBufferGen % 8)) / 8);
        int col = idBufferGen % 8;
        void *ptbf = (void *) AFU::afu.workspace[GET_INDEX(row, col, 8)];
        AFU::afu.fpgaFreeBuffer(ptbf);
        AFU::afu.workspace[GET_INDEX(row, col, 8)] = 0;
        AFU::afu.workspace[GET_INDEX(row + 1, col, 8)] = 0;
    }
}
