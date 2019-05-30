#include "paeth.h"

int main(int argc, char *argv[]) {

    int idx = 0;
    int test = 0;
     
    if(argc > 1)
        test = atoi(argv[1]);
     
    if(argc > 2)
        idx = atoi(argv[2]);
    
    if(test & 1)
        paeth(idx);
    if(test & 2)
        paeth_openmp(idx);
    if(test & 4)
        paeth_cgra(idx, 2);

    return 0;
}

int paeth(int idx) {

    unsigned short *a, *b, *c, *out;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k % 1024;
        b[k] = k % 1024;
        c[k] = k % 1024;
        out[k] = 0;
    }

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    for (int i = 0; i < 100; i++) {
        s = high_resolution_clock::now();
        for (int k = 0; k < DATA_SIZE; ++k) {
            int pas, pbs, pcs;
            bool test_1, test_2, test_3, test_4;
            pas = b[k] - c[k];
            pbs = a[k] - c[k];
            pcs = a[k] | (b[k] - 2 * c[k]);
            test_1 = abs(pas) <= abs(pbs);
            test_2 = abs(pas) <= abs(pcs);
            test_4 = abs(pbs) <= abs(pcs);
            test_3 = test_1 && test_2;
            if (test_3) out[k] = a[k];
            else if (test_4) out[k] = b[k];
            else out[k] = c[k];
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / 100;

    printf("Time(ms) CPU 1 Thread: %5.2lf\n", cpuExecTime);

    int v = out[idx];

    delete a;
    delete b;
    delete c;
    delete out;

    return v;

}

int paeth_openmp(int idx) {

    unsigned short *a, *b, *c, *out;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k % 1024;
        b[k] = k % 1024;
        c[k] = k % 1024;
        out[k] = 0;
    }
    omp_set_num_threads(NUM_THREAD);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    for (int i = 0; i < 100; i++) {
        s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
        for (int k = 0; k < DATA_SIZE; ++k) {
            int pas, pbs, pcs;
            bool test_1, test_2, test_3, test_4;
            pas = b[k] - c[k];
            pbs = a[k] - c[k];
            pcs = a[k] | (b[k] - 2 * c[k]);
            test_1 = abs(pas) <= abs(pbs);
            test_2 = abs(pas) <= abs(pcs);
            test_4 = abs(pbs) <= abs(pcs);
            test_3 = test_1 && test_2;
            if (test_3) out[k] = a[k];
            else if (test_4) out[k] = b[k];
            else out[k] = c[k];
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / 100;

    printf("Time(ms) CPU %d Thread: %5.2lf\n", NUM_THREAD, cpuExecTime);

    int v = out[idx];

    delete a;
    delete b;
    delete c;
    delete out;

    return v;
}

int paeth_cgra(int idx, int copies) {
    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
    auto cgraHw = new Cgra();
    Scheduler scheduler(cgraArch);
    std::vector<DataFlow *> dfs;
    unsigned short *a, *b, *c, *out;
    int r = 0, v = 0,tries = 0;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k % 1024;
        b[k] = k % 1024;
        c[k] = k % 1024;
        out[k] = 0;
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
                cgraHw->setCgraProgramInputStreamByID(i, (j * 4), &a[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 1, &b[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 2, &c[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramOutputStreamByID(i, (j * 4) + 3, &out[k * data_size], data_size_bytes);
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
        v = out[idx];

    } else {
        printf("Scheduler Error: %d\n", r);
    }

    delete cgraArch;
    delete cgraHw;
    delete[] a;
    delete[] b;
    delete[] c;
    delete[] out;

    for (auto df:dfs) {
        delete df;
    }
    return v;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "paeth");
    int idx = 0;
    Operator *inA[2];
    Operator *inB[2];
    Operator *inC[2];
    Operator *out[2];
    for (int i = 0; i < copies; ++i) {
        inA[i] = new InputStream(idx++);
        inB[i] = new InputStream(idx++);
        inC[i] = new InputStream(idx++);
        out[i] = new OutputStream(idx++);
    }
    for (int i = 0; i < copies; ++i) {
        auto sub1 = new Sub(idx++);
        auto sub2 = new Sub(idx++);
        auto sub3 = new Sub(idx++);
        auto m1 = new Multi(idx++, 2);
        auto or1 = new Or(idx++);
        auto sgt1 = new Sgt(idx++);
        auto sgt2 = new Sgt(idx++);
        auto sgt3 = new Sgt(idx++);
        auto mux1 = new Mux(idx++);
        auto mux2 = new Mux(idx++);
        auto mux3 = new Mux(idx++);
        auto regUm = new PassBi(idx++, 1);
        auto regZero = new PassBi(idx++, 0);
        auto and0 = new And(idx++);
        auto sub1reg1 = new PassB(idx++);
        auto sub1reg2 = new PassB(idx++);
        auto sub2reg1 = new PassB(idx++);
        auto sub2reg2 = new PassB(idx++);
        auto pas = new Abs(idx++);
        auto pbs = new Abs(idx++);
        auto pcs = new Abs(idx++);
        auto beq1 = new Beqi(idx++, 1);
        auto beq2 = new Beqi(idx++, 1);
        auto muxCB = new Mux(idx++);
        auto regA1 = new PassB(idx++);
        auto regA2 = new PassB(idx++);
        auto regA3 = new PassB(idx++);
        auto regA4 = new PassB(idx++);
        auto regA5 = new PassB(idx++);
        auto regA6 = new PassB(idx++);
        auto regA7 = new PassB(idx++);
        auto regA8 = new PassB(idx++);
        auto regA9 = new PassB(idx++);
        auto regB1 = new PassB(idx++);
        auto regB2 = new PassB(idx++);
        auto regB3 = new PassB(idx++);
        auto regB4 = new PassB(idx++);
        auto regB5 = new PassB(idx++);
        auto regB6 = new PassB(idx++);
        auto regB7 = new PassB(idx++);
        auto regC1 = new PassB(idx++);
        auto regC2 = new PassB(idx++);
        auto regC3 = new PassB(idx++);
        auto regC4 = new PassB(idx++);
        auto regC5 = new PassB(idx++);
        auto regC6 = new PassB(idx++);
        auto regC7 = new PassB(idx++);
        auto muxCBA = new Mux(idx++);
        auto regAnd0 = new PassA(idx++);
        auto regMuxCB = new PassA(idx++);

        df->connect(inA[i], regA1, regA1->getPortB());
        df->connect(regA1, regA2, regA2->getPortB());
        df->connect(regA2, regA3, regA3->getPortB());
        df->connect(regA3, regA4, regA4->getPortB());
        df->connect(regA4, regA5, regA5->getPortB());
        df->connect(regA5, regA6, regA6->getPortB());
        df->connect(regA6, regA7, regA7->getPortB());
        df->connect(regA7, regA8, regA8->getPortB());
        df->connect(regA8, regA9, regA9->getPortB());
        df->connect(inB[i], regB1, regB1->getPortB());
        df->connect(regB1, regB2, regB2->getPortB());
        df->connect(regB2, regB3, regB3->getPortB());
        df->connect(regB3, regB4, regB4->getPortB());
        df->connect(regB4, regB5, regB5->getPortB());
        df->connect(regB5, regB6, regB6->getPortB());
        df->connect(regB6, regB7, regB7->getPortB());
        df->connect(inC[i], regC1, regC1->getPortB());
        df->connect(regC1, regC2, regC2->getPortB());
        df->connect(regC2, regC3, regC3->getPortB());
        df->connect(regC3, regC4, regC4->getPortB());
        df->connect(regC4, regC5, regC5->getPortB());
        df->connect(regC5, regC6, regC6->getPortB());
        df->connect(regC6, regC7, regC7->getPortB());
        df->connect(inA[i], sub1, sub1->getPortA());
        df->connect(inC[i], sub1, sub1->getPortB());
        df->connect(inB[i], sub2, sub2->getPortA());
        df->connect(inC[i], sub2, sub2->getPortB());
        df->connect(inC[i], m1, m1->getPortA());
        df->connect(regB1, sub3, sub3->getPortA());
        df->connect(m1, sub3, sub3->getPortB());
        df->connect(regA2, or1, or1->getPortA());
        df->connect(sub3, or1, sub3->getPortB());
        df->connect(sub1, sub1reg1, sub1reg1->getPortB());
        df->connect(sub1reg1, sub1reg2, sub1reg2->getPortB());
        df->connect(sub2, sub2reg1, sub2reg1->getPortB());
        df->connect(sub2reg1, sub2reg2, sub2reg2->getPortB());
        df->connect(or1, pcs, pcs->getPortA());
        df->connect(sub1reg2, pbs, pbs->getPortA());
        df->connect(sub2reg2, pas, pas->getPortA());
        df->connect(pbs, sgt1, sgt1->getPortA());
        df->connect(pcs, sgt1, sgt1->getPortB());
        df->connect(pas, sgt2, sgt2->getPortA());
        df->connect(pcs, sgt2, sgt2->getPortB());
        df->connect(pas, sgt3, sgt3->getPortA());
        df->connect(pbs, sgt3, sgt3->getPortB());
        df->connect(regZero, mux1, mux1->getPortA());
        df->connect(regUm, mux1, mux1->getPortB());
        df->connect(sgt1, mux1, mux1->getPortBranch());
        df->connect(regZero, mux2, mux2->getPortA());
        df->connect(regUm, mux2, mux2->getPortB());
        df->connect(sgt2, mux2, mux2->getPortBranch());
        df->connect(regZero, mux3, mux3->getPortA());
        df->connect(regUm, mux3, mux3->getPortB());
        df->connect(sgt3, mux3, mux3->getPortBranch());
        df->connect(mux2, and0, and0->getPortA());
        df->connect(mux3, and0, and0->getPortB());
        df->connect(and0, regAnd0, regAnd0->getPortA());
        df->connect(mux1, beq1, beq1->getPortA());
        df->connect(regAnd0, beq2, beq2->getPortA());
        df->connect(regB7, muxCB, muxCB->getPortA());
        df->connect(regC7, muxCB, muxCB->getPortB());
        df->connect(beq1, muxCB, muxCB->getPortBranch());
        df->connect(muxCB, regMuxCB, regMuxCB->getPortA());
        df->connect(regMuxCB, muxCBA, muxCBA->getPortB());
        df->connect(beq2, muxCBA, muxCBA->getPortBranch());
        df->connect(regA9, muxCBA, muxCBA->getPortA());
        df->connect(muxCBA, out[i], out[i]->getPortA());
    }

    return df;
}
