//
// Created by lucas on 01/02/19.
//

#include <Paeth.h>

Paeth::Paeth(Cgra *cgra, CgraArch *cgraArch) {
    Paeth::cgraHw = cgra;
    Paeth::cgraArch = cgraArch;
    Paeth::schedulingTime = 0;
    Paeth::cpuExecTime = 0;
    Paeth::cgraExecTime = 0;
    Paeth::cgraConfTime = 0;
}

Paeth::~Paeth() = default;

void Paeth::runCGRA(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};

    Paeth::cgraHw->loadCgraProgram("../cgra_bitstreams/paeth.cgra");
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < copies; ++j) {
            Paeth::cgraHw->setCgraProgramInputStreamByID(i, j * 4, data_in[i][j][0], sizeof(short) * data_size);
            Paeth::cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 1, data_in[i][j][1], sizeof(short) * data_size);
            Paeth::cgraHw->setCgraProgramInputStreamByID(i, (j * 4) + 2, data_in[i][j][2], sizeof(short) * data_size);
            Paeth::cgraHw->setCgraProgramOutputStreamByID(i, (j * 4) + 3, data_out[i][j], sizeof(short) * data_size);
        }
    }
    s = high_resolution_clock::now();
    Paeth::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    Paeth::cgraExecTime = diff.count() * 1000;

}

void Paeth::runCPU(unsigned short ****data_in, unsigned short ***data_out, int data_size, int numThreads, int copies) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int j = 0; j < NUM_THREADS; j++) {
        for (int k = 0; k < copies; ++k) {
            for (int i = 0; i < data_size; ++i) {
                int pas, pbs, pcs;
                bool test_1, test_2, test_3, test_4;
                pas = data_in[j][k][1][i] - data_in[j][k][2][i];
                pbs = data_in[j][k][0][i] - data_in[j][k][2][i];
                pcs = data_in[j][k][0][i] | (data_in[j][k][1][i] - 2 * data_in[j][k][2][i]);
                test_1 = abs(pas) <= abs(pbs);
                test_2 = abs(pas) <= abs(pcs);
                test_4 = abs(pbs) <= abs(pcs);
                test_3 = test_1 && test_2;
                if (test_3) data_out[j][k][i] = data_in[j][k][0][i];
                else if (test_4) data_out[j][k][i] = data_in[j][k][1][i];
                else data_out[j][k][i] = data_in[j][k][2][i];
            }
        }

    }
    diff = high_resolution_clock::now() - s;
    Paeth::cpuExecTime = diff.count() * 1000;
}

bool Paeth::compile(int numThreads, int copies) {
    Scheduler scheduler(Paeth::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;
    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(Paeth::createDataFlow(i,copies));
        scheduler.addDataFlow(dfs[i], i, 0);
        Paeth::cgraArch->getNetBranch(i)->createRouteTable();
        Paeth::cgraArch->getNet(i)->createRouteTable();
    }

    int r = 0;
    for (int j = 0; j < 100; ++j) {
        s = high_resolution_clock::now();
        r = scheduler.scheduling();
        diff = high_resolution_clock::now() - s;
        if(r == SCHEDULE_SUCCESS )
            break;
    }

    Paeth::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../cgra_bitstreams/%s.cgra", dfs[0]->getName().c_str());
        Paeth::cgraArch->writeCgraProgram(filename);
    } else {
        printf("Error on scheduling: Code %d\n", r);
    }
    for (auto df:dfs) {
        delete df;
    }
    return r == SCHEDULE_SUCCESS;
}

DataFlow *Paeth::createDataFlow(int id, int copies) {
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

void Paeth::printStatistics() {
    auto df = Paeth::createDataFlow(0,2);
    MSG("INFO Paeth Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << Paeth::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << Paeth::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << Paeth::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << Paeth::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << Paeth::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << Paeth::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << Paeth::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << Paeth::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << Paeth::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Num total edges: " << df->getNumEdges());
    MSG("INFO Scheduling time: " << Paeth::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Paeth::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Paeth::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Paeth::cpuExecTime << "ms");
    Paeth::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Paeth::cgraHw->getAccManagement()->printHwInfo();
    delete df;
}

void Paeth::benchmarking(int numThreads, int data_size) {

    int copies = 2;
    unsigned short ****data_in;
    unsigned short ***data_out_cpu;
    unsigned short ***data_out_cgra;

    data_in = new unsigned short ***[numThreads];
    data_out_cpu = new unsigned short **[numThreads];
    data_out_cgra = new unsigned short **[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        data_in[i] = new unsigned short **[copies];
        data_out_cpu[i] = new unsigned short *[copies];
        data_out_cgra[i] = new unsigned short *[copies];
        for (int j = 0; j < copies; ++j) {
            data_in[i][j] = new unsigned short *[3];
            data_in[i][j][0] = new unsigned short[data_size];
            data_in[i][j][1] = new unsigned short[data_size];
            data_in[i][j][2] = new unsigned short[data_size];
            data_out_cpu[i][j] = new unsigned short[data_size];
            data_out_cgra[i][j] = new unsigned short[data_size];
        }
    }

    for (int t = 0; t < numThreads; ++t) {
        for (int c = 0; c < copies; ++c) {
            for (int i = 0; i < data_size; ++i) {
                data_in[t][c][0][i] = (unsigned short) (random() % 256);
                data_in[t][c][1][i] = (unsigned short) (random() % 256);
                data_in[t][c][2][i] = (unsigned short) (random() % 256);

                data_out_cpu[t][c][i] = 0;
                data_out_cgra[t][c][i] = 0;
            }
        }
    }

    if (Paeth::compile(numThreads,copies)) {
        Paeth::runCGRA(data_in, data_out_cgra, data_size, numThreads,copies);
        Paeth::runCPU(data_in, data_out_cpu, data_size, numThreads,copies);
        for (int t = 0; t < numThreads; ++t) {
            for (int c = 0; c < copies; ++c) {
                for (int i = 0; i < data_size; ++i) {
                    if (data_out_cpu[t][c][i] != data_out_cgra[t][c][i]) {
                        printf("Error: Thread %d, copy %d, index %d, expected %d found %d!\n", t, c, i,
                               data_out_cpu[t][c][i],
                               data_out_cgra[t][c][i]);
                        break;
                    }
                }
            }
        }
    } else {
        printf("Compilation failed!\n");
    }
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < copies; ++j) {
            delete data_in[i][j][0];
            delete data_in[i][j][1];
            delete data_in[i][j][2];
            delete data_in[i][j];
            delete data_out_cpu[i][j];
            delete data_out_cgra[i][j];
        }
        delete data_in[i];
        delete data_out_cpu[i];
        delete data_out_cgra[i];
    }

    delete data_in;
    delete data_out_cpu;
    delete data_out_cgra;

}
