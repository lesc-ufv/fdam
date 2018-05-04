#include <iostream>
#include <cstring>
#include <chrono>
#include <AccManagement.h>

using namespace std;
using namespace std::chrono;

typedef uint64_t data_t;

void loopback(data_t *data_in, data_t *data_out, int num_dados);

int main(int argc, char *argv[]) {
    int num_dados = 0;
    if (argc > 1) {
        num_dados = (CL(1) / sizeof(data_t)) * atoi(argv[1]);
    } else {
        cout << "invalid args!!!" << endl;
        cout << "usage: <num cache lines>" << endl;
        exit(255);
    }

    auto *data_in = new data_t[num_dados];
    auto *data_out = new data_t[num_dados];

    for (int i = 0; i < num_dados; i++) {
        data_in[i] = static_cast<data_t>(i + 1);
    }

    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    loopback(data_in, data_out, num_dados);

    diff = high_resolution_clock::now() - s;
    MSG("Execution Time: " << diff.count() * 1000 << "ms");

    return 0;
}

void loopback(data_t *data_in, data_t *data_out, int num_dados) {

    auto *accMgr = new AccManagement();
    auto &acc = accMgr->getAccelerator(0);
    size_t numBytes = num_dados * sizeof(data_t);
    for (short j = 0; j < acc.getNumInputQueue(); j++) {
        acc.createInputQueue(static_cast<uint8_t>(j), numBytes, data_in);
        acc.createOutputQueue(static_cast<uint8_t>(j), numBytes);
    }
    acc.start();
    acc.waitDone(0);
    for (short j = 0; j < acc.getNumInputQueue(); j++) {
        if (memcmp(acc.getInputQueue(static_cast<uint8_t>(j)), acc.getOutputQueue(static_cast<uint8_t>(j)), numBytes) ==
            0) {
            BEGIN_COLOR(GREEN);
            MSG("Success!");
            END_COLOR();
        } else {
            BEGIN_COLOR(RED);
            MSG("Error!");
            END_COLOR();
        }
    }
    if (acc.isDone()) {
        MSG("ACC " << acc.getId() << " DONE!");
    } else {
        MSG("ACC " << acc.getId() << " NOT DONE!");
    }
    acc.printInf();
    accMgr->printHwInfo();

    delete accMgr;
}
