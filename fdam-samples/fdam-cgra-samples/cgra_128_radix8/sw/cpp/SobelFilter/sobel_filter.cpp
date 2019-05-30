#include "sobel_filter.h"

int main(int argc, char *argv[]) {

    int idx = 0;
    int test = 0;

    if (argc > 1)
        test = atoi(argv[1]);

    if (argc > 2)
        idx = atoi(argv[2]);

    if (test & 1)
        sobel_filter(idx);
    if (test & 2)
        sobel_filter_openmp(idx);
    if (test & 4)
        sobel_filter_cgra(idx, 1);

    return 0;
}

int sobel_filter(int idx) {

    int v = 0;
    int gray_size = DATA_SIZE;
    int width = (int) sqrt(DATA_SIZE);
    byte sobel_h[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    byte sobel_v[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    byte **gray;
    byte **contour_img_cpu;
    gray = new byte *[NUM_THREAD];
    contour_img_cpu = new byte *[NUM_THREAD];
    auto **sobel_h_res = new byte *[NUM_THREAD];
    auto **sobel_v_res = new byte *[NUM_THREAD];

    for (int i = 0; i < NUM_THREAD; ++i) {
        gray[i] = new byte[gray_size];
        contour_img_cpu[i] = new byte[gray_size];
        sobel_h_res[i] = new byte[DATA_SIZE];
        sobel_v_res[i] = new byte[DATA_SIZE];

        for (int j = 0; j < DATA_SIZE; ++j) {
            gray[i][j] = j;
        }
    }

    omp_set_num_threads(NUM_THREAD);

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    for (int l = 0; l < SAMPLES; ++l) {
        s = high_resolution_clock::now();
        for (int i = 0; i < NUM_THREAD; ++i) {
            itConv(gray[i], gray_size, width, sobel_h, sobel_h_res[i]);
            itConv(gray[i], gray_size, width, sobel_v, sobel_v_res[i]);
            contour(sobel_h_res[i], sobel_v_res[i], gray_size, contour_img_cpu[i]);
        }
        diff += high_resolution_clock::now() - s;
    }

    double cpuExecTime = (diff.count() * 1000) / SAMPLES;

    printf("Time(ms) CPU 1 Thread: %5.2lf\n", NUM_THREAD, cpuExecTime);

    v = contour_img_cpu[0][idx];

    for (int k = 0; k < NUM_THREAD; ++k) {
        delete gray[k];
        delete contour_img_cpu[k];
        delete sobel_h_res[k];
        delete sobel_v_res[k];
    }

    delete gray;
    delete contour_img_cpu;
    delete[]sobel_h_res;
    delete[]sobel_v_res;

    return v;

}

int sobel_filter_openmp(int idx) {

    int v = 0;
    int gray_size = DATA_SIZE;
    int width = (int) sqrt(DATA_SIZE);
    byte sobel_h[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    byte sobel_v[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    byte **gray;
    byte **contour_img_cpu;
    gray = new byte *[NUM_THREAD];
    contour_img_cpu = new byte *[NUM_THREAD];
    auto **sobel_h_res = new byte *[NUM_THREAD];
    auto **sobel_v_res = new byte *[NUM_THREAD];

    for (int i = 0; i < NUM_THREAD; ++i) {
        gray[i] = new byte[gray_size];
        contour_img_cpu[i] = new byte[gray_size];
        sobel_h_res[i] = new byte[DATA_SIZE];
        sobel_v_res[i] = new byte[DATA_SIZE];

        for (int j = 0; j < DATA_SIZE; ++j) {
            gray[i][j] = j;
        }
    }

    omp_set_num_threads(NUM_THREAD);

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    for (int l = 0; l < SAMPLES; ++l) {
        s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
        for (int i = 0; i < NUM_THREAD; ++i) {
            itConv(gray[i], gray_size, width, sobel_h, sobel_h_res[i]);
            itConv(gray[i], gray_size, width, sobel_v, sobel_v_res[i]);
            contour(sobel_h_res[i], sobel_v_res[i], gray_size, contour_img_cpu[i]);
        }
        diff += high_resolution_clock::now() - s;
    }

    double cpuExecTime = (diff.count() * 1000) / SAMPLES;

    printf("Time(ms) CPU %d Thread: %5.2lf\n", NUM_THREAD, cpuExecTime);

    v = contour_img_cpu[0][idx];

    for (int k = 0; k < NUM_THREAD; ++k) {
        delete gray[k];
        delete contour_img_cpu[k];
        delete sobel_h_res[k];
        delete sobel_v_res[k];
    }

    delete gray;
    delete contour_img_cpu;
    delete[]sobel_h_res;
    delete[]sobel_v_res;

    return v;
}

int sobel_filter_cgra(int idx, int copies) {
    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
    auto cgraHw = new Cgra();
    Scheduler scheduler(cgraArch);
    std::vector<DataFlow *> dfs;
    int r = 0, v = 0, tries = 0;

    auto data_in = new short *[8];
    auto data_out = new short[DATA_SIZE];

    for (int i = 0; i < 8; i++) {
        data_in[i] = new short[DATA_SIZE];
        for (int k = 0; k < DATA_SIZE; ++k) {
            data_in[i][k] = k;
        }
    }
    for (int k = 0; k < DATA_SIZE; ++k) {
        data_out[k] = k;
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
                cgraHw->setCgraProgramInputStreamByID(i, (j * copies), &data_in[0][k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * copies) + 1, &data_in[1][k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * copies) + 2, &data_in[2][k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * copies) + 3, &data_in[3][k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * copies) + 4, &data_in[4][k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * copies) + 5, &data_in[5][k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * copies) + 6, &data_in[6][k * data_size], data_size_bytes);
                cgraHw->setCgraProgramInputStreamByID(i, (j * copies) + 7, &data_in[7][k * data_size], data_size_bytes);
                cgraHw->setCgraProgramOutputStreamByID(i, (j * copies) + 8, &data_out[k * data_size], data_size_bytes);
                k++;
            }
        }
        double cgraExecTime = 0;
        for (int i = 0; i < SAMPLES; i++) {
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

    delete[] data_in;
    delete[] data_out;

    for (auto df:dfs) {
        delete df;
    }
    return v;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "sobel_filter");
    int idx = 0;
    Operator *inputs[9];
    Operator *output[2];
    std::vector<Operator *> aux0;
    std::vector<Operator *> aux1;
    std::vector<Operator *> adds;

    int gx_gy[2][9] = {{1,  2, 1, 0,  0, 0, -1, -2, -1},
                       {-1, 0, 1, -2, 0, 2, -1, 0,  1}};

    for (int i = 0; i < 9; ++i) {
        if (i != 4)
            inputs[i] = new InputStream(idx++);
    }
    output[0] = new OutputStream(idx++);

    for (int i = 0; i < 9; ++i) {
        if (i == 4) {
            auto r = new PassBi(idx++, 0);
            inputs[i] = r;
        } else {
            auto r = new PassB(idx++);
            df->connect(inputs[i], r, r->getPortB());
            inputs[i] = r;
        }
    }

    for (auto &l : gx_gy) {
        aux0.clear();
        aux1.clear();
        for (int j = 0; j < 9; ++j) {
            auto mul = new Multi(idx++, l[9 - j - 1]);
            df->connect(inputs[j], mul, mul->getPortA());
            aux0.push_back(mul);
        }
        while (aux0.size() > 1) {
            int r = 0;
            if (aux0.size() % 2 != 0) {
                auto reg = new PassA(idx++);
                df->connect(aux0[aux0.size() - 1], reg, reg->getPortA());
                aux1.push_back(reg);
                r = 1;
            }
            for (int k = 0; k < aux0.size() - r; k += 2) {
                auto add = new Add(idx++);
                df->connect(aux0[k], add, add->getPortA());
                df->connect(aux0[k + 1], add, add->getPortB());
                aux1.push_back(add);
            }
            aux0.clear();
            for (auto a:aux1) {
                aux0.push_back(a);
            }
            aux1.clear();
        }
        adds.push_back(aux0[0]);
    }
    auto mult1 = new Mult(idx++);
    auto mult2 = new Mult(idx++);
    auto add = new Add(idx++);

    df->connect(adds[0], mult1, mult1->getPortA());
    df->connect(adds[0], mult1, mult1->getPortB());
    df->connect(adds[1], mult2, mult2->getPortA());
    df->connect(adds[1], mult2, mult2->getPortB());
    df->connect(mult1, add, add->getPortA());
    df->connect(mult2, add, add->getPortB());
    df->connect(add, output[0], output[0]->getPortA());
    return df;
}

void itConv(byte *buffer, int buffer_size, int width, byte *op, byte *res) {
    byte op_mem[SOBEL_OP_SIZE];
    memset(op_mem, 0, SOBEL_OP_SIZE);
    for (int i = 0; i < buffer_size; i++) {
        makeOpMemCPU(buffer, buffer_size, width, i, op_mem);
        res[i] = (convolution(op_mem, op, SOBEL_OP_SIZE));
    }
}

byte convolution(byte *X, byte *Y, int c_size) {
    byte sum = 0;
    for (int i = 0; i < c_size; i++) {
        sum += X[i] * Y[c_size - i - 1];
    }

    return sum;
}

void makeOpMemCPU(const byte *buffer, int buffer_size, int width, int cindex, byte *op_mem) {

    int bottom = cindex - width < 0;
    int top = cindex + width >= buffer_size;
    int left = cindex % width == 0;
    int right = (cindex + 1) % width == 0;
    byte zero = 0;

    op_mem[0] = !bottom && !left ? buffer[cindex - width - 1] : zero;
    op_mem[1] = !bottom ? buffer[cindex - width] : zero;
    op_mem[2] = !bottom && !right ? buffer[cindex - width + 1] : zero;

    op_mem[3] = !left ? buffer[cindex - 1] : zero;
    op_mem[4] = buffer[cindex];
    op_mem[5] = !right ? buffer[cindex + 1] : zero;

    op_mem[6] = !top && !left ? buffer[cindex + width - 1] : zero;
    op_mem[7] = !top ? buffer[cindex + width] : zero;
    op_mem[8] = !top && !right ? buffer[cindex + width + 1] : zero;
}

void contour(const byte *sobel_h, const byte *sobel_v, int gray_size, byte *contour_img) {
    for (int i = 0; i < gray_size; i++) {
        contour_img[i] = 255 - (byte) sqrt(pow2(sobel_h[i]) + pow2(sobel_v[i]));
    }
}
