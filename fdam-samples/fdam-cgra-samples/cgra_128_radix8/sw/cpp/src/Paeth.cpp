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

void Paeth::runCGRA(unsigned short ***data_in, unsigned short **data_out, int data_size, int numThreads) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};

    Paeth::cgraHw->loadCgraProgram("../paeth_files/paeth.cgra");
    for (int i = 0; i < numThreads; ++i) {
        Paeth::cgraHw->setCgraProgramInputStreamByID(i, 0, data_in[0][i], sizeof(short) * data_size);
        Paeth::cgraHw->setCgraProgramInputStreamByID(i, 1, data_in[1][i], sizeof(short) * data_size);
        Paeth::cgraHw->setCgraProgramInputStreamByID(i, 2, data_in[2][i], sizeof(short) * data_size);
        Paeth::cgraHw->setCgraProgramOutputStreamByID(i, 3, data_out[i], sizeof(short) * data_size);
    }
    s = high_resolution_clock::now();
    Paeth::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    Paeth::cgraExecTime = diff.count() * 1000;

}

void Paeth::runCPU(unsigned short ***data_in, unsigned short **data_out, int data_size, int numThreads) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int j = 0; j < NUM_THREADS; j++) {
        for (int i = 0; i < data_size; ++i) {
            int pas, pbs, pcs;
            bool test_1, test_2, test_3, test_4;
            pas = data_in[1][j][i] - data_in[2][j][i];
            pbs = data_in[0][j][i] - data_in[2][j][i];
            pcs = data_in[0][j][i] | (data_in[1][j][i] - 2 * data_in[2][j][i]);
            test_1 = abs(pas) <= abs(pbs);
            test_2 = abs(pas) <= abs(pcs);
            test_4 = abs(pbs) <= abs(pcs);
            test_3 = test_1 && test_2;
            if (test_3) data_out[j][i] = data_in[0][j][i];
            else if (test_4) data_out[j][i] = data_in[1][j][i];
            else data_out[j][i] = data_in[2][j][i];
        }
    }
    diff = high_resolution_clock::now() - s;
    Paeth::cpuExecTime = diff.count() * 1000;
}

void Paeth::compile(int numThreads) {
    Scheduler scheduler(Paeth::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;
    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(Paeth::createDataFlow(i));
        scheduler.addDataFlow(dfs[i], i, 0);
        Paeth::cgraArch->getNetBranch(i)->createRouteTable();
        Paeth::cgraArch->getNet(i)->createRouteTable();
    }

    s = high_resolution_clock::now();
    int r = scheduler.scheduling();
    diff = high_resolution_clock::now() - s;

    Paeth::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../paeth_files/%s.cgra", dfs[0]->getName().c_str());
        Paeth::cgraArch->writeCgraProgram(filename);
        sprintf(filename, "../paeth_files/%s.dot", dfs[0]->getName().c_str());
        dfs[0]->toDot(filename);
    } else {
        printf("Error on scheduling: Code %d\n", r);
    }
    for (auto df:dfs) {
        delete df;
    }
}

DataFlow *Paeth::createDataFlow(int id) {
    auto df = new DataFlow(id, "paeth");
    int idx = 0;
    auto inA = new InputStream(idx++);
    auto inB = new InputStream(idx++);
    auto inC = new InputStream(idx++);
    auto out = new OutputStream(idx++);
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
    df->connect(inA, regA1, regA1->getPortB());
    df->connect(regA1, regA2, regA2->getPortB());
    df->connect(regA2, regA3, regA3->getPortB());
    df->connect(regA3, regA4, regA4->getPortB());
    df->connect(regA4, regA5, regA5->getPortB());
    df->connect(regA5, regA6, regA6->getPortB());
    df->connect(regA6, regA7, regA7->getPortB());
    df->connect(regA7, regA8, regA8->getPortB());
    df->connect(regA8, regA9, regA9->getPortB());
    df->connect(inB, regB1, regB1->getPortB());
    df->connect(regB1, regB2, regB2->getPortB());
    df->connect(regB2, regB3, regB3->getPortB());
    df->connect(regB3, regB4, regB4->getPortB());
    df->connect(regB4, regB5, regB5->getPortB());
    df->connect(regB5, regB6, regB6->getPortB());
    df->connect(regB6, regB7, regB7->getPortB());
    df->connect(inC, regC1, regC1->getPortB());
    df->connect(regC1, regC2, regC2->getPortB());
    df->connect(regC2, regC3, regC3->getPortB());
    df->connect(regC3, regC4, regC4->getPortB());
    df->connect(regC4, regC5, regC5->getPortB());
    df->connect(regC5, regC6, regC6->getPortB());
    df->connect(regC6, regC7, regC7->getPortB());
    df->connect(inA, sub1, sub1->getPortA());
    df->connect(inC, sub1, sub1->getPortB());
    df->connect(inB, sub2, sub2->getPortA());
    df->connect(inC, sub2, sub2->getPortB());
    df->connect(inC, m1, m1->getPortA());
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
    df->connect(muxCBA, out, out->getPortA());
    return df;
}

void Paeth::printStatistics() {
    auto df = Paeth::createDataFlow(0);
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
    MSG("INFO Scheduling time: " << Paeth::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Paeth::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Paeth::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Paeth::cpuExecTime << "ms");
    Paeth::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Paeth::cgraHw->getAccManagement()->printHwInfo();
    delete df;
}

void Paeth::benchmarking(int numThreads) {

    int data_size = 134217727;
    unsigned short ***data_in;
    unsigned short **data_out_cpu;
    unsigned short **data_out_cgra;

    data_in = new unsigned short **[3];
    data_in[0] = new unsigned short *[numThreads];
    data_in[1] = new unsigned short *[numThreads];
    data_in[2] = new unsigned short *[numThreads];
    data_out_cpu = new unsigned short *[numThreads];
    data_out_cgra = new unsigned short *[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        data_in[0][i] = new unsigned short[data_size];
        data_in[1][i] = new unsigned short[data_size];
        data_in[2][i] = new unsigned short[data_size];
        data_out_cpu[i] = new unsigned short[data_size];
        data_out_cgra[i] = new unsigned short[data_size];
    }
    for (int k = 0; k < numThreads; ++k) {
        for (int i = 0; i < data_size; ++i) {
            data_in[0][k][i] = 1;
            data_in[1][k][i] = 2;
            data_in[2][k][i] = 3;
            data_out_cpu[k][i] = 0;
            data_out_cgra[k][i] = 0;
        }
    }

    Paeth::compile(numThreads);
    Paeth::runCGRA(data_in, data_out_cgra, data_size, numThreads);
    Paeth::runCPU(data_in, data_out_cpu, data_size, numThreads);

    bool flag_error = false;
    for (int k = 0; k < numThreads; ++k) {
        for (int j = 0; j < data_size; ++j) {
            if (data_out_cpu[k][j] != data_out_cgra[k][j]) {
                printf("Error: Thread %d, index %d, expected %d found %d!\n", k, j, data_out_cpu[k][j],
                       data_out_cgra[k][j]);
                flag_error = true;
                break;
            }
        }
    }
    if (!flag_error) {
        printf("Success!\n");
    }

    for (int i = 0; i < numThreads; ++i) {
        delete data_in[0][i];
        delete data_in[1][i];
        delete data_in[2][i];
        delete data_out_cpu[i];
        delete data_out_cgra[i];
    }

    delete data_in[0];
    delete data_in[1];
    delete data_in[2];
    delete data_in;
    delete data_out_cpu;
    delete data_out_cgra;

}
