#include <main.h>

int main(int argc, char *argv[]) {

    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    int64 n = 100000;

    if (argc > 1) {
        n = atoi(argv[1]) * 16;
    } else {
        cout << "invalid args!!!" << endl;
        cout << "usage: <num lines>" << endl;
        exit(255);
    }
    auto *accMgr = new AccManagement();

    Accelerator &acc = accMgr->getAccelerator(0);

    for (int f = 0; f < 32; f++) {

        acc.createInputQueue(f, sizeof(int) * n);
        acc.createOutputQueue(f, sizeof(int) * n);

        auto in = (int *) acc.getInputQueue(f);
        auto out = (int *) acc.getOutputQueue(f);

        for (int64 i = 0; i < n; i++) {
            in[i] = i + 1;
            out[i] = 0;
        }
    }

    high_resolution_clock::time_point s1;
    duration<double> diff1{};
    s1 = high_resolution_clock::now();

    acc.start();
    acc.waitDone(0);

    diff1 = high_resolution_clock::now() - s1;

    acc.printHwInfo();
    accMgr->printHwInfo();

    int64 idx_error = -1;
    int Id = 0;
    for (int f = 0; f < 32; f++) {
        auto in = (int *) acc.getInputQueue(f);
        auto out = (int *) acc.getOutputQueue(f);

        for (int64 i = 0; i < n; i++) {
            if (in[i] != out[i]) {
                idx_error = i;
                break;
            }
        }
        if (idx_error >= 0) {
            Id = f;
            break;
        }
    }

    auto in = (int *) acc.getInputQueue(Id);
    auto out = (int *) acc.getOutputQueue(Id);

    diff = high_resolution_clock::now() - s;
    double timeExec = diff.count();
    double timeExec1 = diff1.count();
    double nBytes = (n + n) * sizeof(int);
    double nGbytes = nBytes / (1 << 30);
    double thpt = nGbytes / timeExec1;
    MSG("Total Time: " << timeExec * 1000 << "ms");
    MSG("Accelerator Time: " << timeExec1 * 1000 << "ms");
    MSG("Throughput: " << thpt << "GB/s");

    if (idx_error >= 0) {
        MSG("Loopback error at " << idx_error << " position, expect " << in[idx_error] << " found " << out[idx_error]);
    } else {
        MSG("Loopback pass!");
    }

    delete accMgr;
}
