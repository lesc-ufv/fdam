#include "AFU.h"

AFU::AFU(const char *accel_uuid) {
    this->fpga = new OPAE_SVC_WRAPPER(accel_uuid);
    assert(this->fpga->isOk());
    this->csrs = new CSR_MGR(*this->fpga);
    this->wkp = (Workspace *) fpga->allocBuffer(sizeof(Workspace));
    assert(NULL != this->wkp);
    memset(this->wkp, 0x00, sizeof(Workspace));
    this->csrs->writeCSR(ADDR_WORKSPACE_BASE, (uint64_t) intptr_t(this->wkp));
    totalBytesBuffers = 0;
}

AFU::~AFU() {
    // Release the buffers
    for (int i = 0; i < 8; ++i) {
        if (this->wkp->inputBuffer[i] != 0)
            fpga->freeBuffer((void *) this->wkp->inputBuffer[i]);
        if (this->wkp->outputBuffer[i] != 0)
            fpga->freeBuffer((void *) this->wkp->outputBuffer[i]);
    }
    fpga->freeBuffer(this->wkp);
    delete this->csrs;
    delete this->fpga;
}

void *AFU::addInputBuffer(int bufferId, size_t nBytes, void *dataToCopy) {
    double nb(nBytes);
    uint64_t nBytesAlign = ((uint64_t) (ceil(nb / 64))) * 64;
    assert((bufferId >= 0) && (bufferId <= 7));
    void *bf = fpga->allocBuffer(nBytesAlign);
    assert(NULL != bf);
    memset(bf, 0x00, nBytesAlign);
    if (dataToCopy != NULL) {
        memcpy(bf, dataToCopy, nBytes);
    }
    this->wkp->inputBuffer[bufferId] = (uint64_t) intptr_t(bf);
    this->wkp->numDataIn[bufferId] = (nBytesAlign / 64);
    totalBytesBuffers += nBytesAlign;
    return bf;
}

void *AFU::addOutputBuffer(int bufferId, size_t nBytes) {
    double nb(nBytes);
    uint64_t nBytesAlign = ((uint64_t) (ceil(nb / 64))) * 64;
    assert((bufferId >= 0) && (bufferId <= 7));
    void *bf = fpga->allocBuffer(nBytesAlign);
    assert(NULL != bf);
    memset(bf, 0x00, nBytesAlign);
    this->wkp->outputBuffer[bufferId] = (uint64_t) intptr_t(bf);
    this->wkp->numDataOut[bufferId] = (nBytesAlign / 64);
    return bf;
}

void AFU::run() {
    //Start FPGA
    csrs->writeCSR(CFG_REG, AFU::AFU_START);
    struct timespec pause;
    pause.tv_sec = (fpga->hwIsSimulated() ? 1 : 0);
    pause.tv_nsec = 2500000;
#if TIMEOUT_EN
    uint64_t count = TIMEOUT_SEC;
#endif
    while (0 == this->wkp->dsm.done) {
#if TIMEOUT_EN
        if(0 == count--)
       break;
#endif
        nanosleep(&pause, NULL);
    }
    if (0 == this->wkp->dsm.done)
        MSG("AFU NEVER ANSWERED!", RED);
}

void AFU::reset() {
    this->csrs->writeCSR(CFG_REG, 0x0000000000000002);
    this->csrs->writeCSR(CFG_REG, 0x0000000000000000);
}

void AFU::printBuffer(int bufferId, bool isInput) {
    assert((bufferId >= 0) && (bufferId <= 7));
    char *bf = (char *) (isInput ? this->wkp->inputBuffer[bufferId] : this->wkp->outputBuffer[bufferId]);
    uint64_t size = (isInput ? this->wkp->numDataIn[bufferId] : this->wkp->numDataOut[bufferId]);
    if (isInput)
        MSG("INPUT BUFFER" << bufferId << ":", 0);
    else
        MSG("OUTPUT BUFFER" << bufferId << ":", 0);
    char buffer[64];
    int j = 0;
    int cl = 0;
    for (int i = 0; i < size * 64; ++i) {
        if (i % 64 == 0) {
            j = 63;
        }
        buffer[j--] = bf[i];
        if (j == -1) {
            printf("  [APP]  CL %d: ", cl++);
            for (int k = 0; k < 64; ++k) {
                printf("%X", buffer[k]);
            }
            printf("\n");
        }
    }
}

void AFU::printStatics() {
    double total_cicles = this->csrs->readCSR(0);
    double cicle_time = 1.0 / (this->csrs->getAFUMHz() * 1000000.0);
    double time_spend = cicle_time * total_cicles;
    double speed = totalBytesBuffers / time_spend;
    speed = speed / GB(1);

    // Reads CSRs to get some statistics
    MSG("Total Clock cicles: " << std::dec << total_cicles, GREEN);
    MSG("AFU frequency: " << std::dec << csrs->getAFUMHz() << " MHz", GREEN);
    MSG("AFU TARGET:" << (fpga->hwIsSimulated() ? " [simulated]" : "[real device]"), GREEN);
    MSG("Transfer speed:" << speed << " GB/s", GREEN);

    // MFP VTP (virtual to physical) statistics
    if (mpfVtpIsAvailable(fpga->mpf_handle)) {
        mpf_vtp_stats vtp_stats;
        mpfVtpGetStats(fpga->mpf_handle, &vtp_stats);
        if (vtp_stats.numFailedTranslations) {
            MSG("VTP failed translating VA: 0x" << hex << uint64_t(vtp_stats.ptWalkLastVAddr) << dec, GREEN);
        }
        MSG("VTP PT walk cycles: " << vtp_stats.numPTWalkBusyCycles, GREEN);
        MSG("VTP L2 4KB hit / miss: " << vtp_stats.numTLBHits4KB << " / " << vtp_stats.numTLBMisses4KB, GREEN);
        MSG("VTP L2 2MB hit / miss: " << vtp_stats.numTLBHits2MB << " / " << vtp_stats.numTLBMisses2MB, GREEN);
    }
}
