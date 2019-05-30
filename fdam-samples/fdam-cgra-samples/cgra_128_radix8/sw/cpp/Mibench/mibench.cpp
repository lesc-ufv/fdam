#include "mibench.h"

int main(int argc, char *argv[]) {

    int idx = 0;
    int test = 0;
     
    if(argc > 1)
        test = atoi(argv[1]);
     
    if(argc > 2)
        idx = atoi(argv[2]);

    if(test & 1)
        mibench(idx);
    if(test & 2)
        mibench_openmp(idx);
    if(test & 4)
        mibench_cgra(idx, 2);

    return 0;
}

int mibench(int idx) {

    auto A = new unsigned short[DATA_SIZE];
    auto B = new unsigned short[DATA_SIZE];
    auto C = new unsigned short[DATA_SIZE];
    auto OUT = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        A[k] = k;
        B[k] = k;
        C[k] = k;
        OUT[k] = 0;
    }
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    for (int i = 0; i < 100; i++) {
        s = high_resolution_clock::now();
        for (int k = 0; k < DATA_SIZE; ++k) {
            OUT[k] = (unsigned short) (A[k] * (6 * A[k] + 43 + B[k]) + C[k] * (6 * A[k] + 43 + 2 * B[k] + C[k]) +
                                       A[k] * (9 * A[k] + 1));
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / 100;
    printf("Time(ms) CPU 1 Thread: %5.2lf\n", cpuExecTime);

    int v = OUT[idx];

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] OUT;

    return v;

}

int mibench_openmp(int idx) {

    auto A = new unsigned short[DATA_SIZE];
    auto B = new unsigned short[DATA_SIZE];
    auto C = new unsigned short[DATA_SIZE];
    auto OUT = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        A[k] = k;
        B[k] = k;
        C[k] = k;
        OUT[k] = 0;
    }
    omp_set_num_threads(NUM_THREAD);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    for (int i = 0; i < 100; i++) {
        s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
        for (int k = 0; k < DATA_SIZE; ++k) {
            OUT[k] = (unsigned short) (A[k] * (6 * A[k] + 43 + B[k]) + C[k] * (6 * A[k] + 43 + 2 * B[k] + C[k]) +
                                       A[k] * (9 * A[k] + 1));
        }

        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / 100;
    printf("Time(ms) CPU %d Thread: %5.2lf\n", NUM_THREAD, cpuExecTime);

    int v = OUT[idx];

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] OUT;

    return v;


}

int mibench_cgra(int idx, int copies) {
    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
    auto cgraHw = new Cgra();
    Scheduler scheduler(cgraArch);
    std::vector<DataFlow *> dfs;

    auto A = new unsigned short[DATA_SIZE];
    auto B = new unsigned short[DATA_SIZE];
    auto C = new unsigned short[DATA_SIZE];
    auto OUT = new unsigned short[DATA_SIZE];

    int r = 0, v = 0, tries = 0;

    for (int k = 0; k < DATA_SIZE; ++k) {
        A[k] = k;
        B[k] = k;
        C[k] = k;
        OUT[k] = 0;
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
    } while (r != SCHEDULE_SUCCESS && tries < 1000);
    
    if (r == SCHEDULE_SUCCESS) {

        cgraHw->loadCgraProgram(cgraArch->getCgraProgram());

        auto data_size = (size_t) (DATA_SIZE / ((NUM_THREAD) * copies));
        auto data_size_bytes = sizeof(unsigned short) * data_size;
        int k = 0;
        for (int i = 0; i < NUM_THREAD; ++i) {
            for (int j = 0; j < copies; ++j) {
                cgraHw->setCgraProgramInputStreamByID(i, (j * 4), &A[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 1, &B[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 2, &C[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramOutputStreamByID(i, (j * 4) + 3, &OUT[k * data_size], data_size_bytes);
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
        v = OUT[idx];

    } else {
        printf("Scheduler Error: %d\n", r);
    }

    delete cgraArch;
    delete cgraHw;
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] OUT;

    for (auto df:dfs) {
        delete df;
    }
    return v;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "mibench");
    int idx = 0;
    std::vector<Operator *> in1;
    std::vector<Operator *> in2;
    std::vector<Operator *> in3;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in1.push_back(new InputStream(idx++));
        in2.push_back(new InputStream(idx++));
        in3.push_back(new InputStream(idx++));
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
        auto reg8 = new PassA(idx++);

        auto reg9 = new PassA(idx++);
        auto reg10 = new PassA(idx++);

        auto mult1 = new Multi(idx++, 9);
        auto mult2 = new Multi(idx++, 6);
        auto mult3 = new Multi(idx++, 2);

        auto add1 = new Addi(idx++, 1);
        auto add2 = new Addi(idx++, 43);
        auto add3 = new Add(idx++);

        auto mult4 = new Mult(idx++);
        auto add4 = new Add(idx++);
        auto add5 = new Add(idx++);

        auto mult5 = new Mult(idx++);
        auto mult6 = new Mult(idx++);
        auto add6 = new Add(idx++);
        auto add7 = new Add(idx++);

        df->connect(in3[i], mult1, mult1->getPortA());
        df->connect(in3[i], mult2, mult2->getPortA());
        df->connect(in3[i], reg1, reg1->getPortA());
        df->connect(reg1, reg2, reg2->getPortA());

        df->connect(in1[i], mult3, mult3->getPortA());
        df->connect(in1[i], reg3, reg3->getPortA());
        df->connect(reg3, reg4, reg4->getPortA());
        df->connect(reg4, reg5, reg5->getPortA());

        df->connect(in2[i], reg6, reg6->getPortA());
        df->connect(reg6, reg7, reg7->getPortA());
        df->connect(reg7, reg8, reg8->getPortA());

        df->connect(mult1, add1, add1->getPortA());
        df->connect(mult2, add2, add2->getPortA());
        df->connect(mult3, add3, add3->getPortA());
        df->connect(reg6, add3, add3->getPortB());

        df->connect(add1, mult4, mult4->getPortA());
        df->connect(reg2, mult4, mult4->getPortB());

        df->connect(add2, add4, add4->getPortA());
        df->connect(reg4, add4, add4->getPortB());

        df->connect(add3, add5, add5->getPortA());
        df->connect(add2, add5, add5->getPortB());

        df->connect(add4, mult5, mult5->getPortA());
        df->connect(reg5, mult5, mult5->getPortB());

        df->connect(add5, mult6, mult6->getPortA());
        df->connect(reg8, mult6, mult6->getPortB());

        df->connect(mult5, add6, add6->getPortA());
        df->connect(mult6, add6, add6->getPortB());

        df->connect(add6, add7, add7->getPortA());
        df->connect(mult4, reg9, reg9->getPortA());
        df->connect(reg9, reg10, reg10->getPortA());
        df->connect(reg10, add7, add7->getPortB());

        df->connect(add7, out[i], out[i]->getPortA());
    }
    return df;
}

