#include <Accelerator.h>
#include <AccManagement.h>

Accelerator::Accelerator(AccManagement_t &accManagement, accid_t id, int numInputQueue, int numOutputQueue) :
        accManagement(accManagement),
        id(id),
        numInputQueue(numInputQueue),
        numOutputQueue(numOutputQueue) {

    if (numInputQueue <= 0 || numOutputQueue <= 0) {
        MSG("Error 2: Number of input queues or number of output queues less than or equal to 0!");
        accManagement.clear();
        exit(1);
    }

    Accelerator::inputQueue = new void *[numInputQueue];
    Accelerator::outputQueue = new void *[numOutputQueue];
    Accelerator::sizeOfInputQueues = new size_t[numInputQueue];
    Accelerator::sizeOfOutputQueues = new size_t[numOutputQueue];

    for (int i = 0; i < Accelerator::getNumInputQueue() ; ++i) {
        Accelerator::inputQueue[i] = nullptr;
        Accelerator::sizeOfInputQueues[i] = 0;
        Accelerator::sendConfIn(static_cast<uint8_t>(i));
    }
    for (int i = 0; i < Accelerator::getNumOutputQueue(); ++i) {
        Accelerator::outputQueue[i] = nullptr;
        Accelerator::sizeOfOutputQueues[i] = 0;
        Accelerator::sendConfOut(static_cast<uint8_t>(i));
    }
    Accelerator::createDsm();
}

Accelerator::~Accelerator(){
    Accelerator::clear();
};

void Accelerator::createDsm() {
    double size = CL(
            1.0 + ceil(Accelerator::getNumInputQueue() / 16.0) + ceil(Accelerator::getNumOutputQueue() / 16.0));
    auto nBytesAlign = static_cast<size_t>((std::ceil(size / 256.0)) * 256.0);
    Accelerator::setDsmSize(static_cast<size_t>(nBytesAlign));
    Accelerator::ptrDsm = Accelerator::accManagement.fpgaAllocQueue(Accelerator::getDsmSize());
    Accelerator::clearDsm();
    Accelerator::sendConfDsm();
}

void *Accelerator::getDsm() const {
    return Accelerator::ptrDsm;
}

void Accelerator::setDsmSize(size_t size) {
    Accelerator::dsmSize = size;
}

size_t Accelerator::getDsmSize() const {
    return Accelerator::dsmSize;
}

void Accelerator::sendConfDsm() {
    int idQueueGlobal = Accelerator::getId();
    auto numCl = static_cast<uint64_t>(Accelerator::getDsmSize() / 64);
    auto val_low = static_cast<uint64_t>(numCl << 32 | idQueueGlobal);
    auto val_high = static_cast<uint64_t>(intptr_t(Accelerator::getDsm()) / 64);
    Accelerator::accManagement.writeCSR(REG_CONF_DSM_LOW, val_low);
    Accelerator::accManagement.writeCSR(REG_CONF_DSM_HIGH, val_high);
}

void Accelerator::clearDsm() {
    std::memset(Accelerator::getDsm(), 0x00, Accelerator::getDsmSize());
}

void Accelerator::sendConfIn(uint8_t idQueue) {
    int idQueueGlobal = Accelerator::getInputQueueGlobalID(idQueue);
    auto numCl = static_cast<uint64_t>(Accelerator::getSizeOfInputQueue(idQueue) / 64);
    auto val_low = static_cast<uint64_t>(numCl << 32 | idQueueGlobal);
    auto val_high = static_cast<uint64_t>(intptr_t(Accelerator::getInputQueue(idQueue)) / 64);
    Accelerator::accManagement.writeCSR(REG_CONF_IN_LOW, val_low);
    Accelerator::accManagement.writeCSR(REG_CONF_IN_HIGH, val_high);
}

void Accelerator::sendConfOut(uint8_t idQueue) {
    int idQueueGlobal = Accelerator::getOutputQueueGlobalID(idQueue);
    auto numCl = static_cast<uint64_t>(Accelerator::getSizeOfOutputQueue(idQueue) / 64);
    auto val_low = static_cast<uint64_t>(numCl << 32 | idQueueGlobal);
    auto val_high = static_cast<uint64_t>(intptr_t(Accelerator::getOutputQueue(idQueue)) / 64);
    Accelerator::accManagement.writeCSR(REG_CONF_OUT_LOW, val_low);
    Accelerator::accManagement.writeCSR(REG_CONF_OUT_HIGH, val_high);
}

void *Accelerator::getInputQueue(uint8_t idQueue) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue()) {
        return Accelerator::inputQueue[idQueue];
    }
    return nullptr;
}

void *Accelerator::getOutputQueue(uint8_t idQueue) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue()) {
        return Accelerator::outputQueue[idQueue];
    }
    return nullptr;
}

void Accelerator::setInputQueue(uint8_t idQueue, void *ptrQueue, size_t numBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue()) {
        Accelerator::inputQueue[idQueue] = ptrQueue;
        Accelerator::sizeOfInputQueues[idQueue] = numBytes;
    }
}

void Accelerator::setOutputQueue(uint8_t idQueue, void *ptrQueue, size_t numBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue()) {
        Accelerator::outputQueue[idQueue] = ptrQueue;
        Accelerator::sizeOfOutputQueues[idQueue] = numBytes;
    }
}

void Accelerator::start() {
    Accelerator::clearDsm();
    Accelerator::accManagement.startAccelerators(static_cast<const uint64_t>(1L << Accelerator::getId()));
}

void Accelerator::stop() {
    Accelerator::accManagement.stopAccelerators(static_cast<const uint64_t>(1L << Accelerator::getId()));
}

void Accelerator::reset() {
    Accelerator::clearDsm();
    Accelerator::accManagement.resetAccelerators(static_cast<const uint64_t>(1L << Accelerator::getId()));
    Accelerator::accManagement.resetAccelerators(0L);
}

bool Accelerator::createInputQueue(uint8_t idQueue, size_t nBytes, void *dataToCopy) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue()) {
        double nb = nBytes;
        auto nBytesAlign = static_cast<size_t>((std::ceil(nb / 256.0)) * 256.0);
        void *ptrQueue = Accelerator::accManagement.fpgaAllocQueue(nBytesAlign);
        std::memset(ptrQueue, 0x00, nBytesAlign);
        if (Accelerator::getInputQueue(idQueue) != nullptr) {
            Accelerator::accManagement.fpgaFreeQueue(Accelerator::getInputQueue(idQueue));
        }
        Accelerator::setInputQueue(idQueue, ptrQueue, nBytesAlign);
        if (dataToCopy != nullptr) {
            std::memcpy(ptrQueue, dataToCopy, nBytes);
        }
        Accelerator::sendConfIn(idQueue);
        return true;
    }
    return false;
}

bool Accelerator::createOutputQueue(uint8_t idQueue, size_t nBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue()) {
        double nb = nBytes;
        auto nBytesAlign = static_cast<size_t>((std::ceil(nb / 256.0)) * 256.0);
        void *ptrQueue = Accelerator::accManagement.fpgaAllocQueue(nBytesAlign);
        std::memset(ptrQueue, 0x00, nBytesAlign);
        if (Accelerator::getOutputQueue(idQueue) != nullptr) {
            Accelerator::accManagement.fpgaFreeQueue(Accelerator::getOutputQueue(idQueue));
        }
        Accelerator::setOutputQueue(idQueue, ptrQueue, nBytesAlign);
        Accelerator::sendConfOut(idQueue);
        return true;
    }
    return false;
}

bool Accelerator::copyFromInputQueue(uint8_t idQueue, void *data, size_t nBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue()) {
        std::memcpy(data, Accelerator::getInputQueue(idQueue), nBytes);
        return true;
    }
    return false;
}

bool Accelerator::copyFromOutputQueue(uint8_t idQueue, void *data, size_t nBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue()) {
        std::memcpy(data, Accelerator::getOutputQueue(idQueue), nBytes);
        return true;
    }
    return false;
}

bool Accelerator::copyToInputQueue(uint8_t idQueue, void *data, size_t nBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue()) {
        std::memcpy(Accelerator::getInputQueue(idQueue), data, nBytes);
        return true;
    }
    return false;
}

bool Accelerator::copyToOutputQueue(uint8_t idQueue, void *data, size_t nBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue()) {
        std::memcpy(Accelerator::getOutputQueue(idQueue), data, nBytes);
        return true;
    }
    return false;
}

bool Accelerator::setNumBytesRead(uint8_t idQueue, size_t nBytes) {
    void *ptrQueue = Accelerator::getInputQueue(idQueue);
    if (idQueue >= 0 &&
        idQueue < Accelerator::getNumInputQueue() &&
        nBytes <= Accelerator::getSizeOfInputQueue(idQueue) &&
        ptrQueue != nullptr) {
        Accelerator::setInputQueue(idQueue, ptrQueue, nBytes);
        Accelerator::sendConfIn(idQueue);
        return true;
    }
    return false;
}

bool Accelerator::setNumBytesWrite(uint8_t idQueue, size_t nBytes) {
    void *ptrQueue = Accelerator::getOutputQueue(idQueue);
    if (idQueue >= 0 &&
        idQueue < Accelerator::getNumOutputQueue() &&
        nBytes <= Accelerator::getSizeOfOutputQueue(idQueue) &&
        ptrQueue != nullptr) {
        Accelerator::setOutputQueue(idQueue, ptrQueue, nBytes);
        Accelerator::sendConfOut(idQueue);
        return true;
    }
    return false;
}

size_t Accelerator::getSizeOfInputQueue(uint8_t idQueue) const {
    return Accelerator::sizeOfInputQueues[idQueue];
}

size_t Accelerator::getSizeOfOutputQueue(uint8_t idQueue) const {
    return Accelerator::sizeOfOutputQueues[idQueue];
}

int Accelerator::getNumInputQueue() const {
    return Accelerator::numInputQueue;
}

int Accelerator::getNumOutputQueue() const {
    return Accelerator::numOutputQueue;
}

size_t Accelerator::getNumBytesRead(uint8_t idQueue) const {
    auto *dsm = (uint32_t *) Accelerator::getDsm();
    auto bytes = static_cast<size_t>((dsm[idQueue]) << 6);
    return bytes;
}

size_t Accelerator::getNumBytesWrite(uint8_t idQueue) const {
    auto *dsm = (uint32_t *) Accelerator::getDsm();
    auto bytes = static_cast<size_t>((dsm[idQueue + Accelerator::getNumInputQueue()]) << 6);
    return bytes;
}

bool Accelerator::isDoneInputQueue(uint8_t idQueue) const {
    auto dsmNumCL = Accelerator::getDsmSize() / 64;
    auto *doneVet = (uint64_t *) (Accelerator::getDsm());
    int col = idQueue >> 6;
    int bit = idQueue % 64;
    uint64_t doneDword = 0;
    doneDword = doneVet[GET_INDEX(dsmNumCL - 1, col, 8)];
    return (doneDword & (2 << bit)) == (2 << bit);
}

bool Accelerator::isDoneOutputQueue(uint8_t idQueue) const {
    auto dsmNumCL = Accelerator::getDsmSize() / 64;
    auto *doneVet = (uint64_t *) (Accelerator::getDsm());
    idQueue += Accelerator::getNumInputQueue();
    int col = idQueue >> 6;
    int bit = idQueue % 64;
    uint64_t doneDword = 0;
    doneDword = doneVet[GET_INDEX(dsmNumCL - 1, col, 8)];
    return (doneDword & (2 << bit)) == (2 << bit);
}


void Accelerator::waitDone(int64_t timeWaitMax) {
    bool flagNoMaxTime = false;
    struct timespec pause{};
    pause.tv_sec = 0;
    pause.tv_nsec = 500000;
    if (timeWaitMax == 0) {
        flagNoMaxTime = true;
        timeWaitMax = 1;
    }
    timeWaitMax *= 2;
    while (timeWaitMax > 0 && !Accelerator::isDone()) {
        if (!flagNoMaxTime)timeWaitMax--;
        nanosleep(&pause, nullptr);
    };
    Accelerator::stop();
}

bool Accelerator::isDone() const {
    auto dsmNumCL = Accelerator::getDsmSize() / 64;
    auto *done = (int *) Accelerator::getDsm();
    return (done[GET_INDEX(dsmNumCL - 1, 0, 16)] & 1) == 1L;
}

accid_t Accelerator::getId() const {
    return Accelerator::id;
}

void Accelerator::printInf() {
    auto dsmNumCL = Accelerator::Accelerator::getDsmSize() / 64;
    auto * dsm = static_cast<uint32_t *>(Accelerator::getDsm());
    MSG("ACC " << Accelerator::getId() << " DSM:");
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

void Accelerator::clear() {
    delete Accelerator::sizeOfInputQueues;
    delete Accelerator::sizeOfOutputQueues;
    Accelerator::sizeOfInputQueues = nullptr;
    Accelerator::sizeOfOutputQueues = nullptr;
    for (int i = 0; i < Accelerator::numInputQueue; ++i) {
        if (nullptr != Accelerator::inputQueue[i]) {
            Accelerator::accManagement.fpgaFreeQueue(Accelerator::inputQueue[i]);
            Accelerator::inputQueue[i] = nullptr;
        }
    }
    for (int i = 0; i < Accelerator::numOutputQueue; ++i) {
        if (nullptr != Accelerator::outputQueue[i]) {
            Accelerator::accManagement.fpgaFreeQueue(Accelerator::outputQueue[i]);
            Accelerator::outputQueue[i] = nullptr;
        }
    }
}

int Accelerator::getInputQueueGlobalID(uint8_t idQueue) {
    int globalID = 0;
    auto accs = Accelerator::accManagement.getAccelerators();
    accid_t id = Accelerator::getId();
    for (auto &acc : accs){
        if(id == acc.second->getId()) break;
        globalID += acc.second->getNumInputQueue();
    }
    return globalID+idQueue;
}
int Accelerator::getOutputQueueGlobalID(uint8_t idQueue) {
    int globalID = 0;
    auto &accs = Accelerator::accManagement.getAccelerators();
    accid_t id = Accelerator::getId();
    for (auto &acc : accs){
        if(id == acc.second->getId()) break;
        globalID += acc.second->getNumOutputQueue();
    }
    return globalID+idQueue;
}

Accelerator &Accelerator::operator=(const Accelerator &Accelerator) {
    Accelerator::accManagement = Accelerator.accManagement;
    Accelerator::id = Accelerator.id;
    Accelerator::numInputQueue = Accelerator.numInputQueue;
    Accelerator::numOutputQueue = Accelerator.numOutputQueue;
    Accelerator::inputQueue = Accelerator.inputQueue;
    Accelerator::outputQueue = Accelerator.outputQueue;
    return *this;
}
