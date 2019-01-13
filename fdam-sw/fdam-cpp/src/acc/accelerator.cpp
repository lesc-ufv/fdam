#include <fdam/acc/accelerator.h>
#include <fdam/acc/acc_management.h>

Accelerator::Accelerator(AccManagement_t &accManagement, unsigned short id, int numInputQueue, int numOutputQueue) :
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

    for (int i = 0; i < Accelerator::getNumInputQueue(); ++i) {
        Accelerator::inputQueue[i] = nullptr;
        Accelerator::sizeOfInputQueues[i] = 0;
        Accelerator::sendConfIn(static_cast<unsigned char>(i));
    }
    for (int i = 0; i < Accelerator::getNumOutputQueue(); ++i) {
        Accelerator::outputQueue[i] = nullptr;
        Accelerator::sizeOfOutputQueues[i] = 0;
        Accelerator::sendConfOut(static_cast<unsigned char>(i));
    }
    Accelerator::createDsm();
}

Accelerator::~Accelerator() {
    Accelerator::clear();
};

void Accelerator::createDsm() {
    double size = CL(
            1.0 + ceil(Accelerator::getNumInputQueue() / 16.0) + ceil(Accelerator::getNumOutputQueue() / 16.0));
    auto nBytesAlign = static_cast<size_t>((std::ceil(size / 256.0)) * 256.0);
    Accelerator::setDsmSize(static_cast<size_t>(nBytesAlign));
    Accelerator::ptrDsm = Accelerator::accManagement.accAllocQueue(Accelerator::getDsmSize());
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
    auto numCl = static_cast<unsigned long>(Accelerator::getDsmSize() / 64);
    auto val_low = static_cast<unsigned long>(numCl << 32 | idQueueGlobal);
    auto val_high = static_cast<unsigned long>(intptr_t(Accelerator::getDsm()) / 64);
    Accelerator::accManagement.writeCSR(REG_CONF_DSM_LOW, val_low);
    Accelerator::accManagement.writeCSR(REG_CONF_DSM_HIGH, val_high);
}

void Accelerator::clearDsm() {
    std::memset(Accelerator::getDsm(), 0x00, Accelerator::getDsmSize());
}

void Accelerator::sendConfIn(unsigned char idQueue) {
    int idQueueGlobal = Accelerator::getInputQueueGlobalID(idQueue);
    auto numCl = static_cast<unsigned long>(Accelerator::getSizeOfInputQueue(idQueue) / 64);
    auto val_low = numCl << 32L | idQueueGlobal;
    auto val_high = static_cast<unsigned long>(intptr_t(Accelerator::getInputQueue(idQueue)) / 64);
    Accelerator::accManagement.writeCSR(REG_CONF_IN_LOW, val_low);
    Accelerator::accManagement.writeCSR(REG_CONF_IN_HIGH, val_high);
}

void Accelerator::sendConfOut(unsigned char idQueue) {
    int idQueueGlobal = Accelerator::getOutputQueueGlobalID(idQueue);
    auto numCl = static_cast<unsigned long>(Accelerator::getSizeOfOutputQueue(idQueue) / 64);
    auto val_low = numCl << 32L | idQueueGlobal;
    auto val_high = static_cast<unsigned long>(intptr_t(Accelerator::getOutputQueue(idQueue)) / 64);
    Accelerator::accManagement.writeCSR(REG_CONF_OUT_LOW, val_low);
    Accelerator::accManagement.writeCSR(REG_CONF_OUT_HIGH, val_high);
}

void *Accelerator::getInputQueue(unsigned char idQueue) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue()) {
        return Accelerator::inputQueue[idQueue];
    }
    return nullptr;
}

void *Accelerator::getOutputQueue(unsigned char idQueue) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue()) {
        return Accelerator::outputQueue[idQueue];
    }
    return nullptr;
}

void Accelerator::setInputQueue(unsigned char idQueue, void *ptrQueue, long long numBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue()) {
        Accelerator::inputQueue[idQueue] = ptrQueue;
        Accelerator::sizeOfInputQueues[idQueue] = numBytes;
    }
}

void Accelerator::setOutputQueue(unsigned char idQueue, void *ptrQueue, long long numBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue()) {
        Accelerator::outputQueue[idQueue] = ptrQueue;
        Accelerator::sizeOfOutputQueues[idQueue] = numBytes;
    }
}

void Accelerator::start() {
    Accelerator::clearDsm();
    Accelerator::accManagement.startAccelerators(static_cast<const unsigned long>(1L << Accelerator::getId()));
}

void Accelerator::stop() {
    Accelerator::accManagement.stopAccelerators(static_cast<const unsigned long>(1L << Accelerator::getId()));
}

void Accelerator::reset() {
    Accelerator::clearDsm();
    Accelerator::accManagement.resetAccelerators(static_cast<const unsigned long>(1L << Accelerator::getId()));
    Accelerator::accManagement.resetAccelerators(0L);
    Accelerator::sendConfDsm();
}

bool Accelerator::createInputQueue(unsigned char idQueue, long long nBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue()) {
        double nb = nBytes;
        auto nBytesAlign = static_cast<long long>((std::ceil(nb / 256.0)) * 256.0);
        void *ptrQueue = Accelerator::accManagement.accAllocQueue(nBytesAlign);
        std::memset(ptrQueue, 0x00, static_cast<size_t>(nBytesAlign));
        if (Accelerator::getInputQueue(idQueue) != nullptr) {
            Accelerator::accManagement.accFreeQueue(Accelerator::getInputQueue(idQueue));
        }
        Accelerator::setInputQueue(idQueue, ptrQueue, nBytesAlign);
        Accelerator::sendConfIn(idQueue);
        return true;
    }
    return false;
}

bool Accelerator::createOutputQueue(unsigned char idQueue, long long nBytes) {
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue()) {
        double nb = nBytes;
        auto nBytesAlign = static_cast<long long>((std::ceil(nb / 256.0)) * 256.0);
        void *ptrQueue = Accelerator::accManagement.accAllocQueue(nBytesAlign);
        std::memset(ptrQueue, 0x00, static_cast<size_t>(nBytesAlign));
        if (Accelerator::getOutputQueue(idQueue) != nullptr) {
            Accelerator::accManagement.accFreeQueue(Accelerator::getOutputQueue(idQueue));
        }
        Accelerator::setOutputQueue(idQueue, ptrQueue, nBytesAlign);
        Accelerator::sendConfOut(idQueue);
        return true;
    }
    return false;
}

bool Accelerator::copyToInputQueue(unsigned char idQueue, const signed char *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfInputQueue(idQueue) * sizeof(signed char);
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<signed char *>(Accelerator::getInputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            accData[i] = data[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyToInputQueue(unsigned char idQueue, const short *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfInputQueue(idQueue) * sizeof(short);
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<short *>(Accelerator::getInputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            accData[i] = data[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyToInputQueue(unsigned char idQueue, const int *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfInputQueue(idQueue) * sizeof(int);
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<int *>(Accelerator::getInputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            accData[i] = data[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyToInputQueue(unsigned char idQueue, const long long *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfInputQueue(idQueue) * sizeof(long long);
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<long *>(Accelerator::getInputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            accData[i] = data[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyFromInputQueue(unsigned char idQueue, signed char *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfInputQueue(idQueue) * sizeof(signed char);
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<signed char *>(Accelerator::getInputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            data[i] = accData[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyFromInputQueue(unsigned char idQueue, short *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfInputQueue(idQueue) * sizeof(short);
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<short *>(Accelerator::getInputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            data[i] = accData[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyFromInputQueue(unsigned char idQueue, int *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfInputQueue(idQueue) * sizeof(int);
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<int *>(Accelerator::getInputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            data[i] = accData[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyFromInputQueue(unsigned char idQueue, long long *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfInputQueue(idQueue) * sizeof(long long);
    if (idQueue >= 0 && idQueue < Accelerator::getNumInputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<long *>(Accelerator::getInputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            data[i] = accData[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyToOutputQueue(unsigned char idQueue, const signed char *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfOutputQueue(idQueue) * sizeof(signed char);
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<signed char *>(Accelerator::getOutputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            accData[i] = data[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyToOutputQueue(unsigned char idQueue, const short *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfOutputQueue(idQueue) * sizeof(short);
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<short *>(Accelerator::getOutputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            accData[i] = data[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyToOutputQueue(unsigned char idQueue, const int *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfOutputQueue(idQueue) * sizeof(int);
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<int *>(Accelerator::getOutputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            accData[i] = data[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyToOutputQueue(unsigned char idQueue, const long long *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfOutputQueue(idQueue) * sizeof(long long);
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<long *>(Accelerator::getOutputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            accData[i] = data[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyFromOutputQueue(unsigned char idQueue, signed char *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfOutputQueue(idQueue) * sizeof(signed char);
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<signed char *>(Accelerator::getOutputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            data[i] = accData[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyFromOutputQueue(unsigned char idQueue, short *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfOutputQueue(idQueue) * sizeof(short);
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<short *>(Accelerator::getOutputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            data[i] = accData[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyFromOutputQueue(unsigned char idQueue, int *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfOutputQueue(idQueue) * sizeof(int);
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<int *>(Accelerator::getOutputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            data[i] = accData[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::copyFromOutputQueue(unsigned char idQueue, long long *data, long long ndata) {
    long long ndataQueue = Accelerator::getSizeOfOutputQueue(idQueue) * sizeof(long long);
    if (idQueue >= 0 && idQueue < Accelerator::getNumOutputQueue() && (ndata <= ndataQueue)) {
        auto *accData = static_cast<long *>(Accelerator::getOutputQueue(idQueue));
        for (int i = 0; i < ndata; ++i) {
            data[i] = accData[i];
        }
        return true;
    }
    return false;
}

bool Accelerator::setNumBytesRead(unsigned char idQueue, long long nBytes) {
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

bool Accelerator::setNumBytesWrite(unsigned char idQueue, long long nBytes) {
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

long long Accelerator::getSizeOfInputQueue(unsigned char idQueue) const {
    return Accelerator::sizeOfInputQueues[idQueue];
}

long long Accelerator::getSizeOfOutputQueue(unsigned char idQueue) const {
    return Accelerator::sizeOfOutputQueues[idQueue];
}

int Accelerator::getNumInputQueue() const {
    return Accelerator::numInputQueue;
}

int Accelerator::getNumOutputQueue() const {
    return Accelerator::numOutputQueue;
}

size_t Accelerator::getNumBytesRead(unsigned char idQueue) const {
    auto *dsm = (unsigned int *) Accelerator::getDsm();
    auto bytes = static_cast<size_t>((dsm[idQueue]) << 6);
    return bytes;
}

size_t Accelerator::getNumBytesWrite(unsigned char idQueue) const {
    auto *dsm = (unsigned int *) Accelerator::getDsm();
    auto bytes = static_cast<size_t>((dsm[idQueue + Accelerator::getNumInputQueue()]) << 6);
    return bytes;
}

bool Accelerator::isDoneInputQueue(unsigned char idQueue) const {
    auto dsmNumCL = Accelerator::getDsmSize() / 64;
    auto *doneVet = (unsigned long *) (Accelerator::getDsm());
    int col = idQueue >> 6;
    int bit = idQueue % 64;
    unsigned long doneDword = 0;
    doneDword = doneVet[GET_INDEX(dsmNumCL - 1, col, 8)];

    return (doneDword & (1UL << bit));
}

bool Accelerator::isDoneOutputQueue(unsigned char idQueue) const {

    auto dsmNumCL = Accelerator::getDsmSize() / 64;
    auto *doneVet = (unsigned long *) (Accelerator::getDsm());

    idQueue += Accelerator::getNumInputQueue();
    int col = idQueue >> 6;
    int bit = idQueue % 64;
    unsigned long doneDword = 0;
    doneDword = doneVet[GET_INDEX(dsmNumCL - 1, col, 8)];

    return (doneDword & (1UL << bit));
}

void Accelerator::waitDone(long long timeWaitMax) {
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

    for (int i = 0; i < dsmNumCL; i++) {
        if (!(done[GET_INDEX(i, 16 - 1, 16)] & 0x80000000UL)) {
            return false;
        }
    }

    return true;
}

unsigned short Accelerator::getId() const {
    return Accelerator::id;
}

long Accelerator::getInputQueueBytesRead(unsigned char idQueue) {
    auto *dsm = static_cast<unsigned int *>(Accelerator::getDsm());
    return dsm[idQueue] * 64;
}

long Accelerator::getOutputQueueWrittenBytes(unsigned char idQueue) {
    auto dsm = static_cast<unsigned int *>(Accelerator::getDsm());
    int numIn = Accelerator::getNumInputQueue();
    auto offset = static_cast<int>(std::ceil(numIn / 16.0)) * 16;

    return dsm[idQueue + offset] * 64;
}


void Accelerator::printHwInfo() {

    int numIn = Accelerator::getNumInputQueue();
    int numOut = Accelerator::getNumOutputQueue();
    int id = Accelerator::getId();

    MSG("INFO Accelerator:");
    MSG("ID: " << id);
    MSG("Amount of input queues: " << numIn);
    MSG("Amount of output queues: " << numOut);
    if (Accelerator::isDone()) {
        MSG("Status: Done");
    } else {
        MSG("Status: Not done");
    }
    MSG("INFO Input Queues:");
    for (unsigned char i = 0; i < numIn; i++) {
        if (Accelerator::isDoneInputQueue(i)) {
            MSG("Queue ID: " << static_cast<int >(i) << ", State: Done, Bytes read: " << getInputQueueBytesRead(i));
        } else {
            MSG("Queue ID: " << static_cast<int >(i) << ", State: Not done, Bytes read: " << getInputQueueBytesRead(i));
        }
    }
    MSG("INFO Output Queues:");
    for (unsigned char i = 0; i < numOut; i++) {
        if (Accelerator::isDoneOutputQueue(i)) {
            MSG("Queue ID: " << static_cast<int >(i) << " State: Done, Written bytes: "
                             << getOutputQueueWrittenBytes(i));
        } else {
            MSG("Queue ID: " << static_cast<int >(i) << " State: Not done, Written bytes: "
                             << getOutputQueueWrittenBytes(i));
        }
    }
}

void Accelerator::clear() {
    delete[] Accelerator::sizeOfInputQueues;
    delete[] Accelerator::sizeOfOutputQueues;
    Accelerator::sizeOfInputQueues = nullptr;
    Accelerator::sizeOfOutputQueues = nullptr;
    for (int i = 0; i < Accelerator::numInputQueue; ++i) {
        if (nullptr != Accelerator::inputQueue[i]) {
            Accelerator::accManagement.accFreeQueue(Accelerator::inputQueue[i]);
            Accelerator::inputQueue[i] = nullptr;
        }
    }
    for (int i = 0; i < Accelerator::numOutputQueue; ++i) {
        if (nullptr != Accelerator::outputQueue[i]) {
            Accelerator::accManagement.accFreeQueue(Accelerator::outputQueue[i]);
            Accelerator::outputQueue[i] = nullptr;
        }
    }
}

int Accelerator::getInputQueueGlobalID(unsigned char idQueue) {
    int globalID = 0;
    auto accs = Accelerator::accManagement.getAccelerators();
    unsigned short id = Accelerator::getId();
    for (auto &acc : accs) {
        if (id == acc.second->getId()) break;
        globalID += acc.second->getNumInputQueue();
    }
    return globalID + idQueue;
}

int Accelerator::getOutputQueueGlobalID(unsigned char idQueue) {
    int globalID = 0;
    auto &accs = Accelerator::accManagement.getAccelerators();
    unsigned short id = Accelerator::getId();
    for (auto &acc : accs) {
        if (id == acc.second->getId()) break;
        globalID += acc.second->getNumOutputQueue();
    }
    return globalID + idQueue;
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
