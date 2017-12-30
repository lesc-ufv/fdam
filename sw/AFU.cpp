//
// Created by lucas on 12/28/17.
//

#include <assert.h>
#include <cmath>
#include "AFU.h"


AFU::AFU(const char *accel_uuid) {

    AFU::fpga = new OPAE_SVC_WRAPPER(accel_uuid);
    assert(AFU::fpga->isOk());
    AFU::csrs = new CSR_MGR(*AFU::fpga);
    AFU::readInfoHwAfu();
    assert(AFU::numSubAFUs > 0);
    AFU::createWorkspace();
    AFU::createSubAFUs();
}

AFU::~AFU() {
    AFU::clear();
}

void AFU::readInfoHwAfu() {
    uint64_t inf[8];
    inf[0] = AFU::readCSR(INF_1);
    inf[1] = AFU::readCSR(INF_2);
    inf[2] = AFU::readCSR(INF_3);
    inf[3] = AFU::readCSR(INF_4);
    inf[4] = AFU::readCSR(INF_5);
    inf[5] = AFU::readCSR(INF_6);
    inf[6] = AFU::readCSR(INF_7);
    inf[7] = AFU::readCSR(INF_8);
    int index = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            AFU::afuInfo[index] = (int) ((inf[i] >> j * 8) & 0xFF);
            index++;
        }
    }
    AFU::numSubAFUs = 0;
    AFU::numInputBuffers = 0;
    AFU::numOutputBuffers = 0;
    for (int k = 0; k < AFU_INF_SIZE; k += 2) {
        if (AFU::afuInfo[k] != 0 && AFU::afuInfo[k + 1] != 0) {
            AFU::numSubAFUs++;
        }
        AFU::numInputBuffers += AFU::afuInfo[k];
        AFU::numOutputBuffers += AFU::afuInfo[k + 1];
    }

}

SubAFU *AFU::getSubAFU(subafu_id id) {
    if (id < AFU::numSubAFUs) {
        return AFU::subAFUs.at(id);
    } else {
        BEGIN_COLOR(RED);
        MSG("SubAFU ID invalid!");
        END_COLOR();
        clear();
        exit(255);
    }
}

void AFU::writeCSR(uint32_t regID, uint64_t val) {
    AFU::csrs->writeCSR(regID, val);
}

uint64_t AFU::readCSR(uint32_t regID) {
    return AFU::csrs->readCSR(regID);
}

bool AFU::AFUIsSimulated() {
    return AFU::fpga->hwIsSimulated();
}

void *AFU::fpgaAllocBuffer(size_t numBytes) {
    AFU::commitedWorkspace = false;
    return AFU::fpga->allocBuffer(numBytes);
}

void AFU::fpgaFreeBuffer(void *ptr) {
    AFU::commitedWorkspace = false;
    AFU::fpga->freeBuffer(ptr);
}

void AFU::createWorkspace() {
    int num_cl_conf_in = (int) (2 * std::ceil((double)AFU::numInputBuffers / 8));
    int num_cl_conf_out = (int) (2 * std::ceil((double)AFU::numOutputBuffers / 8));
    AFU::numClConfs = num_cl_conf_in + num_cl_conf_out;
    AFU::numClDSM = 1 + (AFU::numClConfs / 2);
    size_t workspaceSize = (size_t) CL(AFU::numClConfs + AFU::numClDSM);
    AFU::workspace = (uint64_t *) AFU::fpgaAllocBuffer(workspaceSize);
    assert(NULL != AFU::workspace);
    AFU::dsm = (uint64_t *) (((char *) AFU::workspace) + CL(AFU::numClConfs));
    memset(AFU::workspace, 0x00, workspaceSize);
    AFU::writeCSR(ADDR_WORKSPACE_BASE, (uint64_t) intptr_t(AFU::workspace));
    uint64_t size = (uint64_t) AFU::numClDSM;
    size = size << 16;
    size |= AFU::numClConfs;
    AFU::writeCSR(WORKSPACE_SIZE, size);
}

void AFU::createSubAFUs() {
    for (int i = 0; i < AFU::numSubAFUs; ++i) {
        int numInputBuffers = afuInfo[i * 2];
        int numOutputBuffers = afuInfo[i * 2 + 1];
        subafu_id id = i;
        AFU::subAFUs[id] = new SubAFU(*this, id, numInputBuffers, numOutputBuffers);
    }
}

void AFU::clear() {
    AFU::commitedWorkspace = false;
    AFU::writeCSR(CFG_REG, AFU_CONTROLLER_STOP);
    for (map<subafu_id, SubAFU *>::iterator it = AFU::subAFUs.begin(); it != AFU::subAFUs.end(); ++it)
        delete (it->second);
    AFU::fpgaFreeBuffer(AFU::workspace);
    delete AFU::csrs;
    delete AFU::fpga;
}

void AFU::commitWorkspace() {
    AFU::commitedWorkspace = false;
    AFU::writeCSR(CFG_REG, AFU_CONTROLLER_STOP);
    AFU::writeCSR(CFG_REG, AFU_CONTROLLER_UPDATE_WKP);
    int result = 1;
    struct timespec pause;
    pause.tv_sec = 1;
    pause.tv_nsec = 0;
    while (result != 0) {
        nanosleep(&pause, NULL);
        result = (int) (AFU::readCSR(AFU_CONTROLLER_STATUS) & 1L);
    };
    AFU::writeCSR(CFG_REG, AFU_CONTROLLER_START);
    while (result == 0) {
        nanosleep(&pause, NULL);
        result = (int) (AFU::readCSR(AFU_CONTROLLER_STATUS) & 1L);
    };
    AFU::commitedWorkspace = true;
}

bool AFU::workspaceIscommited() {
    return AFU::commitedWorkspace;
}
void AFU::printStatics() {
    double total_cicles = AFU::readCSR(CLOCK_COUNT);
    double total_cl_wr = AFU::readCSR(CL_WR_COUNT);
    double total_cl_rd = AFU::readCSR(CL_RD_COUNT);
    double totalBytesBuffers = CL(total_cl_wr + total_cl_rd);
    double cicle_time = 1.0 / (AFU::csrs->getAFUMHz() * 1000000.0);
    double time_spend = cicle_time * total_cicles;
    double speed = 0;
    if (time_spend != 0) {
        speed = totalBytesBuffers / time_spend;
        speed = speed / GB(1);
    }
    // Reads CSRs to get some statistics
    BEGIN_COLOR(GREEN);
    MSG("Total Clock cicles: " << std::dec << total_cicles);
    MSG("AFU frequency: " << std::dec << AFU::csrs->getAFUMHz() << " MHz");
    MSG("AFU TARGET:" << (AFU::fpga->hwIsSimulated() ? " [simulated]" : "[real device]"));
    MSG("Transfer speed:" << speed << " GB/s");

    // MFP VTP (virtual to physical) statistics
    if (mpfVtpIsAvailable(AFU::fpga->mpf_handle)) {
        mpf_vtp_stats vtp_stats;
        mpfVtpGetStats(AFU::fpga->mpf_handle, &vtp_stats);
        if (vtp_stats.numFailedTranslations) {
            MSG("VTP failed translating VA: 0x" << hex << uint64_t(vtp_stats.ptWalkLastVAddr) << dec);
        }
        MSG("VTP PT walk cycles: " << vtp_stats.numPTWalkBusyCycles);
        MSG("VTP L2 4KB hit / miss: " << vtp_stats.numTLBHits4KB << " / " << vtp_stats.numTLBMisses4KB);
        MSG("VTP L2 2MB hit / miss: " << vtp_stats.numTLBHits2MB << " / " << vtp_stats.numTLBMisses2MB);
    }
    END_COLOR();
}

void AFU::printInfoAFU() {
    BEGIN_COLOR(BLUE);
    MSG("AFU INFO:");
    MSG("NUM SUB AFUS: " << numSubAFUs);
    for (int i = 0; i < numSubAFUs; ++i) {
        MSG("SUB AFU " << i << ": Input Buffer: " << afuInfo[i] << " Output Buffer: " << afuInfo[i + 1]);
    }
    END_COLOR();
}
void AFU::printWorkspace() {
    for (int i = 0; i < AFU::numClConfs; ++i) {
        for (int j = 0; j < 8; ++j) {
          cout << std::hex << AFU::workspace[GET_INDEX(i,j,8)] << " ";
        }
        cout << endl;
    }
}

void AFU::printDSM() {
    BEGIN_COLOR(GREEN);
    MSG("DSM:");
    for (int i = 0; i < AFU::numClDSM; ++i) {
        cout <<"  [APP]  ";
        for (int j = 7; j >= 0; --j) {
            cout << std::hex << AFU::dsm[GET_INDEX(i,j,8)] << " ";
        }
        cout << endl;
    }
    END_COLOR();
}

int AFU::getNumClConf() {
    return AFU::numClConfs;
}

int AFU::getNumClDSM() {
    return AFU::numClDSM;
}

