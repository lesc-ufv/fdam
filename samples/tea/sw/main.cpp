#include <iostream>
#include <cstring>
#include <chrono>
#include <AccManagement.h>

using namespace std;
using namespace std::chrono;


double tea(uint32_t ***data_in, uint32_t ***data_out, int num_data_in, int num_data_out);

int main(int argc, char *argv[]) {
    int num_data_in = 0;
    int num_data_out = 0;
    int num_copies = 0;
    if (argc > 2) {
        num_copies = atoi(argv[1]);
        num_data_in = (CL(1) / sizeof(uint16_t)) * atoi(argv[2]);
    } else {
        cout << "invalid args!!!" << endl;
        cout << "usage: <num copies> <num cache lines>" << endl;
        exit(255);
    }
    
    num_data_out = num_data_in;

    auto ***data_in = new uint32_t **[num_copies];
    auto ***data_out = new uint32_t **[num_copies];
    for (int k = 0; k < num_copies; ++k) {
        data_in[k] = new uint32_t *[num_copies];
        data_out[k] = new uint32_t *[num_copies];
        for (int i = 0; i < 2; ++i) {
            data_in[k][i] = new uint32_t[num_data_in];
            data_out[k][i] = new uint32_t[num_data_out];
            for (int j = 0; j < num_data_in; j ++) {
                if(i %2==0){
                    data_in[k][i][j] = static_cast<uint32_t>(0x2885842e);
                }else{
                    data_in[k][i][j] = static_cast<uint32_t>(0xa3e6999b);
                }
            }
            for (int j = 0; j < num_data_out; ++j) {
                data_out[k][i][j] = 0;
            }
        }
    }
    double timeExec = tea(data_in, data_out, num_data_in, num_data_out);
    double nBytes = (num_data_in + num_data_out) * sizeof(uint32_t);
    double nGbytes = nBytes / (1 << 30);
    double thpt = nGbytes / timeExec;
    MSG("Execution Time: " << timeExec * 1000 << "ms");
    MSG("Throughput: " << thpt << "GB/s");
    
    /*for (int k = 0; k < num_copies; ++k) {
        MSG("ACC " << k << ":");
        for (int i = 0; i < 2; ++i) {
            cout << "         OUT "<<i<<":";
            cout << " [ ";
            for (int j = 0; j < num_data_out; ++j) {
                cout << std::hex << data_out[k][i][j] << " ";
            }
            cout <<"]"<< endl;
        }
    }*/
    
    delete[](data_in);
    delete[](data_out);

    return 0;
}

double tea(uint32_t ***data_in, uint32_t ***data_out, int num_data_in, int num_data_out) {

    auto *accMgr = new AccManagement();
    size_t numBytesIn = num_data_in * sizeof(uint32_t);
    size_t numBytesOut = num_data_out * sizeof(uint32_t);
    for (auto &it:accMgr->getAccelerators()) {
        auto &acc = it.second;
        auto nIn = acc->getNumInputQueue();
        auto nOut = acc->getNumOutputQueue();
        for (short j = 0; j < nIn; ++j) {
            acc->createInputQueue(static_cast<uint32_t>(j), numBytesIn, data_in[acc->getId()][j]);
        }
        for (short j = 0; j < nOut; ++j) {
            acc->createOutputQueue(static_cast<uint32_t>(j), numBytesOut);
        }
    }

    auto accMask = static_cast<uint32_t>((1 << accMgr->getNumAccelerators()) - 1L);
    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    accMgr->startAccelerators(accMask);
    accMgr->waitDone(accMask, 0);

    diff = high_resolution_clock::now() - s;

    for (auto &it:accMgr->getAccelerators()) {
        auto &acc = it.second;
        for (short j = 0; j < acc->getNumOutputQueue(); j++) {
            acc->copyFromOutputQueue(static_cast<uint8_t>(j), data_out[acc->getId()][j], numBytesOut);
        }
        if (acc->isDone()) {
            MSG("ACC " << acc->getId() << " DONE!");
        } else {
            MSG("ACC " << acc->getId() << " NOT DONE!");
        }
        acc->printInf();
    }

    accMgr->printHwInfo();

    delete accMgr;

    return diff.count();
}
