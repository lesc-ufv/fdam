#include "chebyshev.h"

int main(int argc, char *argv[]) {

    int idx = 0;
    int test= 0;
    if (argc > 1)
        test = atoi(argv[1]);
    if (argc > 2)
        idx = atoi(argv[2]);
    
    if(test & 1)
        chebyshev(idx);
    if(test & 2)
        chebyshev_openmp(idx);
    if(test & 4)
        chebyshev_cgra(idx, 8);

    return 0;
}

int chebyshev(int idx) {

    unsigned short *data_in, *data_out;

    data_in = new unsigned short[DATA_SIZE];
    data_out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        data_in[k] = k;
        data_out[k] = 0;
    }

    high_resolution_clock::time_point s;
    duration<double> diff = {};

    for (int i = 0; i < SAMPLES; i++) {
        s = high_resolution_clock::now();
        for (int k = 0; k < DATA_SIZE; ++k) {
            int A = data_in[k];
            data_out[k] = (unsigned short) (A * (A * (A * ((A * 16 * A) - 20)) + 5));
        }
        diff += high_resolution_clock::now() - s;
    }

    double cpuExecTime = (diff.count() * 1000) / SAMPLES;

    printf("Time(ms) CPU 1 Thread: %5.2lf\n", cpuExecTime);

    int v = data_out[idx];

    delete data_in;
    delete data_out;

    return v;

}

int chebyshev_openmp(int idx) {

    unsigned short *data_in, *data_out;

    data_in = new unsigned short[DATA_SIZE];
    data_out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        data_in[k] = k;
        data_out[k] = 0;
    }
    omp_set_num_threads(NUM_THREAD);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    for (int i = 0; i < SAMPLES; i++) {
        s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
        for (int k = 0; k < DATA_SIZE; ++k) {
            int A = data_in[k];
            data_out[k] = (unsigned short) (A * (A * (A * ((A * 16 * A) - 20)) + 5));
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / SAMPLES;

    printf("Time(ms) CPU %d Thread: %5.2lf\n", NUM_THREAD, cpuExecTime);

    int v = data_out[idx];

    delete data_in;
    delete data_out;

    return v;
}

int chebyshev_cgra(int idx, int copies) {

    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
    auto cgraHw = new Cgra();
    Scheduler scheduler(cgraArch);
    std::vector<DataFlow *> dfs;
    unsigned short *data_in, *data_out;
    data_in = new unsigned short[DATA_SIZE];
    data_out = new unsigned short[DATA_SIZE];
    int r = 0, v = 0, tries = 0;

    for (int k = 0; k < DATA_SIZE; ++k) {
        data_in[k] = k;
        data_out[k] = 0;
    }

    for (int i = 0; i < NUM_THREAD; ++i) {
        dfs.push_back(createDataFlow(i, copies));
        scheduler.addDataFlow(dfs[i], i, 0);
        cgraArch->getNetBranch(i)->createRouteTable();
        cgraArch->getNet(i)->createRouteTable();
    }

    do {
        r = scheduler.scheduling();
        tries++;
    } while (r != SCHEDULE_SUCCESS && tries < 10000);

    if (r == SCHEDULE_SUCCESS) {

        cgraHw->loadCgraProgram(cgraArch->getCgraProgram());

        auto data_size = (size_t) (DATA_SIZE / ((NUM_THREAD) * copies));
        auto data_size_bytes = sizeof(unsigned short) * data_size;

        int k = 0;
        for (int i = 0; i < NUM_THREAD; ++i) {
            for (int j = 0, c = 0; j < copies; ++j) {
                cgraHw->setCgraProgramInputStreamByID(i, c, &data_in[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramOutputStreamByID(i, c + 1, &data_out[k * data_size], data_size_bytes);
                c = c + 2;
                k++;
            }
        }
        double cgraExecTime = 0;
        for (int i = 0; i < SAMPLES; i++){
            cgraHw->syncExecute(0);
            cgraExecTime += cgraHw->getTimeExec();
        }
        cgraExecTime /= SAMPLES;
        printf("Time(ms) CGRA: %5.2lf\n", cgraExecTime);

        v = data_out[idx];

    } else {
        printf("Scheduler Error: %d\n", r);
    }

    delete cgraArch;
    delete cgraHw;
    delete data_in;
    delete data_out;
    for (auto df:dfs) {
        delete df;
    }
    return v;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "chebyshev");
    int idx = 0;
    std::vector<Operator *> in;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in.push_back(new InputStream(idx++));
        out.push_back(new OutputStream(idx++));
    }
    for (int i = 0; i < copies; ++i) {

        auto reg1 = new PassA(idx++);
        auto reg2 = new PassA(idx++);
        auto reg3 = new PassA(idx++);
        auto reg4 = new PassA(idx++);
        auto reg5 = new PassA(idx++);
        auto reg6 = new PassA(idx++);
        auto reg7 = new PassA(idx++);
        auto mult1 = new Multi(idx++, 16);
        auto mult2 = new Mult(idx++);
        auto sub1 = new Subi(idx++, 20);
        auto mult3 = new Mult(idx++);
        auto mult4 = new Mult(idx++);
        auto add1 = new Addi(idx++, 5);
        auto mult5 = new Mult(idx++);

        df->connect(in[i], mult1, mult1->getPortA());
        df->connect(in[i], reg1, reg1->getPortA());
        df->connect(reg1, reg2, reg2->getPortA());
        df->connect(reg2, reg5, reg5->getPortA());
        df->connect(reg5, reg3, reg3->getPortA());
        df->connect(reg3, reg6, reg6->getPortA());
        df->connect(reg6, reg4, reg4->getPortA());
        df->connect(reg1, mult2, mult2->getPortA());
        df->connect(mult1, mult2, mult2->getPortB());
        df->connect(mult2, sub1, sub1->getPortA());
        df->connect(reg2, reg7, reg7->getPortA());
        df->connect(reg7, mult3, mult3->getPortA());
        df->connect(sub1, mult3, mult3->getPortB());
        df->connect(reg3, mult4, mult4->getPortA());
        df->connect(mult3, mult4, mult4->getPortB());
        df->connect(mult4, add1, add1->getPortA());
        df->connect(reg4, mult5, mult5->getPortA());
        df->connect(add1, mult5, mult5->getPortB());
        df->connect(mult5, out[i], out[i]->getPortA());
    }
    return df;
}
