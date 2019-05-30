#include "poly8.h"

int main(int argc, char *argv[]) {

    int idx = 0;
    int test = 0;
     
    if(argc > 1)
        test = atoi(argv[1]);
     
    if(argc > 2)
        idx = atoi(argv[2]);
    
    if(test & 1)
        poly8(idx);
    if(test & 2)
        poly8_openmp(idx);
    if(test & 4)    
        poly8_cgra(idx, 1);

    return 0;
}

int poly8(int idx) {

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

    for (int i = 0; i < SAMPLES; i++) {
        s = high_resolution_clock::now();
        for (int k = 0; k < DATA_SIZE; ++k) {
            out[k] = a[k] * (c[k] * (432 * b[k] - 13824) +
                             a[k] * a[k] * (c[k] * (6 * c[k] - 4312) + 55296 - (464 * c[k] - 13824))) + a[k];
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / SAMPLES;

    printf("Time(ms) CPU 1 Thread: %5.2lf\n", cpuExecTime);

    int v = out[idx];

    delete a;
    delete b;
    delete c;
    delete out;

    return v;
}

int poly8_openmp(int idx) {

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

    for (int i = 0; i < SAMPLES; i++) {
        s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
        for (int k = 0; k < DATA_SIZE; ++k) {
            out[k] = a[k] * (c[k] * (432 * b[k] - 13824) +
                             a[k] * a[k] * (c[k] * (6 * c[k] - 4312) + 55296 - (464 * c[k] - 13824))) + a[k];
        }
        diff += high_resolution_clock::now() - s;
    }
    double cpuExecTime = (diff.count() * 1000) / SAMPLES;

    printf("Time(ms) CPU %d Thread: %5.2lf\n", NUM_THREAD, cpuExecTime);

    int v = out[idx];

    delete a;
    delete b;
    delete c;
    delete out;

    return v;
}

int poly8_cgra(int idx, int copies) {
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
    auto df = new DataFlow(id, "poly8");
    int idx = 0;

    std::vector<Operator *> i0_n1;
    std::vector<Operator *> i1_n2;
    std::vector<Operator *> i2_n3;
    std::vector<Operator *> o0_n36;

    for (int i = 0; i < copies; ++i) {
        i0_n1.push_back(new InputStream(idx++));//0
        i1_n2.push_back(new InputStream(idx++));//1
        i2_n3.push_back(new InputStream(idx++));//2
        o0_n36.push_back(new OutputStream(idx++));//3
    }
    for (int i = 0; i < copies; ++i) {

        auto mul_n4 = new Mult(idx++);//4
        auto mul_n6 = new Mult(idx++);//5
        auto mul_n7 = new Mult(idx++);//6
        auto mul_n8 = new Mult(idx++);//7
        auto mul_n9 = new Mult(idx++);//8
        auto mul_n10 = new Mult(idx++);//9
        auto mul_n16 = new Mult(idx++);//10
        auto mul_n27 = new Mult(idx++);//11
        auto mul_n30 = new Mult(idx++);//12
        auto mul_n31 = new Mult(idx++);//13
        auto sub_n15 = new Sub(idx++);//14
        auto sub_n19 = new Sub(idx++);//15
        auto add_n17 = new Add(idx++);//16
        auto add_n18 = new Add(idx++);//17
        auto add_n20 = new Add(idx++);//18
        auto add_n24 = new Add(idx++);//19
        auto add_n32 = new Add(idx++);//20
        auto add_n35 = new Add(idx++);//21
        auto add_lmm_72_n5 = new Addi(idx++, 72);//22
        auto add_lmm_6912_n23 = new Addi(idx++, 6912);//23
        auto add_lmm_1728_n22 = new Addi(idx++, 1728);//24
        auto add_lmm_55296_n33 = new Addi(idx++, 55296);//25
        auto mul_lmm_4_n12 = new Multi(idx++, 4);//26
        auto mul_lmm_432_n21 = new Multi(idx++, 432);//27
        auto mul_lmm_360_n28 = new Multi(idx++, 360);//28
        auto mul_lmm_71_n14 = new Multi(idx++, 71);//29
        auto mul_lmm_6_n11 = new Multi(idx++, 6);//30
        auto mul_lmm_464_n13 = new Multi(idx++, 464);//31
        auto sub_lmm_13824_n26 = new Subi(idx++, 13824);//32
        auto sub_lmm_13824_n34 = new Subi(idx++, 13824);//33
        auto sub_lmm_13824_n29 = new Subi(idx++, 13824);//34
        auto sub_lmm_4312_n25 = new Subi(idx++, 4312);//35
        auto reg13 = new PassA(idx++);//36
        auto reg14 = new PassA(idx++);//37
        auto reg15 = new PassA(idx++);//38
        auto reg16 = new PassA(idx++);//39
        auto reg17 = new PassA(idx++);//40
        auto reg18 = new PassA(idx++);//41
        auto reg19 = new PassA(idx++);//42
        auto reg20 = new PassA(idx++);//43
        auto reg21 = new PassA(idx++);//44
        auto reg22 = new PassA(idx++);//45
        auto reg23 = new PassA(idx++);//46
        auto reg24 = new PassA(idx++);//47
        auto reg25 = new PassA(idx++);//48
        auto reg28 = new PassA(idx++);//49
        auto reg29 = new PassA(idx++);//50
        auto reg30 = new PassA(idx++);//51
        auto reg31 = new PassA(idx++);//52
        auto reg32 = new PassA(idx++);//53
        auto reg34 = new PassA(idx++);//54
        auto reg35 = new PassA(idx++);//55
        auto reg36 = new PassA(idx++);//56
        auto reg39 = new PassA(idx++);//57
        auto reg40 = new PassA(idx++);//58
        auto reg41 = new PassA(idx++);//59
        auto reg43 = new PassA(idx++);//60
        auto reg44 = new PassA(idx++);//61
        auto reg45 = new PassA(idx++);//62
        auto reg46 = new PassA(idx++);//63
        auto reg47 = new PassA(idx++);//64
        auto reg48 = new PassA(idx++);//65
        auto reg49 = new PassA(idx++);//66
        auto reg50 = new PassA(idx++);//67

        df->connect(reg17, mul_n6, mul_n6->getPortA());
        df->connect(reg19, mul_n10, mul_n10->getPortA());
        df->connect(i0_n1[i], reg13, reg13->getPortA());
        df->connect(reg13, reg14, reg14->getPortA());
        df->connect(reg14, reg15, reg15->getPortA());
        df->connect(reg15, reg16, reg16->getPortA());
        df->connect(reg16, reg17, reg17->getPortA());
        df->connect(reg17, reg18, reg18->getPortA());
        df->connect(reg18, reg19, reg19->getPortA());
        df->connect(reg19, reg20, reg20->getPortA());
        df->connect(reg20, reg21, reg21->getPortA());
        df->connect(reg21, mul_n4, mul_n4->getPortA());
        df->connect(mul_n6, add_n18, add_n18->getPortA());
        df->connect(mul_n10, add_n35, add_n35->getPortA());
        df->connect(add_n18, mul_n10, mul_n10->getPortB());
        df->connect(add_n35, mul_n4, mul_n4->getPortB());
        df->connect(mul_n4, add_n17, add_n17->getPortA());
        df->connect(add_n17, o0_n36[i], o0_n36[i]->getPortA());
        df->connect(i1_n2[i], reg22, reg22->getPortA());
        df->connect(reg22, reg23, reg23->getPortA());
        df->connect(reg23, reg24, reg24->getPortA());
        df->connect(reg24, reg25, reg25->getPortA());
        df->connect(reg25, mul_n9, mul_n9->getPortA());
        df->connect(mul_n9, add_n32, add_n32->getPortA());
        df->connect(add_n32, add_n18, add_n18->getPortB());
        df->connect(i1_n2[i], add_lmm_72_n5, add_lmm_72_n5->getPortA());
        df->connect(add_lmm_72_n5, mul_lmm_4_n12, mul_lmm_4_n12->getPortA());
        df->connect(mul_lmm_4_n12, mul_n27, mul_n27->getPortA());
        df->connect(mul_n27, add_n20, add_n20->getPortA());
        df->connect(add_n20, mul_n9, mul_n9->getPortB());
        df->connect(i1_n2[i], mul_lmm_432_n21, mul_lmm_432_n21->getPortA());
        df->connect(mul_lmm_432_n21, sub_lmm_13824_n26, sub_lmm_13824_n26->getPortA());
        df->connect(sub_lmm_13824_n26, mul_n8, mul_n8->getPortA());
        df->connect(mul_n8, reg28, reg28->getPortA());
        df->connect(reg28, reg29, reg29->getPortA());
        df->connect(reg29, reg30, reg30->getPortA());
        df->connect(reg30, reg31, reg31->getPortA());
        df->connect(reg31, reg32, reg32->getPortA());
        df->connect(reg32, add_n35, add_n35->getPortB());
        df->connect(reg24, mul_n7, mul_n7->getPortA());
        df->connect(mul_n7, sub_n15, sub_n15->getPortA());
        df->connect(sub_n15, mul_n6, mul_n6->getPortB());
        df->connect(reg22, mul_n16, mul_n16->getPortA());
        df->connect(mul_n16, add_n24, add_n24->getPortA());
        df->connect(add_n24, mul_n7, mul_n7->getPortB());
        df->connect(i1_n2[i], sub_n19, sub_n19->getPortA());
        df->connect(sub_n19, mul_n16, mul_n16->getPortB());
        df->connect(i2_n3[i], sub_n19, sub_n19->getPortB());
        df->connect(i2_n3[i], reg35, reg35->getPortA());
        df->connect(reg35, reg36, reg36->getPortA());
        df->connect(reg36, mul_n8, mul_n8->getPortB());
        df->connect(i2_n3[i], mul_lmm_360_n28, mul_lmm_360_n28->getPortA());
        df->connect(mul_lmm_360_n28, reg34, reg34->getPortA());
        df->connect(reg34, add_lmm_6912_n23, add_lmm_6912_n23->getPortA());
        df->connect(add_lmm_6912_n23, add_n20, add_n20->getPortB());
        df->connect(i2_n3[i], mul_lmm_71_n14, mul_lmm_71_n14->getPortA());
        df->connect(mul_lmm_71_n14, add_lmm_1728_n22, add_lmm_1728_n22->getPortA());
        df->connect(add_lmm_1728_n22, add_n24, add_n24->getPortB());
        df->connect(i2_n3[i], mul_lmm_6_n11, mul_lmm_6_n11->getPortA());
        df->connect(mul_lmm_6_n11, sub_lmm_4312_n25, sub_lmm_4312_n25->getPortA());
        df->connect(sub_lmm_4312_n25, mul_n31, mul_n31->getPortA());
        df->connect(mul_n31, add_lmm_55296_n33, add_lmm_55296_n33->getPortA());
        df->connect(add_lmm_55296_n33, reg39, reg39->getPortA());
        df->connect(reg39, add_n32, add_n32->getPortB());
        df->connect(reg36, mul_n31, mul_n31->getPortB());
        df->connect(i2_n3[i], mul_lmm_464_n13, mul_lmm_464_n13->getPortA());
        df->connect(mul_lmm_464_n13, sub_lmm_13824_n29, sub_lmm_13824_n29->getPortA());
        df->connect(sub_lmm_13824_n29, reg40, reg40->getPortA());
        df->connect(reg40, reg41, reg41->getPortA());
        df->connect(reg41, sub_n15, sub_n15->getPortB());
        df->connect(reg35, mul_n30, mul_n30->getPortA());
        df->connect(mul_n30, reg43, reg43->getPortA());
        df->connect(reg43, reg44, reg44->getPortA());
        df->connect(reg44, reg45, reg45->getPortA());
        df->connect(reg45, reg46, reg46->getPortA());
        df->connect(reg46, reg47, reg47->getPortA());
        df->connect(reg47, reg48, reg48->getPortA());
        df->connect(reg48, reg49, reg49->getPortA());
        df->connect(reg49, reg50, reg50->getPortA());
        df->connect(reg50, add_n17, add_n17->getPortB());
        df->connect(i2_n3[i], sub_lmm_13824_n34, sub_lmm_13824_n34->getPortA());
        df->connect(sub_lmm_13824_n34, mul_n30, mul_n30->getPortB());
    }

    return df;
}
