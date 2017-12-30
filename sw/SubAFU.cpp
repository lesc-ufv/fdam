//
// Created by lucas on 12/28/17.
//

#include <assert.h>
#include <cmath>
#include "SubAFU.h"
#include "AFU.h"

SubAFU::SubAFU(AFU &afu, subafu_id id, int numInputBuffer, int numOutputBuffer) : afu(afu),
                                                                                  ID(id),
                                                                                  numInputBuffer(numInputBuffer),
                                                                                  numOutputBuffer(numOutputBuffer) {

}

SubAFU::~SubAFU() {
    SubAFU::clear();
}

void SubAFU::start() {
    SubAFU::afu.writeCSR(START_INTERFACES, (uint64_t) (1 << SubAFU::ID));
}

void SubAFU::stop() {
    SubAFU::afu.writeCSR(STOP_INTERFACES, (uint64_t) (1 << SubAFU::ID));
}

void SubAFU::setDone(bool done) {
    SubAFU::done = done;
}

bool SubAFU::isDone() {
    return SubAFU::done;
}

void SubAFU::waitDone(int64_t timeWaitMax) {

    uint64_t *done = &SubAFU::afu.dsm[GET_INDEX(SubAFU::afu.getNumClDSM()-1,0,8)];
    bool flagDone = false;
    assert(done != nullptr);
    struct timespec pause;
    pause.tv_sec = 0;
    pause.tv_nsec = 1000000;
    while (timeWaitMax > 0 && !flagDone) {
        uint64_t afuDoneBit = (uint64_t) (1 << (SubAFU::ID + 1));
        flagDone = (done[0] & afuDoneBit) != 0;
        timeWaitMax--;
        nanosleep(&pause, NULL);
    };
    SubAFU::stop();
    SubAFU::setDone(flagDone);
}

void *SubAFU::createInputBufferSW(int indexInputBuffer, size_t nBytes, void *dataToCopy) {

    void *bf = nullptr;
    if (indexInputBuffer < SubAFU::numInputBuffer) {
        int idBufferGen = (SubAFU::ID * SubAFU::numInputBuffer + indexInputBuffer);
        int row = ((idBufferGen - (idBufferGen % 8)) / 8);
        int col = idBufferGen % 8;
        double nb(nBytes);
        uint64_t nBytesAlign = ((uint64_t) (std::ceil(nb / 64))) * 64;
        if (SubAFU::afu.workspace[GET_INDEX(row, col, 8)] == 0) {
            bf = SubAFU::afu.fpgaAllocBuffer(nBytesAlign);
            assert(NULL != bf);
            memset(bf, 0x00, nBytesAlign);
            if (dataToCopy != NULL) {
                memcpy(bf, dataToCopy, nBytes);
            }
            SubAFU::afu.workspace[GET_INDEX(row, col, 8)] = (uint64_t) intptr_t(bf);
            row = row + 1;
            SubAFU::afu.workspace[GET_INDEX(row, col, 8)] = (nBytesAlign / 64);
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

void *SubAFU::createOutputBufferSW(int indexOutputBuffer, size_t nBytes) {

    void *bf = nullptr;
    if(indexOutputBuffer < SubAFU::numOutputBuffer) {
        int idBufferGen = (SubAFU::ID * (SubAFU::numOutputBuffer) + indexOutputBuffer);
        int row = ((idBufferGen - (idBufferGen % 8)) / 8) + (SubAFU::numInputBuffer * 2);
        int col = idBufferGen % 8;
        double nb(nBytes);
        uint64_t nBytesAlign = ((uint64_t) (std::ceil(nb / 64))) * 64;
        if (SubAFU::afu.workspace[GET_INDEX(row, col, 8)] == 0) {
            bf = SubAFU::afu.fpgaAllocBuffer(nBytesAlign);
            assert(NULL != bf);
            memset(bf, 0x00, nBytesAlign);
            SubAFU::afu.workspace[GET_INDEX(row, col, 8)] = (uint64_t) intptr_t(bf);
            row = row + 1;
            SubAFU::afu.workspace[GET_INDEX(row, col, 8)] = (nBytesAlign / 64);
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

SubAFU &SubAFU::operator=(const SubAFU &subAFU) {
    SubAFU::afu = subAFU.afu;
    SubAFU::ID = subAFU.ID;
    SubAFU::numInputBuffer = subAFU.numInputBuffer;
    SubAFU::numOutputBuffer = subAFU.numOutputBuffer;
    return *this;
}

void SubAFU::clear() {
    for (int i = 0; i < SubAFU::numInputBuffer; ++i) {
        int idBufferGen = (SubAFU::ID * SubAFU::numInputBuffer + i);
        int row = ((idBufferGen - (idBufferGen % 8)) / 8);
        int col = idBufferGen % 8;
        void *ptbf = (void *) SubAFU::afu.workspace[GET_INDEX(row, col, 8)];
        SubAFU::afu.fpgaFreeBuffer(ptbf);
        SubAFU::afu.workspace[GET_INDEX(row, col, 8)] = 0;
        SubAFU::afu.workspace[GET_INDEX(row + 1, col, 8)] = 0;
    }
    for (int i = 0; i < SubAFU::numOutputBuffer; ++i) {
        int idBufferGen = (SubAFU::ID * (SubAFU::numInputBuffer + SubAFU::numOutputBuffer) + i);
        int row = ((idBufferGen - (idBufferGen % 8)) / 8);
        int col = idBufferGen % 8;
        void *ptbf = (void *) SubAFU::afu.workspace[GET_INDEX(row, col, 8)];
        SubAFU::afu.fpgaFreeBuffer(ptbf);
        SubAFU::afu.workspace[GET_INDEX(row, col, 8)] = 0;
        SubAFU::afu.workspace[GET_INDEX(row + 1, col, 8)] = 0;
    }
}
