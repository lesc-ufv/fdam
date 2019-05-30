#include "sgfilter.h"

int main(int argc, char *argv[]) {

    int idx = 0;
    int test = 0;
     
    if(argc > 1)
        test = atoi(argv[1]);
     
    if(argc > 2)
        idx = atoi(argv[2]);

    if(test & 1)
        sgfilter(idx);
    if(test & 2)
        sgfilter_openmp(idx);
    if(test & 4)    
        sgfilter_cgra(idx, 3);

    return 0;
}

int sgfilter(int idx) {

    auto a = new unsigned short[DATA_SIZE];
    auto b = new unsigned short[DATA_SIZE];
    auto OUT = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k % 1024;
        b[k] = k % 1024;
        OUT[k] = 0;
    }
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    for (int i = 0; i < SAMPLES; i++) {
        s = high_resolution_clock::now();
        for (int k = 0; k < DATA_SIZE; ++k) {
            OUT[k] = ((-984 * a[k] - 46) * a[k] - 46) * a[k] +
                     ((92 * a[k] - 39) * a[k] + (-76 * a[k] + 7 * b[k] + 7) * b[k] + 7) * b[k] - 75;
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / SAMPLES;
    printf("Time(ms) CPU 1 Thread: %5.2lf\n", cpuExecTime);

    int v = OUT[idx];

    delete[] a;
    delete[] b;
    delete[] OUT;

    return v;
}

int sgfilter_openmp(int idx) {
    auto a = new unsigned short[DATA_SIZE];
    auto b = new unsigned short[DATA_SIZE];
    auto OUT = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k % 1024;
        b[k] = k % 1024;
        OUT[k] = 0;
    }
    omp_set_num_threads(NUM_THREAD);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    for (int i = 0; i < SAMPLES; i++) {
        s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
        for (int k = 0; k < DATA_SIZE; ++k) {
            OUT[k] = ((-984 * a[k] - 46) * a[k] - 46) * a[k] +
                     ((92 * a[k] - 39) * a[k] + (-76 * a[k] + 7 * b[k] + 7) * b[k] + 7) * b[k] - 75;
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / SAMPLES;
    printf("Time(ms) CPU %d Thread: %5.2lf\n", NUM_THREAD, cpuExecTime);

    int v = OUT[idx];

    delete[] a;
    delete[] b;
    delete[] OUT;

    return v;
}

int sgfilter_cgra(int idx, int copies) {
    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
    auto cgraHw = new Cgra();
    Scheduler scheduler(cgraArch);
    std::vector<DataFlow *> dfs;
    unsigned short *a, *b, *c, *out;
    int r = 0, v = 0, tries = 0;

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
                cgraHw->setCgraProgramInputStreamByID(i, (j * 3), &a[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 3) + 1, &b[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramOutputStreamByID(i, (j * 3) + 2, &out[k * data_size], data_size_bytes);
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
    delete[] out;

    for (auto df:dfs) {
        delete df;
    }
    return v;

}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "sgfilter");
    int idx = 0;
    std::vector<Operator *> in1;
    std::vector<Operator *> in2;
    std::vector<Operator *> out;

    for (int i = 0; i < copies; ++i) {
        in1.push_back(new InputStream(idx++));
        in2.push_back(new InputStream(idx++));
        out.push_back(new OutputStream(idx++));
    }
    for (int j = 0; j < copies; ++j) {
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
        auto reg11 = new PassA(idx++);
        auto reg12 = new PassA(idx++);
        auto reg13 = new PassA(idx++);
        auto reg14 = new PassA(idx++);
        auto reg15 = new PassA(idx++);
        auto mult1 = new Multi(idx++, 92);
        auto mult2 = new Multi(idx++, -984);
        auto mult3 = new Multi(idx++, -76);
        auto mult4 = new Multi(idx++, 7);
        auto sub1 = new Subi(idx++, 46);
        auto sub2 = new Subi(idx++, 39);
        auto add1 = new Add(idx++);
        auto mult5 = new Mult(idx++);
        auto mult6 = new Mult(idx++);
        auto add2 = new Addi(idx++, 7);
        auto sub3 = new Subi(idx++, 46);
        auto mult7 = new Mult(idx++);
        auto mult8 = new Mult(idx++);
        auto add3 = new Add(idx++);
        auto add4 = new Addi(idx++, 7);
        auto mult9 = new Mult(idx++);
        auto add5 = new Add(idx++);
        auto sub4 = new Subi(idx++, 75);

        df->connect(in1[j], mult1, mult1->getPortA());
        df->connect(in1[j], mult2, mult2->getPortA());
        df->connect(in1[j], mult3, mult3->getPortA());
        df->connect(in1[j], reg11, reg11->getPortA());
        df->connect(reg11, reg12, reg12->getPortA());
        df->connect(in1[j], reg1, reg1->getPortA());
        df->connect(reg1, reg2, reg2->getPortA());
        df->connect(reg2, reg3, reg3->getPortA());
        df->connect(reg3, reg4, reg4->getPortA());
        df->connect(in2[j], mult4, mult4->getPortA());
        df->connect(in2[j], reg5, reg5->getPortA());
        df->connect(reg5, reg6, reg6->getPortA());
        df->connect(reg6, reg7, reg7->getPortA());
        df->connect(reg7, reg8, reg8->getPortA());
        df->connect(reg8, reg9, reg9->getPortA());
        df->connect(reg9, reg10, reg10->getPortA());
        df->connect(mult2, sub1, sub1->getPortA());
        df->connect(mult1, sub2, sub2->getPortA());
        df->connect(mult3, add1, add1->getPortA());
        df->connect(mult4, add1, add1->getPortB());
        df->connect(reg12, mult5, mult5->getPortA());
        df->connect(sub1, mult5, mult5->getPortB());
        df->connect(reg2, mult6, mult6->getPortA());
        df->connect(sub2, mult6, mult6->getPortB());
        df->connect(add1, add2, add2->getPortA());
        df->connect(mult5, sub3, sub3->getPortA());
        df->connect(reg7, mult7, mult7->getPortA());
        df->connect(add2, mult7, mult7->getPortB());
        df->connect(reg4, mult8, mult8->getPortA());
        df->connect(sub3, mult8, mult8->getPortB());
        df->connect(mult6, reg13, reg13->getPortA());
        df->connect(reg13, add3, add3->getPortA());
        df->connect(mult7, add3, add3->getPortB());
        df->connect(add3, add4, add4->getPortA());
        df->connect(add4, mult9, mult9->getPortA());
        df->connect(reg10, mult9, mult9->getPortB());
        df->connect(mult9, add5, add5->getPortA());
        df->connect(mult8, reg14, reg14->getPortB());
        df->connect(reg14, reg15, reg15->getPortA());
        df->connect(reg15, add5, add5->getPortB());
        df->connect(add5, sub4, sub4->getPortA());
        df->connect(sub4, out[j], out[j]->getPortA());
    }
    return df;
}

