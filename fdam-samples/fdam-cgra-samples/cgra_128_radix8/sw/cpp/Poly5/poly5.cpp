#include "poly5.h"

int main(int argc, char *argv[]) {

    int idx = 0;
    int test = 0;
     
    if(argc > 1)
        test = atoi(argv[1]);
     
    if(argc > 2)
        idx = atoi(argv[2]);

    if(test & 1)
        poly5(idx);
    if(test & 2)
        poly5_openmp(idx);
    if(test & 4)
        poly5_cgra(idx,2);

    return 0;
}

int poly5(int idx) {

    unsigned short *a, *b, *c, *out;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k;
        b[k] = k;
        c[k] = k;
        out[k] = 0;
    }

    high_resolution_clock::time_point s;
    duration<double> diff = {};

    for (int i = 0; i < 100; i++) {
        s = high_resolution_clock::now();
        for (int k = 0; k < DATA_SIZE; ++k) {
            out[k] = ((a[k] * (a[k] * (a[k - 432]) + 62208) - 2985984) * b[k] *
                      (a[k] * (78 * a[k] - 9504) + b[k] * (288 * a[k] - 5184)) +
                      c[k] * b[k] * (a[k] * (a[k] - 207) + 3456 - b[k] * (2 * a[k] - c[k] + 144))) * c[k];
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

int poly5_openmp(int idx) {

    unsigned short *a, *b, *c, *out;

    a = new unsigned short[DATA_SIZE];
    b = new unsigned short[DATA_SIZE];
    c = new unsigned short[DATA_SIZE];
    out = new unsigned short[DATA_SIZE];

    for (int k = 0; k < DATA_SIZE; ++k) {
        a[k] = k;
        b[k] = k;
        c[k] = k;
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
            out[k] = ((a[k] * (a[k] * (a[k - 432]) + 62208) - 2985984) * b[k] *
                      (a[k] * (78 * a[k] - 9504) + b[k] * (288 * a[k] - 5184)) +
                      c[k] * b[k] * (a[k] * (a[k] - 207) + 3456 - b[k] * (2 * a[k] - c[k] + 144))) * c[k];
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

int poly5_cgra(int idx,int copies){
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
    auto df = new DataFlow(id, "poly5");
    int idx = 0;

    std::vector<Operator *> in0;
    std::vector<Operator *> in1;
    std::vector<Operator *> in2;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in0.push_back(new InputStream(idx++));
        in1.push_back(new InputStream(idx++));
        in2.push_back(new InputStream(idx++));
        out.push_back(new OutputStream(idx++));
    }
    for (int i = 0; i < copies; ++i) {

        auto in0_3 = new PassA(idx++);
        auto in0_4 = new PassA(idx++);
        auto in0_5 = new PassA(idx++);

        auto in1_1 = new PassA(idx++);
        auto in1_2 = new PassA(idx++);
        auto in1_3 = new PassA(idx++);
        auto in1_4 = new PassA(idx++);
        auto in1_5 = new PassA(idx++);
        auto in1_6 = new PassA(idx++);
        auto in1_7 = new PassA(idx++);
        auto in1_8 = new PassA(idx++);

        auto in2_1 = new PassA(idx++);
        auto in2_2 = new PassA(idx++);
        auto in2_3 = new PassA(idx++);

        //level 1
        auto sub_imm_432_n28 = new Subi(idx++, 432);
        auto sub_imm_207_n26 = new Subi(idx++, 207);
        auto mul_imm_78_n9 = new Multi(idx++, 78);
        auto mul_imm_288_n8 = new Multi(idx++, 288);
        auto mul_imm_2_n21 = new Multi(idx++, 2);
        auto add_imm_144_n12 = new Addi(idx++, 144);
        //level 2
        auto mul_n17 = new Mult(idx++);
        auto sub_imm_9504_n16 = new Subi(idx++, 9504);
        auto sub_imm_5184_n29 = new Subi(idx++, 5184);
        auto mul_n7 = new Mult(idx++);
        auto sub_n18 = new Sub(idx++);
        //level 3
        auto add_imm_62208_n15 = new Addi(idx++, 62208);
        auto mul_n11 = new Mult(idx++);
        auto mul_n19 = new Mult(idx++);
        auto add_imm_3456_n14 = new Addi(idx++, 3456);
        auto mul_n6 = new Mult(idx++);
        //level 4
        auto mul_n20 = new Mult(idx++);
        auto add_n13 = new Add(idx++);
        auto sub_n10 = new Sub(idx++);
        //level 5
        auto sub_imm_2985984_n23 = new Subi(idx++, 2985984);
        auto sub_imm_248832_n22 = new Subi(idx++, 248832);
        auto mul_n24 = new Mult(idx++);
        //level 6
        auto mul_n5 = new Mult(idx++);
        auto mul_n4 = new Mult(idx++);
        auto sub_imm_2985984_n23_reg1 = new PassA(idx++);
        //level 7
        auto mul_n4_reg1 = new PassA(idx++);
        auto add_n30 = new Add(idx++);
        auto add_n27 = new Add(idx++);
        //level 8
        auto mul_n25 = new Mult(idx++);

        //level 1
        df->connect(in2[i], sub_imm_432_n28, sub_imm_432_n28->getPortA());
        df->connect(in2[i], mul_imm_78_n9, mul_imm_78_n9->getPortA());
        df->connect(in2[i], mul_imm_288_n8, mul_imm_288_n8->getPortA());
        df->connect(in2[i], sub_imm_207_n26, sub_imm_207_n26->getPortA());
        df->connect(in2[i], mul_imm_2_n21, mul_imm_78_n9->getPortA());
        df->connect(in2[i], in2_1, in2_1->getPortA());
        df->connect(in1[i], add_imm_144_n12, add_imm_144_n12->getPortA());
        df->connect(in1[i], in1_1, in1_1->getPortA());
        //level 2
        df->connect(in2_1, mul_n17, mul_n17->getPortA());
        df->connect(sub_imm_432_n28, mul_n17, mul_n17->getPortB());
        df->connect(mul_imm_78_n9, sub_imm_9504_n16, sub_imm_9504_n16->getPortA());
        df->connect(mul_imm_288_n8, sub_imm_5184_n29, sub_imm_5184_n29->getPortA());
        df->connect(sub_imm_207_n26, mul_n7, mul_n7->getPortA());
        df->connect(in2_1, mul_n7, mul_n7->getPortB());
        df->connect(mul_imm_2_n21, sub_n18, sub_n18->getPortA());
        df->connect(add_imm_144_n12, sub_n18, sub_n18->getPortB());
        df->connect(in2_1, in2_2, in2_2->getPortA());
        df->connect(in1_1, in1_2, in1_2->getPortA());
        // level 3
        df->connect(in0[i], in0_3, in0_3->getPortA());
        df->connect(mul_n17, add_imm_62208_n15, add_imm_62208_n15->getPortA());
        df->connect(sub_imm_9504_n16, mul_n11, mul_n11->getPortA());
        df->connect(in2_2, mul_n11, mul_n11->getPortB());
        df->connect(sub_imm_5184_n29, mul_n19, mul_n19->getPortA());
        df->connect(in0[i], mul_n19, mul_n19->getPortB());
        df->connect(mul_n7, add_imm_3456_n14, add_imm_3456_n14->getPortA());
        df->connect(sub_n18, mul_n6, mul_n6->getPortA());
        df->connect(in0[i], mul_n6, mul_n6->getPortB());
        df->connect(in2_2, in2_3, in2_3->getPortA());
        df->connect(in1_2, in1_3, in1_3->getPortA());
        // level 4
        df->connect(in0_3, in0_4, in0_4->getPortA());
        df->connect(in2_3, mul_n20, mul_n20->getPortA());
        df->connect(add_imm_62208_n15, mul_n20, mul_n20->getPortB());
        df->connect(mul_n11, add_n13, add_n13->getPortA());
        df->connect(mul_n19, add_n13, add_n13->getPortB());
        df->connect(add_imm_3456_n14, sub_n10, sub_n10->getPortA());
        df->connect(mul_n6, sub_n10, sub_n10->getPortB());
        df->connect(in1_3, in1_4, in1_4->getPortA());
        // level 5
        df->connect(mul_n20, sub_imm_2985984_n23, sub_imm_2985984_n23->getPortA());
        df->connect(add_n13, sub_imm_248832_n22, sub_imm_248832_n22->getPortA());
        df->connect(sub_n10, mul_n24, mul_n24->getPortA());
        df->connect(in0_4, mul_n24, mul_n24->getPortB());
        df->connect(in1_4, in1_5, in1_5->getPortA());
        df->connect(in0_4, in0_5, in0_5->getPortA());
        // level 6
        df->connect(sub_imm_2985984_n23, sub_imm_2985984_n23_reg1, sub_imm_2985984_n23_reg1->getPortA());
        df->connect(in1_5, in1_6, in1_6->getPortA());

        df->connect(mul_n24, mul_n4, mul_n4->getPortA());
        df->connect(in0_5, mul_n4, mul_n4->getPortB());

        df->connect(sub_imm_248832_n22, mul_n5, mul_n5->getPortA());
        df->connect(in0_5, mul_n5, mul_n5->getPortB());

        //level 7
        df->connect(mul_n4, mul_n4_reg1, mul_n4_reg1->getPortA());
        df->connect(sub_imm_2985984_n23_reg1, add_n30, add_n30->getPortA());
        df->connect(mul_n5, add_n30, add_n30->getPortB());
        df->connect(in1_6, in1_7, in1_7->getPortA());
        // level 8
        df->connect(add_n30, add_n27, add_n27->getPortA());
        df->connect(mul_n4_reg1, add_n27, add_n27->getPortB());
        df->connect(in1_7, in1_8, in1_8->getPortA());
        //level 9
        df->connect(add_n27, mul_n25, mul_n25->getPortA());
        df->connect(in1_8, mul_n25, mul_n25->getPortB());
        //level 10
        df->connect(mul_n25, out[i], out[i]->getPortA());
    }

    return df;
}
