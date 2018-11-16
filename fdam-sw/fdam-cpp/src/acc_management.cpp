#include <fdam/acc/acc_management.h>

AccManagement::AccManagement() {
    AccManagement::fpga = new OPAE_SVC_WRAPPER(UUID_ACC_MANAGEMENT);
    if (!AccManagement::fpga->isOk()) {
        MSG("Error 0: FPGA or Simulator not found!");
        AccManagement::clear();
        exit(1);
    }
    AccManagement::csrs = new CSR_MGR(*AccManagement::fpga);
    AccManagement::createAccelerators();
}

AccManagement::~AccManagement() {
    AccManagement::clear();
}

void AccManagement::createAccelerators() {
    AccManagement::getInfoHw();
    auto n = AccManagement::getNumAccelerators();
    if (n > 0) {
        for (int i = 0; i < n; ++i) {
            int numInputQueue = AccManagement::accInfo[i * 2];
            int numOutputQueue = AccManagement::accInfo[i * 2 + 1];
            auto id = (unsigned short) i;
            auto *acc = new Accelerator(*this, id, numInputQueue, numOutputQueue);
            AccManagement::accelerators[id] = acc;
        }
    } else {
        MSG("Error 1: Accelerator Management not found!");
        AccManagement::clear();
        exit(1);
    }
}

void AccManagement::writeCSR(unsigned int regID, unsigned long val) {
    AccManagement::csrs->writeCSR(regID, val);
}

unsigned long AccManagement::readCSR(unsigned int regID) {
    return AccManagement::csrs->readCSR(regID);
}

void AccManagement::clear() {
    for (auto &acc: AccManagement::accelerators) {
        delete (acc.second);
    }
    delete AccManagement::csrs;
    delete AccManagement::fpga;
}

void AccManagement::setNumInputQueue(int numInputQueue) {
    AccManagement::numInputQueue = numInputQueue;
}

void AccManagement::setNumOutputQueue(int numOutputQueue) {
    AccManagement::numOutputQueue = numOutputQueue;
}

void AccManagement::setNumAccelerators(int numAccelerators) {
    AccManagement::numAccelerators = numAccelerators;
}

int AccManagement::getNumInputQueue() const {
    return AccManagement::numInputQueue;
}

int AccManagement::getNumOutputQueue() const {
    return AccManagement::numOutputQueue;
}

int AccManagement::getNumAccelerators() const {
    return AccManagement::numAccelerators;
}

void *AccManagement::accAllocQueue(long long numBytes) {
    return AccManagement::fpga->allocBuffer(static_cast<size_t>(numBytes));
}

void AccManagement::accFreeQueue(void *ptr) {
    AccManagement::fpga->freeBuffer(ptr);
}

void AccManagement::startAccelerators(unsigned long accelerators) {
    AccManagement::writeCSR(REG_START_AFUs, accelerators);
}

void AccManagement::stopAccelerators(unsigned long accelerators) {
    AccManagement::writeCSR(REG_STOP_AFUs, accelerators);
}

void AccManagement::resetAccelerators(unsigned long accelerators) {
    AccManagement::writeCSR(REG_RESET_AFUs, accelerators);
}

void AccManagement::waitDone(unsigned long accelerators, long timeWaitMax) {
    for (auto &acc : AccManagement::accelerators) {
        if ((accelerators & (1L << acc.second->getId())) != 0L) {
            acc.second->waitDone(timeWaitMax);
        }
    }
}

bool AccManagement::isSimulated() {
    return AccManagement::fpga->hwIsSimulated();
}

Accelerator &AccManagement::getAccelerator(unsigned short id) {
    auto acc = (AccManagement::accelerators.at(id));
    return *acc;
}

bool AccManagement::isAccDone(const unsigned long accelerators) {

    for (auto &acc : AccManagement::accelerators) {
        if ((accelerators & (1 << acc.second->getId())) != 0) {
            if (!acc.second->isDone())return false;
        }
    }

    return true;
}

long AccManagement::getTotalClocks(){
   return AccManagement::readCSR(REG_CLOCK_COUNT);    
}

long AccManagement::getTotalWrittenCacheLine(){
   return AccManagement::readCSR(REG_CL_WR_COUNT);     
}

long AccManagement::getTotalCacheLineRead(){
   return AccManagement::readCSR(REG_CL_RD_COUNT);     
}

long AccManagement::getClockSpeed(){
   return AccManagement::csrs->getAFUMHz();    
}

bool AccManagement::IsSimulated(){
   return AccManagement::fpga->hwIsSimulated();    
}

void AccManagement::printHwInfo() {

    double totalClocks = AccManagement::readCSR(REG_CLOCK_COUNT);
    double totalClWrite = AccManagement::readCSR(REG_CL_WR_COUNT);
    double totalClRead = AccManagement::readCSR(REG_CL_RD_COUNT);
    double cicleTime = 1.0 / (AccManagement::csrs->getAFUMHz() * 1000000.0);
    double totalTime = cicleTime * totalClocks;
    double Mb = ((totalClRead + totalClWrite) * 64.0) / (1024 * 1024);
    auto throughput = Mb / totalTime;

    MSG("INFO: Accelerator Management");
    MSG("INFO: TARGET:" << (AccManagement::fpga->hwIsSimulated() ? " [simulated]" : "[real device]"));
    MSG("INFO: Frequency: " << std::dec << AccManagement::csrs->getAFUMHz() << " MHz");
    MSG("INFO: Number of accelerators: " << AccManagement::getNumAccelerators());
    MSG("INFO: Total number of input queues: " << AccManagement::getNumInputQueue());
    MSG("INFO: Total number of output queues: " << AccManagement::getNumOutputQueue());
    MSG("INFO: Total clock: " << totalClocks);
    MSG("INFO: Total bytes read: " << totalClRead * 64);
    MSG("INFO: Total written bytes: " << totalClWrite * 64);
    MSG("INFO: Approximate throughput: " << throughput << "MB/s");
    if (mpfVtpIsAvailable(AccManagement::fpga->mpf_handle)) {
        mpf_vtp_stats vtp_stats{};
        mpfVtpGetStats(AccManagement::fpga->mpf_handle, &vtp_stats);
        if (vtp_stats.numFailedTranslations) {
            MSG("INFO: VTP failed translating VA: 0x" << hex << vtp_stats.ptWalkLastVAddr << dec);
        }
        MSG("INFO: VTP PT walk cycles: " << vtp_stats.numPTWalkBusyCycles);
        MSG("INFO: VTP L2 4KB hit / miss: " << vtp_stats.numTLBHits4KB << " / " << vtp_stats.numTLBMisses4KB);
        MSG("INFO: VTP L2 2MB hit / miss: " << vtp_stats.numTLBHits2MB << " / " << vtp_stats.numTLBMisses2MB);
    }
}

void AccManagement::getInfoHw() {
    unsigned long inf[8];
    inf[0] = AccManagement::readCSR(REG_INF_1);
    inf[1] = AccManagement::readCSR(REG_INF_2);
    inf[2] = AccManagement::readCSR(REG_INF_3);
    inf[3] = AccManagement::readCSR(REG_INF_4);
    inf[4] = AccManagement::readCSR(REG_INF_5);
    inf[5] = AccManagement::readCSR(REG_INF_6);
    inf[6] = AccManagement::readCSR(REG_INF_7);
    inf[7] = AccManagement::readCSR(REG_INF_8);
    int index = 0;
    for (unsigned long i : inf) {
        for (int j = 0; j < 8; ++j) {
            AccManagement::accInfo[index] = (int) ((i >> j * 8) & 0xFF);
            index++;
        }
    }
    AccManagement::numAccelerators = 0;
    AccManagement::numInputQueue = 0;
    AccManagement::numOutputQueue = 0;
    for (int k = 0; k < ACC_INF_SIZE; k += 2) {
        if (AccManagement::accInfo[k] != 0 && AccManagement::accInfo[k + 1] != 0) {
            AccManagement::numAccelerators++;
        }
        AccManagement::numInputQueue += AccManagement::accInfo[k];
        AccManagement::numOutputQueue += AccManagement::accInfo[k + 1];
    }
}

const std::map<unsigned short, Accelerator *> &AccManagement::getAccelerators() const {
    return AccManagement::accelerators;
}
