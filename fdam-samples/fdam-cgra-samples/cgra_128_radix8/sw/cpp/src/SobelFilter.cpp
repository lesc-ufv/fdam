//
// Created by lucas on 31/01/19.
//

#include <SobelFilter.h>

SobelFilter::SobelFilter(Cgra *cgraHw, CgraArch *cgraArch) {
    SobelFilter::cgraHw = cgraHw;
    SobelFilter::cgraArch = cgraArch;
    SobelFilter::schedulingTime = 0;
    SobelFilter::cpuExecTime = 0;
    SobelFilter::cgraExecTime = 0;
    SobelFilter::cgraConfTime = 0;
}

SobelFilter::~SobelFilter() = default;

void SobelFilter::benchmarking(int numThread, int img_width, int img_height) {
    char file_in[8][100] = {
            {"../sobel_files/img.rgb"},
            {"../sobel_files/img.rgb"},
            {"../sobel_files/img.rgb"},
            {"../sobel_files/img.rgb"},
            {"../sobel_files/img.rgb"},
            {"../sobel_files/img.rgb"},
            {"../sobel_files/img.rgb"},
            {"../sobel_files/img.rgb"}
    };
    char file_out_cpu[8][100] = {
            {"../sobel_files/img_contour_cpu1.gray"},
            {"../sobel_files/img_contour_cpu2.gray"},
            {"../sobel_files/img_contour_cpu3.gray"},
            {"../sobel_files/img_contour_cpu4.gray"},
            {"../sobel_files/img_contour_cpu5.gray"},
            {"../sobel_files/img_contour_cpu6.gray"},
            {"../sobel_files/img_contour_cpu7.gray"},
            {"../sobel_files/img_contour_cpu8.gray"}
    };
    char file_out_cgra[8][100] = {
            {"../sobel_files/img_contour_cgra1.gray"},
            {"../sobel_files/img_contour_cgra2.gray"},
            {"../sobel_files/img_contour_cgra3.gray"},
            {"../sobel_files/img_contour_cgra4.gray"},
            {"../sobel_files/img_contour_cgra5.gray"},
            {"../sobel_files/img_contour_cgra6.gray"},
            {"../sobel_files/img_contour_cgra7.gray"},
            {"../sobel_files/img_contour_cgra8.gray"}
    };

    byte **rgb;
    byte **gray;
    byte **contour_img_cpu;
    byte **contour_img_cgra;

    int width = img_width;
    int height = img_height;
    int gray_size = width * height;
    int rgb_size = gray_size * 3;

    rgb = new byte *[numThread];
    gray = new byte *[numThread];
    contour_img_cpu = new byte *[numThread];
    contour_img_cgra = new byte *[numThread];

    for (int i = 0; i < numThread; ++i) {
        rgb[i] = new byte[rgb_size];
        gray[i] = new byte[gray_size];
        contour_img_cpu[i] = new byte[gray_size];
        contour_img_cgra[i] = new byte[gray_size];
    }
    for (int j = 0; j < numThread; ++j) {
        readFile(file_in[j], rgb[j], rgb_size);
        rgbToGray(rgb[j], gray[j], gray_size);
    }

    if (SobelFilter::compile(numThread)) {
        SobelFilter::runCGRA(gray, contour_img_cgra, width, gray_size, numThread);
        SobelFilter::runCPU(gray, contour_img_cpu, width, gray_size, numThread);
    } else {
        printf("Compilation failed!\n");
    }
    for (int k = 0; k < numThread; ++k) {
        writeFile(file_out_cgra[k], contour_img_cgra[k], gray_size);
        writeFile(file_out_cpu[k], contour_img_cpu[k], gray_size);
    }

    for (int i = 0; i < numThread; ++i) {
        delete rgb[i];
        delete gray[i];
        delete contour_img_cpu[i];
        delete contour_img_cgra[i];
    }

    delete rgb;
    delete gray;
    delete contour_img_cpu;
    delete contour_img_cgra;
}

void SobelFilter::runCGRA(byte **gray, byte **contour_img, int width, int gray_size, int num_img) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};

    short ***inputs;
    short **output;

    inputs = new short **[num_img];
    output = new short *[num_img];
    for (int l = 0; l < num_img; ++l) {
        inputs[l] = new short *[8];
        output[l] = new short[gray_size];
        for (int k = 0; k < 8; ++k) {
            inputs[l][k] = new short[gray_size];
        }
        SobelFilter::makeOpMemCGRA(gray[l], gray_size, width, inputs[l]);
    }
    SobelFilter::cgraHw->loadCgraProgram("../sobel_files/sobel_filter.cgra");
    for (int m = 0; m < num_img; ++m) {
        for (int i = 0; i < 8; ++i) {
            SobelFilter::cgraHw->setCgraProgramInputStreamByID(m, i, inputs[m][i], sizeof(short) * gray_size);
        }
        SobelFilter::cgraHw->setCgraProgramOutputStreamByID(m, 8, output[m], sizeof(short) * gray_size);
    }
    s = high_resolution_clock::now();
    SobelFilter::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    SobelFilter::cgraExecTime = diff.count() * 1000;
    for (int n = 0; n < num_img; ++n) {
        for (int j = 0; j < gray_size; ++j) {
            contour_img[n][j] = (byte) (255 - sqrt(output[n][j]));
        }
    }
    for (int l = 0; l < num_img; ++l) {
        for (int k = 0; k < 8; ++k) {
            delete inputs[l][k];
        }
        delete inputs[l];
        delete output[l];
    }
    delete inputs;
    delete output;
}

void SobelFilter::runCPU(byte **gray, byte **contour_img, int width, int gray_size, int num_img) {

    byte sobel_h[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1},
            sobel_v[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

    auto **sobel_h_res = new byte *[num_img];
    auto **sobel_v_res = new byte *[num_img];

    for (int j = 0; j < num_img; ++j) {
        sobel_h_res[j] = new byte[gray_size];
        sobel_v_res[j] = new byte[gray_size];
    }

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int i = 0; i < NUM_THREADS; ++i) {
        SobelFilter::itConv(gray[i], gray_size, width, sobel_h, sobel_h_res[i]);
        SobelFilter::itConv(gray[i], gray_size, width, sobel_v, sobel_v_res[i]);
        SobelFilter::contour(sobel_h_res[i], sobel_v_res[i], gray_size, contour_img[i]);
    }
    diff = high_resolution_clock::now() - s;
    SobelFilter::cpuExecTime = diff.count() * 1000;

    for (int k = 0; k < num_img; ++k) {
        delete sobel_h_res[k];
        delete sobel_v_res[k];
    }

    delete[] sobel_h_res;
    delete[] sobel_v_res;
}

void SobelFilter::rgbToGray(byte *rgb, byte *gray, int gray_size) {
    byte *p_rgb = rgb;
    byte *p_gray = gray;
    for (int i = 0; i < gray_size; i++) {
        *p_gray = (byte) (0.30 * p_rgb[0] + 0.59 * p_rgb[1] + 0.11 * p_rgb[2]);
        p_rgb += 3;
        p_gray++;
    }
}

void SobelFilter::readFile(char *file_name, byte *buffer, int buffer_size) {
    FILE *file = fopen(file_name, "r");
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = fgetc(file);
    }
    fclose(file);
}

void SobelFilter::writeFile(char *file_name, byte *buffer, int buffer_size) {

    FILE *file = fopen(file_name, "w");
    for (int i = 0; i < buffer_size; i++) {
        fputc(buffer[i], file);
    }
    fclose(file);
}

void SobelFilter::itConv(byte *buffer, int buffer_size, int width, byte *op, byte *res) {
    byte op_mem[SOBEL_OP_SIZE];
    memset(op_mem, 0, SOBEL_OP_SIZE);
    for (int i = 0; i < buffer_size; i++) {
        SobelFilter::makeOpMemCPU(buffer, buffer_size, width, i, op_mem);
        res[i] = (SobelFilter::convolution(op_mem, op, SOBEL_OP_SIZE));
    }
}

byte SobelFilter::convolution(byte *X, byte *Y, int c_size) {
    byte sum = 0;
    for (int i = 0; i < c_size; i++) {
        sum += X[i] * Y[c_size - i - 1];
    }

    return sum;
}

void SobelFilter::makeOpMemCGRA(byte *buffer, int buffer_size, int width, short **op_mem) {
    for (int cindex = 0; cindex < buffer_size; cindex++) {
        int bottom = cindex - width < 0;
        int top = cindex + width >= buffer_size;
        int left = cindex % width == 0;
        int right = (cindex + 1) % width == 0;
        short zero = 0;

        op_mem[0][cindex] = !bottom && !left ? (short) buffer[cindex - width - 1] : zero;
        op_mem[1][cindex] = !bottom ? (short) buffer[cindex - width] : zero;
        op_mem[2][cindex] = !bottom && !right ? (short) buffer[cindex - width + 1] : zero;

        op_mem[3][cindex] = !left ? (short) buffer[cindex - 1] : zero;
        //op_mem[4][cindex] = buffer[cindex];
        op_mem[4][cindex] = !right ? (short) buffer[cindex + 1] : zero;

        op_mem[5][cindex] = !top && !left ? (short) buffer[cindex + width - 1] : zero;
        op_mem[6][cindex] = !top ? (short) buffer[cindex + width] : zero;
        op_mem[7][cindex] = !top && !right ? (short) buffer[cindex + width + 1] : zero;
    }
}

void SobelFilter::makeOpMemCPU(const byte *buffer, int buffer_size, int width, int cindex, byte *op_mem) {

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

void SobelFilter::contour(const byte *sobel_h, const byte *sobel_v, int gray_size, byte *contour_img) {
    for (int i = 0; i < gray_size; i++) {
        contour_img[i] = 255 - (byte) sqrt(pow2(sobel_h[i]) + pow2(sobel_v[i]));
    }
}

bool SobelFilter::compile(int numThreads) {

    Scheduler scheduler(SobelFilter::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;

    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(SobelFilter::createDataFlow(i));
        scheduler.addDataFlow(dfs[i], i, 0);
        SobelFilter::cgraArch->getNetBranch(i)->createRouteTable();
        SobelFilter::cgraArch->getNet(i)->createRouteTable();
    }

    s = high_resolution_clock::now();
    int r = scheduler.scheduling();
    diff = high_resolution_clock::now() - s;

    SobelFilter::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../sobel_files/%s.cgra", dfs[0]->getName().c_str());
        SobelFilter::cgraArch->writeCgraProgram(filename);
        sprintf(filename, "../sobel_files/%s.dot", dfs[0]->getName().c_str());
        dfs[0]->toDot(filename);
    } else {
        printf("Error on scheduling: Code %d\n", r);
    }
    for (auto df:dfs) {
        delete df;
    }
    return r == SCHEDULE_SUCCESS;
}

DataFlow *SobelFilter::createDataFlow(int id) {
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

void SobelFilter::printStatistics() {
    auto df = SobelFilter::createDataFlow(0);
    MSG("INFO Sobel Filter Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << SobelFilter::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << SobelFilter::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << SobelFilter::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << SobelFilter::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << SobelFilter::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << SobelFilter::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << SobelFilter::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << SobelFilter::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << SobelFilter::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Scheduling time: " << SobelFilter::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << SobelFilter::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << SobelFilter::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << SobelFilter::cpuExecTime << "ms");
    SobelFilter::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    SobelFilter::cgraHw->getAccManagement()->printHwInfo();
    delete df;
}


