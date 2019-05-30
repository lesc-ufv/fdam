#include "qspline.h"

int main(int argc, char *argv[]) {

    int idx = 0;
    int test = 0;
     
    if(argc > 1)
        test = atoi(argv[1]);
     
    if(argc > 2)
        idx = atoi(argv[2]);

    if(test & 1)
        qspline(idx);
    if(test & 2)    
        qspline_openmp(idx);
    if(test & 4)
        qspline_cgra(idx, 1);

    return 0;
}

int qspline(int idx) {

    unsigned short *a, *b, *c, *d, *e, *f, *g, *out;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    d = new unsigned short[DATA_SIZE];
    e = new unsigned short[DATA_SIZE];
    f = new unsigned short[DATA_SIZE];
    g = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k % 1024;
        b[k] = k % 1024;
        c[k] = k % 1024;
        d[k] = k % 1024;
        e[k] = k % 1024;
        f[k] = k % 1024;
        g[k] = k % 1024;
        out[k] = 0;
    }

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    for (int i = 0; i < SAMPLES; i++) {
        s = high_resolution_clock::now();
        for (int k = 0; k < DATA_SIZE; ++k) {
            out[k] = b[k] * a[k] * b[k] * b[k] * b[k] + b[k] * b[k] + b[k] * f[k] * 6 * c[k] * b[k] * f[k] +
                     e[k] * f[k] * f[k] * f[k] + b[k] * f[k] * f[k] * 4 * g[k] * f[k];
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / SAMPLES;

    printf("Time(ms) CPU 1 Thread: %5.2lf\n", cpuExecTime);

    int v = out[idx];

    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    delete g;
    delete out;

    return v;

}

int qspline_openmp(int idx) {

    unsigned short *a, *b, *c, *d, *e, *f, *g, *out;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    d = new unsigned short[DATA_SIZE];
    e = new unsigned short[DATA_SIZE];
    f = new unsigned short[DATA_SIZE];
    g = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k % 1024;
        b[k] = k % 1024;
        c[k] = k % 1024;
        d[k] = k % 1024;
        e[k] = k % 1024;
        f[k] = k % 1024;
        g[k] = k % 1024;
        out[k] = 0;
    }

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    omp_set_num_threads(NUM_THREAD);

    for (int i = 0; i < SAMPLES; i++) {
        s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
        for (int k = 0; k < DATA_SIZE; ++k) {
            out[k] = b[k] * a[k] * b[k] * b[k] * b[k] + b[k] * b[k] + b[k] * f[k] * 6 * c[k] * b[k] * f[k] +
                     e[k] * f[k] * f[k] * f[k] + b[k] * f[k] * f[k] * 4 * g[k] * f[k];
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / SAMPLES;

    printf("Time(ms) CPU %d Thread: %5.2lf\n", NUM_THREAD, cpuExecTime);

    int v = out[idx];

    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    delete g;
    delete out;

    return v;
}

int qspline_cgra(int idx, int copies) {
    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
    auto cgraHw = new Cgra();
    Scheduler scheduler(cgraArch);
    std::vector<DataFlow *> dfs;
    unsigned short *a, *b, *c, *d, *e, *f, *g, *out;
    int r = 0, v = 0, tries = 0;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    d = new unsigned short[DATA_SIZE];
    e = new unsigned short[DATA_SIZE];
    f = new unsigned short[DATA_SIZE];
    g = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k % 1024;
        b[k] = k % 1024;
        c[k] = k % 1024;
        d[k] = k % 1024;
        e[k] = k % 1024;
        f[k] = k % 1024;
        g[k] = k % 1024;
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
                cgraHw->setCgraProgramInputStreamByID(i, (j * 8), &a[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 1, &b[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 2, &c[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 3, &d[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 4, &e[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 5, &f[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * 8) + 6, &g[k * data_size], data_size_bytes);
                cgraHw->setCgraProgramOutputStreamByID(i, (j * 8) + 7, &out[k * data_size], data_size_bytes);
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
    delete[] d;
    delete[] e;
    delete[] f;
    delete[] g;
    delete[] out;

    for (auto df:dfs) {
        delete df;
    }
    return v;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "qspline");
    int idx = 0;
    // level 1
    std::vector<Operator *> i0_n1;
    std::vector<Operator *> i1_n2;
    std::vector<Operator *> i2_n3;
    std::vector<Operator *> i3_n4;
    std::vector<Operator *> i4_n5;
    std::vector<Operator *> i5_n6;
    std::vector<Operator *> i6_n7;
    std::vector<Operator *> o0_n34;
    for (int i = 0; i < copies; ++i) {
        i0_n1.push_back(new InputStream(idx++));//0
        i1_n2.push_back(new InputStream(idx++));//1
        i2_n3.push_back(new InputStream(idx++));//2
        i3_n4.push_back(new InputStream(idx++));//3
        i4_n5.push_back(new InputStream(idx++));//4
        i5_n6.push_back(new InputStream(idx++));//5
        i6_n7.push_back(new InputStream(idx++));//6
        o0_n34.push_back(new OutputStream(idx++));//7
    }
    for (int j = 0; j < copies; ++j) {
        auto reg1 = new PassA(idx++);//8
        auto reg2 = new PassA(idx++);//9
        auto reg3 = new PassA(idx++);//10
        auto reg4 = new PassA(idx++);//11
        auto reg5 = new PassA(idx++);//12
        auto reg6 = new PassA(idx++);//13
        auto reg7 = new PassA(idx++);//14
        auto reg8 = new PassA(idx++);//15
        auto reg9 = new PassA(idx++);//16
        auto reg10 = new PassA(idx++);//17
        auto reg11 = new PassA(idx++);//18
        auto reg12 = new PassA(idx++);//19
        auto reg13 = new PassA(idx++);//20
        auto reg14 = new PassA(idx++);//21
        auto reg15 = new PassA(idx++);//22
        auto reg16 = new PassA(idx++);//23
        auto reg17 = new PassA(idx++);//24
        auto reg18 = new PassA(idx++);//25
        auto reg19 = new PassA(idx++);//26
        auto reg20 = new PassA(idx++);//27
        auto reg21 = new PassA(idx++);//28
        auto reg22 = new PassA(idx++);//29
        auto reg23 = new PassA(idx++);//30
        auto reg24 = new PassA(idx++);//31
        auto reg25 = new PassA(idx++);//32
        auto reg27 = new PassA(idx++);//33
        auto reg28 = new PassA(idx++);//34
        auto reg29 = new PassA(idx++);//35
        auto reg30 = new PassA(idx++);//36
        auto reg31 = new PassA(idx++);//37
        auto reg32 = new PassA(idx++);//38
        auto reg33 = new PassA(idx++);//39
        auto reg34 = new PassA(idx++);//40
        auto reg35 = new PassA(idx++);//41
        auto reg36 = new PassA(idx++);//42
        auto mul_n8 = new Mult(idx++);//43
        auto mul_n9 = new Mult(idx++);//44
        auto mul_n11 = new Mult(idx++);//45
        auto mul_n13 = new Mult(idx++);//46
        auto mul_n29 = new Mult(idx++);//47
        auto mul_n25 = new Mult(idx++);//48
        auto mul_n23 = new Mult(idx++);//49
        auto mul_n24 = new Mult(idx++);//50
        auto mul_n18 = new Mult(idx++);//51
        auto mul_n10 = new Mult(idx++);//52
        auto mul_n28 = new Mult(idx++);//53
        auto mul_n20 = new Mult(idx++);//54
        auto mul_n21 = new Mult(idx++);//55
        auto mul_n14 = new Mult(idx++);//56
        auto mul_n15 = new Mult(idx++);//57
        auto mul_n12 = new Mult(idx++);//58
        auto mul_n26 = new Mult(idx++);//59
        auto mul_n19 = new Mult(idx++);//60
        auto mul_n17 = new Mult(idx++);//61
        auto add_n30 = new Add(idx++);//62
        auto add_n31 = new Add(idx++);//63
        auto add_n32 = new Add(idx++);//64
        auto add_n33 = new Add(idx++);//65
        auto mul_lmm_6_n27 = new Multi(idx++, 6);//66
        auto mul_lmm_4_n22 = new Multi(idx++, 4);//67
        auto mul_lmm_4_n16 = new Multi(idx++, 4);//68


        df->connect(i4_n5[j], mul_n13, mul_n13->getPortA());
        df->connect(i1_n2[j], mul_n13, mul_n13->getPortB());

        df->connect(i1_n2[j], reg1, reg1->getPortA());
        df->connect(reg1, reg2, reg2->getPortA());
        df->connect(reg2, reg3, reg3->getPortA());
        df->connect(reg3, mul_n8, mul_n8->getPortA());

        df->connect(i1_n2[j], reg4, reg4->getPortA());
        df->connect(reg4, mul_n11, mul_n11->getPortA());

        df->connect(i1_n2[j], reg5, reg5->getPortA());
        df->connect(reg5, reg6, reg6->getPortA());
        df->connect(reg6, mul_n29, mul_n29->getPortA());

        df->connect(i1_n2[j], reg7, reg7->getPortA());
        df->connect(reg7, reg8, reg8->getPortA());
        df->connect(reg8, reg9, reg9->getPortA());
        df->connect(reg9, mul_n25, mul_n25->getPortA());

        df->connect(i1_n2[j], reg10, reg10->getPortA());
        df->connect(reg10, reg11, reg11->getPortA());
        df->connect(reg11, reg12, reg12->getPortA());
        df->connect(reg12, mul_n20, mul_n20->getPortA());

        df->connect(i1_n2[j], reg13, reg13->getPortA());
        df->connect(reg13, reg14, reg14->getPortA());
        df->connect(reg14, mul_n24, mul_n24->getPortA());

        df->connect(i1_n2[j], mul_n18, mul_n18->getPortA());

        df->connect(i1_n2[j], reg15, reg15->getPortA());
        df->connect(reg15, mul_n10, mul_n11->getPortB());

        df->connect(mul_n13, mul_n11, mul_n11->getPortB());

        df->connect(mul_n8, reg16, reg16->getPortA());
        df->connect(reg16, reg35, reg35->getPortA());
        df->connect(reg35, reg36, reg36->getPortA());
        df->connect(reg36, add_n30, add_n30->getPortA());

        df->connect(mul_n29, mul_n8, mul_n8->getPortB());

        df->connect(mul_n25, reg17, reg17->getPortA());
        df->connect(reg17, reg18, reg18->getPortA());
        df->connect(reg18, add_n33, add_n33->getPortA());

        df->connect(mul_n24, mul_n25, mul_n25->getPortB());
        df->connect(mul_n10, mul_n28, mul_n28->getPortA());
        df->connect(mul_n11, mul_n29, mul_n29->getPortB());
        df->connect(add_n30, o0_n34[j], o0_n34[j]->getPortA());
        df->connect(add_n33, add_n30, add_n30->getPortB());
        df->connect(mul_n28, mul_n9, mul_n9->getPortA());

        df->connect(mul_n9, reg20, reg20->getPortA());
        df->connect(reg20, add_n31, add_n31->getPortA());

        df->connect(add_n31, add_n32, add_n32->getPortA());
        df->connect(add_n32, add_n33, add_n33->getPortB());

        df->connect(mul_n18, reg19, reg19->getPortA());
        df->connect(reg19, mul_n9, mul_n9->getPortB());

        df->connect(mul_n20, mul_n21, mul_n21->getPortA());
        df->connect(mul_n21, add_n32, add_n32->getPortB());
        df->connect(i3_n4[j], mul_lmm_6_n27, mul_lmm_6_n27->getPortA());
        df->connect(mul_lmm_6_n27, mul_n28, mul_n28->getPortB());

        df->connect(i0_n1[j], mul_lmm_4_n22, mul_lmm_4_n22->getPortA());
        df->connect(mul_lmm_4_n22, mul_n23, mul_n23->getPortA());
        df->connect(mul_n23, mul_n24, mul_n24->getPortB());

        df->connect(i6_n7[j], mul_n14, mul_n14->getPortA());
        df->connect(mul_n14, mul_n15, mul_n15->getPortA());
        df->connect(mul_n15, mul_n12, mul_n12->getPortA());
        df->connect(mul_n12, mul_n26, mul_n26->getPortA());

        df->connect(mul_n26, add_n31, add_n31->getPortB());

        df->connect(i5_n6[j], mul_n18, mul_n18->getPortB());
        df->connect(i5_n6[j], mul_n10, mul_n10->getPortA());
        df->connect(i5_n6[j], mul_n14, mul_n14->getPortB());

        df->connect(i5_n6[j], reg21, reg21->getPortA());
        df->connect(reg21, mul_n15, mul_n15->getPortB());

        df->connect(i5_n6[j], reg22, reg22->getPortA());
        df->connect(reg22, reg23, reg23->getPortA());
        df->connect(reg23, mul_n12, mul_n12->getPortB());

        df->connect(i5_n6[j], reg24, reg24->getPortA());
        df->connect(reg24, reg25, reg25->getPortA());
        df->connect(reg25, reg27, reg27->getPortA());
        df->connect(reg27, mul_n26, mul_n26->getPortB());

        df->connect(i5_n6[j], reg28, reg28->getPortA());
        df->connect(reg28, reg29, reg29->getPortA());
        df->connect(reg29, reg30, reg30->getPortA());
        df->connect(reg30, reg31, reg31->getPortA());
        df->connect(reg31, mul_n21, mul_n21->getPortB());

        df->connect(i5_n6[j], reg32, reg32->getPortA());
        df->connect(reg32, reg33, reg33->getPortA());
        df->connect(reg33, mul_n19, mul_n19->getPortA());

        df->connect(i5_n6[j], reg34, reg34->getPortA());
        df->connect(reg34, mul_n17, mul_n17->getPortA());

        df->connect(mul_n19, mul_n20, mul_n20->getPortB());
        df->connect(mul_n17, mul_n19, mul_n19->getPortB());

        df->connect(i2_n3[j], mul_lmm_4_n16, mul_lmm_4_n16->getPortA());
        df->connect(mul_lmm_4_n16, mul_n17, mul_n17->getPortB());

        df->connect(mul_n10, mul_n23, mul_n23->getPortB());
    }
    return df;
}
