//
// Created by lucas on 14/01/19.
//

#include <test.h>

void Test::runLoopBack() {
    CgraArch cgraArch(0, 128, 8, 8, 8, 1, 2);
    Scheduler scheduler(&cgraArch);
    Cgra cgra;

    int numThread = 8;
    int copies = 8;
    int data_size = 32;

    short ***data_in_a;
    short ***data_out;

    data_in_a = new short **[numThread];
    data_out = new short **[numThread];

    for (int i = 0; i < numThread; ++i) {
        data_in_a[i] = new short *[copies];
        data_out[i] = new short *[copies];
    }
    for (int i = 0; i < numThread; ++i) {
        for (int j = 0; j < copies; ++j) {
            data_in_a[i][j] = new short[data_size];
            data_out[i][j] = new short[data_size];
        }
    }
    for (int k = 0; k < numThread; ++k) {
        for (int j = 0; j < copies; ++j) {
            for (int i = 0; i < data_size; ++i) {
                data_in_a[k][j][i] = (short) (k + 1);
                data_out[k][j][i] = 0;
            }
        }
    }

    for (int l = 0; l < numThread; ++l) {
        auto df = Samples::loopback(l, 1, copies);
        df->toDot("../dot_files/loopback.dot");
        scheduler.addDataFlow(df, l, 0);
    }
    int r = scheduler.scheduling();
    if (r == SCHEDULE_SUCCESS) {
        cgra.loadCgraProgram(cgraArch.getCgraProgram());
        for (int i = 0; i < numThread; ++i) {
            for (int j = 0; j < copies; ++j) {
                cgra.setCgraProgramInputStreamByID(i, j, data_in_a[i][j], sizeof(short) * data_size);
                cgra.setCgraProgramOutputStreamByID(i, j + copies, data_out[i][j], sizeof(short) * data_size);
            }
        }
        cgra.syncExecute(0);
        printf("\n");
        for (int k = 0; k < numThread; ++k) {
            printf("Thread %d data in a:\n", k);
            for (int i = 0; i < copies; ++i) {
                printf("Copie %d: [ ", i);
                for (int j = 0; j < data_size; ++j) {
                    printf("%d ", data_in_a[k][i][j]);
                }
                printf("]\n");
            }
            printf("\n");
        }
        for (int k = 0; k < numThread; ++k) {
            printf("Thread %d data out:\n", k);
            for (int i = 0; i < copies; ++i) {
                printf("Copie %d: [ ", i);
                for (int j = 0; j < data_size; ++j) {
                    printf("%d ", data_out[k][i][j]);
                }
                printf("]\n");
            }
            printf("\n");
        }
        printf("\n");
    } else {
        printf("Error: %d\n",r);
    }
}

void Test::runFir() {
    CgraArch cgraArch(0, 128, 8, 8, 8, 1, 2);
    Scheduler scheduler;
    Cgra cgra;
    int numThread = 1;
    int copies = 1;
    int firSize = 48;
    int data_size = 1;

    short ***coef;
    short ***data_in;
    short ***data_out;

    scheduler.setCgra(&cgraArch);

    coef = new short **[numThread];
    data_in = new short **[numThread];
    data_out = new short **[numThread];

    for (int i = 0; i < numThread; ++i) {
        coef[i] = new short *[copies];
        data_in[i] = new short *[copies];
        data_out[i] = new short *[copies];
    }
    for (int i = 0; i < numThread; ++i) {
        for (int j = 0; j < copies; ++j) {
            coef[i][j] = new short[firSize];
            data_in[i][j] = new short[data_size];
            data_out[i][j] = new short[data_size];
        }
    }
    for (int k = 0; k < numThread; ++k) {
        for (int j = 0; j < copies; ++j) {
            for (int i = 0; i < firSize; ++i) {
                coef[k][j][i] = (short) (i + 1);// ((random() % 10) + 1);
            }
        }

        for (int j = 0; j < copies; ++j) {
            for (int i = 0; i < data_size; ++i) {
                data_in[k][j][i] = (short) (i + 1);// (random() % 10 + 1);
                data_out[k][j][i] = 0;
            }
        }
    }

    for (int l = 0; l < numThread; ++l) {
        auto df = Samples::fir(l, coef[l], firSize, copies);
        df->toDot("../dot_files/fir.dot");
        scheduler.addDataFlow(df, l, 0);
    }
    int r = scheduler.scheduling();
    if (r == SCHEDULE_SUCCESS) {
        cgra.loadCgraProgram(cgraArch.getCgraProgram());
        for (int i = 0; i < numThread; ++i) {
            for (int j = 0; j < copies; ++j) {
                cgra.setCgraProgramInputStreamByID(i, j, data_in[i][j], sizeof(short) * data_size);
                cgra.setCgraProgramOutputStreamByID(i, j + copies, data_out[i][j], sizeof(short) * data_size);
            }
        }
        cgra.syncExecute(0);
        printf("\n");
        for (int k = 0; k < numThread; ++k) {
            printf("Thread %d data in:\n", k);
            for (int i = 0; i < copies; ++i) {
                printf("Copie %d: [ ", i);
                for (int j = 0; j < data_size; ++j) {
                    printf("%d ", data_in[k][i][j]);
                }
                printf("]\n");
            }
            printf("\n");
        }
        for (int k = 0; k < numThread; ++k) {
            printf("Thread %d data out:\n", k);
            for (int i = 0; i < copies; ++i) {
                printf("Copie %d: [ ", i);
                for (int j = 0; j < data_size; ++j) {
                    printf("%d ", data_out[k][i][j]);
                }
                printf("]\n");
            }
            printf("\n");
        }
        printf("\n");
    } else {
        printf("Error: %d\n",r);
    }
}

void Test::runVetSum() {
    CgraArch cgraArch(0, 128, 8, 8, 8, 1, 2);
    Scheduler scheduler(&cgraArch);
    Cgra cgra;
    int numThread = 8;
    int copies = 4;
    int data_size = 32;

    short ***data_in_a;
    short ***data_in_b;
    short ***data_out;

    data_in_a = new short **[numThread];
    data_in_b = new short **[numThread];
    data_out = new short **[numThread];

    for (int i = 0; i < numThread; ++i) {
        data_in_a[i] = new short *[copies];
        data_in_b[i] = new short *[copies];
        data_out[i] = new short *[copies];
    }
    for (int i = 0; i < numThread; ++i) {
        for (int j = 0; j < copies; ++j) {
            data_in_a[i][j] = new short[data_size];
            data_in_b[i][j] = new short[data_size];
            data_out[i][j] = new short[data_size];
        }
    }
    for (int k = 0; k < numThread; ++k) {
        for (int j = 0; j < copies; ++j) {
            for (int i = 0; i < data_size; ++i) {
                data_in_a[k][j][i] = (short) (k + 1);//(random() % 255 + 1);
                data_in_b[k][j][i] = (short) (k + 1);//(random() % 255 + 1);
                data_out[k][j][i] = 0;
            }
        }
    }

    for (int l = 0; l < numThread; ++l) {
        auto df = Samples::vetorialSum(l, copies);
        df->toDot("../dot_files/vetorial_sum.dot");
        scheduler.addDataFlow(df, l, 0);
    }

    int r = scheduler.scheduling();
    if (r == SCHEDULE_SUCCESS) {
        cgra.loadCgraProgram(cgraArch.getCgraProgram());
        for (int i = 0; i < numThread; ++i) {
            for (int j = 0; j < copies; ++j) {
                cgra.setCgraProgramInputStreamByID(i, j * 2, data_in_a[i][j], sizeof(short) * data_size);
                cgra.setCgraProgramInputStreamByID(i, j * 2 + 1, data_in_b[i][j], sizeof(short) * data_size);
                cgra.setCgraProgramOutputStreamByID(i, j + (copies * 2), data_out[i][j], sizeof(short) * data_size);
            }
        }
        cgra.syncExecute(0);
        printf("\n");
        for (int k = 0; k < numThread; ++k) {
            printf("Thread %d data in a:\n", k);
            for (int i = 0; i < copies; ++i) {
                printf("Copie %d: [ ", i);
                for (int j = 0; j < data_size; ++j) {
                    printf("%d ", data_in_a[k][i][j]);
                }
                printf("]\n");
            }
            printf("\n");
        }
        for (int k = 0; k < numThread; ++k) {
            printf("Thread %d data in b:\n", k);
            for (int i = 0; i < copies; ++i) {
                printf("Copie %d: [ ", i);
                for (int j = 0; j < data_size; ++j) {
                    printf("%d ", data_in_b[k][i][j]);
                }
                printf("]\n");
            }
            printf("\n");
        }
        for (int k = 0; k < numThread; ++k) {
            printf("Thread %d data out:\n", k);
            for (int i = 0; i < copies; ++i) {
                printf("Copie %d: [ ", i);
                for (int j = 0; j < data_size; ++j) {
                    printf("%d ", data_out[k][i][j]);
                }
                printf("]\n");
            }
            printf("\n");
        }
        printf("\n");

    } else {
        printf("Error: %d\n",r);
    }
}

void Test::runKmeans() {
    CgraArch cgraArch(0, 128, 8, 8, 8, 1, 2);
    Scheduler scheduler(&cgraArch);
    Cgra cgra;
    int d = 8;
    int k = 4;
    int data_size = 32;

    short **data_in_a;
    short *data_out;
    short *centroids;

    data_in_a = new short *[d];
    data_out = new short[data_size];
    centroids = new short[d * k];

    for (int l = 0; l < d * k; ++l) {
        centroids[l] = (short) (random() % 255 + 1);
    }
    for (int n = 0; n < d; ++n) {
        data_in_a[n] = new short[data_size];
        for (int i = 0; i < data_size; ++i) {
            data_in_a[n][i] = (short) (random() % 255 + 1);
            data_out[i] = 0;
        }
    }
    auto df = Samples::kmeans(0, k, d);
    df->toDot("../dot_files/kmeans.dot");
    scheduler.addDataFlow(df, 0, 0);
    int r = scheduler.scheduling();
    if (r == SCHEDULE_SUCCESS) {
        cgra.loadCgraProgram(cgraArch.getCgraProgram());
        for (int i = 0; i < d; ++i) {
            cgra.setCgraProgramInputStreamByID(0, (k * d) + i, data_in_a[i], sizeof(short) * data_size);
        }
        cgra.setCgraProgramOutputStreamByID(0, k * d + d, data_out, sizeof(short) * data_size);
        for (int j = 0; j < 10; ++j) {
            cgra.syncExecute(0);
            Test::helpKmeansUpdateCentroids(data_in_a, data_out, centroids, k, d, data_size);
            Test::helpKmeansUpdateConstants(cgra.getCgraProgram(), 0, centroids, k, d);
        }

        for (int i1 = 0; i1 < d; ++i1) {
            printf("k %d: [ ", i1);
            for (int i = 0; i < k; ++i) {
                printf("%d ", centroids[i * d + i1]);
            }
            printf("]\n");
        }

        for (int j = 0; j < d; ++j) {
            printf("D%d: [", j);
            for (int i = 0; i < data_size; ++i) {
                printf("%d ", data_in_a[j][i]);
            }
            printf("]\n");
        }
        printf("\nOUT: [");
        for (int i = 0; i < data_size; ++i) {
            printf("%d ", data_out[i]);
        }
        printf("]\n");
    } else {
        printf("Error: %d\n",r);
    }



}

void Test::helpKmeansUpdateCentroids(short **data_in, short *data_out, short *centroids, int k, int d, int data_size) {
    short kqtd[k];
    short ksum[d];
    for (int i = 0; i < k; ++i) {
        kqtd[i] = 0;
    }
    for (int l = 0; l < d; ++l) {
        ksum[l] = 0;
        for (int i = 0; i < data_size; ++i) {
            ksum[l] += data_in[l][i];
            kqtd[data_out[i]]++;
        }
        for (int j = 0; j < k; ++j) {
            if (kqtd[j] != 0) {
                centroids[j * d + l] = ksum[l] / kqtd[j];
            }
        }
    }
}

void Test::helpKmeansUpdateConstants(cgra_program_t *cgra_program, int thread, short *constants, int k, int d) {
    for (int i = 0; i < cgra_program->initial_conf.size(); ++i) {
        if (cgra_program->initial_conf[i].pe_constant_conf.conf_type == CGRA_CONF_SET_PE_CONST) {
            int pe_id = (int) cgra_program->initial_conf[i].pe_constant_conf.pe_id;
            int op_id = cgra_program->map_pe_to_op[pe_id].second;
            int thread_id = cgra_program->map_pe_to_op[pe_id].first;
            if (thread_id == thread) {
                if (op_id < k * d) {
                    cgra_program->initial_conf[i].pe_constant_conf.constant = (unsigned long) constants[op_id];
                }
            }
        }
    }
}

int Test::runSobelFilter() {

    char file_in[] = {"../src/img.rgb"};
    char file_out[] = {"../src/img.gray"};

    byte *rgb, *gray, *contour_img;
    int rgb_size, width = 8, height = 8;

    rgb_size = width * height * 3;

    readFile(file_in, &rgb, rgb_size);

    int gray_size = sobelFilter(rgb, &gray, &contour_img, width, height);

    writeFile(file_out, contour_img, gray_size);

    return 0;
}

int Test::sobelFilter(byte *rgb, byte **gray, byte **contour_img, int width, int height) {

    CgraArch cgraArch(0, 128, 8, 8, 8, 1, 2);
    Scheduler scheduler(&cgraArch);
    Cgra cgra;

    auto df = Samples::sobelFilter();

    short **inputs;
    short **output;

    int rgb_size = width * height * 3;
    int gray_size = rgbToGray(rgb, gray, rgb_size);

    inputs = new short *[8];
    output = new short *[1];
    output[0] = new short[gray_size];
    output[1] = new short[gray_size];
    *contour_img = (byte *) malloc(sizeof(byte) * gray_size);

    for (int k = 0; k < 8; ++k) {
        inputs[k] = new short[gray_size];
    }
    for (int l = 0; l < gray_size; ++l) {
        output[0][l] = 0;
    }

    Test::makeOpMem(*gray, gray_size, width, inputs);

    scheduler.addDataFlow(df, 0, 0);
    df->toDot("../dot_files/sobel_filter.dot");

    int r = scheduler.scheduling();
    if (r == SCHEDULE_SUCCESS) {
        cgra.loadCgraProgram(cgraArch.getCgraProgram());
        for (int i = 0; i < 8; ++i) {
            cgra.setCgraProgramInputStreamByID(0, i, inputs[i], sizeof(short) * gray_size);
        }
        cgra.setCgraProgramOutputStreamByID(0, 8, output[0], sizeof(short) * gray_size);
        cgra.syncExecute(0);
        for (int j = 0; j < gray_size; ++j) {
            (*contour_img)[j] = (byte) (255 - sqrt(output[0][j]));
        }
    } else {
        printf("Error: %d\n",r);
        exit(0);
    }
    return gray_size;
}

int Test::rgbToGray(byte *rgb, byte **gray, int buffer_size) {

    int gray_size = buffer_size / 3;
    *gray = (byte *) malloc(sizeof(byte) * gray_size);

    byte *p_rgb = rgb;
    byte *p_gray = *gray;

    for (int i = 0; i < gray_size; i++) {
        *p_gray = (byte)(0.30 * p_rgb[0] + 0.59 * p_rgb[1] + 0.11 * p_rgb[2]);
        p_rgb += 3;
        p_gray++;
    }

    return gray_size;
}

void Test::makeOpMem(byte *buffer, int buffer_size, int width, short **op_mem) {
    for (int cindex = 0; cindex < buffer_size; cindex++) {
        int bottom = cindex - width < 0;
        int top = cindex + width >= buffer_size;
        int left = cindex % width == 0;
        int right = (cindex + 1) % width == 0;
        byte zero = 0;

        op_mem[0][cindex] = !bottom && !left ? buffer[cindex - width - 1] : zero;
        op_mem[1][cindex] = !bottom ? buffer[cindex - width] : zero;
        op_mem[2][cindex] = !bottom && !right ? buffer[cindex - width + 1] : zero;

        op_mem[3][cindex] = !left ? buffer[cindex - 1] : zero;
        //op_mem[4][cindex] = buffer[cindex];
        op_mem[4][cindex] = !right ? buffer[cindex + 1] : zero;

        op_mem[5][cindex] = !top && !left ? buffer[cindex + width - 1] : zero;
        op_mem[6][cindex] = !top ? buffer[cindex + width] : zero;
        op_mem[7][cindex] = !top && !right ? buffer[cindex + width + 1] : zero;
    }

}

void Test::readFile(char *file_name, byte **buffer, int buffer_size) {

    FILE *file = fopen(file_name, "r");
    *buffer = (byte *) malloc(sizeof(byte) * buffer_size);
    for (int i = 0; i < buffer_size; i++) {
        (*buffer)[i] = (byte)fgetc(file);
    }
    fclose(file);
}

void Test::writeFile(char *file_name, byte *buffer, int buffer_size) {

    FILE *file = fopen(file_name, "w");
    for (int i = 0; i < buffer_size; i++) {
        fputc(buffer[i], file);
    }
    fclose(file);
}

void Test::schedulingTimeMeasureBenchmarks() {

    CgraArch cgraArch(0,128,8,8,8,1,2);
    Scheduler scheduler(&cgraArch);

    printf("Scheduling Time Measure\n\n");
    printf("CGRA target architecture:\n");
    printf("Number of Threads: %d\n",cgraArch.getNumThreads());
    printf("Number of PEs: %d\n",cgraArch.getNumPe());
    printf("Number of input PEs: %d\n",cgraArch.getNumPeIn());
    printf("Number of output PEs: %d\n",cgraArch.getNumPeOut());
    printf("Omega net PEs size: %d\n",cgraArch.getNet(0)->getSize());
    printf("Omega net PEs radix: %d\n",cgraArch.getNet(0)->getRadix());
    printf("Omega net branch size: %d\n",cgraArch.getNetBranch(0)->getSize());
    printf("Omega net branch radix: %d\n",cgraArch.getNetBranch(0)->getRadix());
    printf("Word size(bits): %d\n",cgraArch.getWordSize()*8);

    high_resolution_clock::time_point s;
    duration<double> diff={};
    std::vector<DataFlow *>dataflows;
    short **coef;
    coef = new short *[1];
    coef[0] = new short[48];
    int numThreads = 1;

    s = high_resolution_clock::now();
    cgraArch.getNetBranch(0)->createRouteTable();
    cgraArch.getNet(0)->createRouteTable();
    diff = high_resolution_clock::now() - s;

    printf("\nTime to create omega net routing tables: %.3lfms\n\n", diff.count() * 1000);

    for (int i = 0; i < 48; ++i) {
        coef[0][i] = (short)(i+1);
    }

    dataflows.push_back(Samples::loopback(0,1,8));
    dataflows.push_back(Samples::vetorialSum(0,4));
    dataflows.push_back(Samples::sobelFilter());
    dataflows.push_back(Samples::kmeans(0,4,8));
    dataflows.push_back(Samples::fir(0, coef, 48, 1));

    for(auto df:dataflows) {
        scheduler.reset();
        for (int i = 0; i < numThreads; ++i) {
            scheduler.addDataFlow(df,i,0);
        }
        s = high_resolution_clock::now();
        int r = scheduler.scheduling();
        diff = high_resolution_clock::now() - s;
        if(r==SCHEDULE_SUCCESS){
            printf("DataFlow \"%s\" successfully scheduled!\n",df->getName().c_str());
            printf("Num input nodes: %d\n",df->getNumOpIn());
            printf("Num output nodes: %d\n",df->getNumOpOut());
            printf("Num total nodes: %d\n",df->getNumOp());
            printf("Execution Time: %.3lfms\n\n", diff.count() * 1000);
            char file[100];
            sprintf(file,"../cgra_prog_files/%s.cgra",df->getName().c_str());
            cgraArch.writeCgraProgram(file);
        }
    }
}
