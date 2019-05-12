//
// Created by lucas on 31/01/19.
//

#include <Kmeans.h>
#include <algorithm>

Kmeans::Kmeans(Cgra *cgra, CgraArch *cgraArch, int num_dim, int num_clusters) {
    Kmeans::cgraHw = cgra;
    Kmeans::cgraArch = cgraArch;
    Kmeans::schedulingTime = 0;
    Kmeans::cpuExecTime = 0;
    Kmeans::cgraExecTime = 0;
    Kmeans::cgraConfTime = 0;
    Kmeans::num_clusters = num_clusters;
    Kmeans::num_dim = num_dim;
}

Kmeans::~Kmeans() = default;

DataFlow *Kmeans::createDataFlow(int id) {
    auto df = new DataFlow(id, "kmeans");
    int idx = 0;
    std::vector<Operator *> inputs;
    std::map<int, std::vector<Operator *>> subs;
    std::map<int, std::vector<Operator *>> abs;
    std::vector<Operator *> adds_end;
    std::vector<Operator *> aux;
    std::vector<Operator *> aux_reduz;
    std::vector<Operator *> slt_reduz;
    std::queue<Operator *> mux_reduz;
    std::vector<Operator *> constants;
    int outId;
    if (Kmeans::num_clusters > 1) {
        for (int j = 0; j < Kmeans::num_clusters; ++j) {
            for (int i = 0; i < Kmeans::num_dim; ++i) {
                auto s = new Subi(idx++, j * Kmeans::num_dim + i);
                subs[j].push_back(s);
            }
        }
        inputs.reserve(static_cast<unsigned long>(Kmeans::num_dim));
        for (int i = 0; i < Kmeans::num_dim; ++i) {
            inputs.push_back(new InputStream(idx++));
        }
        outId = idx++;
        for (int j = 0; j < Kmeans::num_clusters; ++j) {
            for (int i = 0; i < Kmeans::num_dim; ++i) {
                df->connect(inputs[i], subs[j][i], subs[j][i]->getPortA());
            }
            for (auto s:subs[j]) {
                //auto a = new Abs(idx++);
                //df->connect(s, a, a->getPortA());
                abs[j].push_back(s);
            }
            aux.clear();
            aux_reduz.clear();
            for (auto a:abs[j]) {
                aux.push_back(a);
            }
            while (aux.size() > 1) {
                int start = 0;
                if (aux.size() % 2 != 0) {
                    auto passA = new PassA(idx++);
                    df->connect(aux[0], passA, passA->getPortA());
                    aux_reduz.push_back(passA);
                    start = 1;
                }
                for (int l = start; l < aux.size(); l += 2) {
                    auto add = new Add(idx++);
                    df->connect(aux[l], add, add->getPortA());
                    df->connect(aux[l + 1], add, add->getPortB());
                    aux_reduz.push_back(add);
                }
                aux.clear();
                for (auto a:aux_reduz) {
                    aux.push_back(a);
                }
                aux_reduz.clear();
            }
            for (auto a:aux) {
                adds_end.push_back(a);
            }
        }
        aux.clear();
        aux_reduz.clear();
        for (auto a:adds_end) {
            aux.push_back(a);
        }
        sort(aux.begin(), aux.end(), Kmeans::compare);
        int reg = 0;
        auto addEnd = aux[aux.size() - 1];

        while (aux.size() > 1) {
            for (int l = 0; l < aux.size() - 1; l += 2) {
                auto slt = new Slt(idx++);
                auto mux = new Mux(idx++);
                df->connect(aux[l], slt, slt->getPortA());
                df->connect(aux[l + 1], slt, slt->getPortB());
                df->connect(slt, mux, mux->getPortBranch());
                aux_reduz.push_back(slt);
                if (reg > 0) {
                    df->connect(mux_reduz.front(), mux, mux->getPortA());
                    mux_reduz.pop();
                    df->connect(mux_reduz.front(), mux, mux->getPortB());
                    mux_reduz.pop();
                } else {
                    auto reg1 = new PassBi(idx++, l);
                    auto reg2 = new PassBi(idx++, l + 1);
                    df->connect(reg1, mux, mux->getPortA());
                    df->connect(reg2, mux, mux->getPortB());
                }
                mux_reduz.push(mux);
            }
            aux.clear();
            for (auto a:aux_reduz) {
                aux.push_back(a);
            }
            aux_reduz.clear();
            reg++;
        }
        if (Kmeans::num_clusters % 2 != 0) {
            auto rr = addEnd;
            for (int i = 0; i < reg; ++i) {
                auto r = new PassA(idx++);
                df->connect(rr, r, r->getPortA());
                rr = r;
            }
            auto sltEnd = new Slt(idx++);
            auto muxEnd = new Mux(idx++);
            auto reg1 = new PassBi(idx++, Kmeans::num_clusters - 1);

            df->connect(aux[0], sltEnd, sltEnd->getPortA());
            df->connect(rr, sltEnd, sltEnd->getPortB());
            df->connect(sltEnd, muxEnd, muxEnd->getPortBranch());
            df->connect(mux_reduz.front(), muxEnd, muxEnd->getPortA());
            df->connect(reg1, muxEnd, muxEnd->getPortB());
            auto out = new OutputStream(outId);
            df->connect(muxEnd, out, out->getPortA());
        } else {
            auto out = new OutputStream(outId);
            df->connect(mux_reduz.front(), out, out->getPortA());
        }
    } else {
        auto in = new InputStream(idx++);
        auto out = new OutputStream(idx++);
        auto reg = new PassBi(idx, 0);
        df->connect(in, out, out->getPortB());
        df->connect(reg, out, out->getPortA());
    }
    return df;
}

void Kmeans::helpKmeansUpdateCentroids(unsigned short ***data_in, unsigned short **data_out, unsigned short **centroids,
                                       unsigned short **centroids_old, int data_size, int numThreads) {

    unsigned short k_sum[Kmeans::num_clusters * Kmeans::num_dim];
    unsigned short k_qtde[Kmeans::num_clusters];
    for (int t = 0; t < numThreads; ++t) {
        memset(k_sum, 0, sizeof(int) * (Kmeans::num_clusters * Kmeans::num_dim));
        memset(k_qtde, 0, sizeof(int) * (Kmeans::num_clusters));

        for (int i = 0; i < data_size; i++) {
            for (int j = 0; j < Kmeans::num_dim; j++) {
                k_sum[data_out[t][i] * Kmeans::num_dim + j] += data_in[t][j][i];
            }
            k_qtde[data_out[t][i]] += 1;
        }
        for (int c = 0; c < Kmeans::num_clusters; c++) {
            for (int j = 0; j < Kmeans::num_dim; j++) {
                if (k_qtde[c] > 0) {
                    centroids[t][c * Kmeans::num_dim + j] = k_sum[c * Kmeans::num_dim + j] / k_qtde[c];
                }
                centroids_old[t][c * Kmeans::num_dim + j] = centroids[t][c * Kmeans::num_dim + j];
            }
        }
    }
}

bool Kmeans::compare(Operator *a, Operator *b) {
    return a->getId() < b->getId();
}

void Kmeans::helpKmeansUpdateConstants(cgra_program_t *cgra_program, int thread, unsigned short *constants) {
    for (int i = 0; i < cgra_program->initial_conf.size(); ++i) {
        if (cgra_program->initial_conf[i].pe_constant_conf.conf_type == CGRA_CONF_SET_PE_CONST) {
            if (thread == cgra_program->initial_conf[i].pe_constant_conf.thread_id) {
                int pe_id = (int) cgra_program->initial_conf[i].pe_constant_conf.pe_id;
                std::pair<int, int> key(thread, pe_id);
                if (cgra_program->map_pe_to_op.find(key) != cgra_program->map_pe_to_op.end()) {
                    int op_id = cgra_program->map_pe_to_op[key];
                    if (op_id < Kmeans::num_clusters * Kmeans::num_dim) {
                        cgra_program->initial_conf[i].pe_constant_conf.constant = (unsigned long) constants[op_id];
                    }
                }
            }
        }
    }
}

void Kmeans::runCGRA(unsigned short ***data_in, unsigned short **data_out, unsigned short **centroids, int data_size,
                     int numThread) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    Kmeans::cgraHw->loadCgraProgram("../cgra_bitstreams/kmeans.cgra");
    for (int m = 0; m < numThread; ++m) {
        Kmeans::helpKmeansUpdateConstants(Kmeans::cgraHw->getCgraProgram(), m, centroids[m]);
        for (int i = 0; i < num_dim; ++i) {
            Kmeans::cgraHw->setCgraProgramInputStreamByID(m, (Kmeans::num_clusters * Kmeans::num_dim) + i,
                                                          data_in[m][i],
                                                          sizeof(short) * data_size);
        }
        Kmeans::cgraHw->setCgraProgramOutputStreamByID(m, (Kmeans::num_clusters * Kmeans::num_dim) + Kmeans::num_dim,
                                                       data_out[m],
                                                       sizeof(short) * data_size);
    }
    s = high_resolution_clock::now();
    Kmeans::cgraHw->syncExecute(0);
    diff = high_resolution_clock::now() - s;
    Kmeans::cgraExecTime = diff.count() * 1000;
}

void Kmeans::runCPU(unsigned short ***data_in, unsigned short **data_out, unsigned short **centroids, int data_size,
                    int numThread) {

    high_resolution_clock::time_point s;
    duration<double> diff = {};
    s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
    for (int t = 0; t < NUM_THREADS; ++t) {
        for (int i = 0; i < data_size; i++) {
            unsigned short min = UINT16_MAX, min_id = 0;
            for (unsigned short c = 0; c < Kmeans::num_clusters; c++) {
                unsigned short sum = 0;
                for (int j = 0; j < Kmeans::num_dim; j++) {
                    sum += data_in[t][j][i] - centroids[t][c * Kmeans::num_dim + j];
                }
                if (sum <= min) {
                    min = sum;
                    min_id = c;
                }
            }
            data_out[t][i] = min_id;
        }
    }
    diff = high_resolution_clock::now() - s;
    Kmeans::cpuExecTime = diff.count() * 1000;

}

bool Kmeans::compile(int numThreads) {

    Scheduler scheduler(Kmeans::cgraArch);
    high_resolution_clock::time_point s;
    duration<double> diff = {};
    char filename[100];
    std::vector<DataFlow *> dfs;

    for (int i = 0; i < numThreads; ++i) {
        dfs.push_back(Kmeans::createDataFlow(i));
        scheduler.addDataFlow(dfs[i], i, 0);
        Kmeans::cgraArch->getNetBranch(i)->createRouteTable();
        Kmeans::cgraArch->getNet(i)->createRouteTable();
    }

    int r = 0;
    for (int j = 0; j < 100; ++j) {
        s = high_resolution_clock::now();
        r = scheduler.scheduling();
        diff = high_resolution_clock::now() - s;
        if(r == SCHEDULE_SUCCESS )
            break;
    }

    Kmeans::schedulingTime = diff.count() * 1000;

    if (r == SCHEDULE_SUCCESS) {
        sprintf(filename, "../cgra_bitstreams/%s.cgra", dfs[0]->getName().c_str());
        Kmeans::cgraArch->writeCgraProgram(filename);
    } else {
        printf("Error on scheduling: Code %d\n", r);
    }
    for (auto df:dfs) {
        delete df;
    }
    return r == SCHEDULE_SUCCESS;
}

void Kmeans::printStatistics() {

    auto df = Kmeans::createDataFlow(0);
    MSG("INFO Kmeans Statictics");
    MSG("INFO CGRA target architecture:");
    MSG("INFO Number of Threads: " << Kmeans::cgraArch->getNumThreads());
    MSG("INFO Number of PEs: " << Kmeans::cgraArch->getNumPe());
    MSG("INFO Number of input PEs: " << Kmeans::cgraArch->getNumPeIn());
    MSG("INFO Number of output PEs: " << Kmeans::cgraArch->getNumPeOut());
    MSG("INFO Omega net PEs size: " << Kmeans::cgraArch->getNet(0)->getSize());
    MSG("INFO Omega net PEs radix: " << Kmeans::cgraArch->getNet(0)->getRadix());
    MSG("INFO Omega net branch size: " << Kmeans::cgraArch->getNetBranch(0)->getSize());
    MSG("INFO Omega net branch radix: " << Kmeans::cgraArch->getNetBranch(0)->getRadix());
    MSG("INFO Word size(bits): " << Kmeans::cgraArch->getWordSize() * 8);
    MSG("INFO DataFlow:");
    MSG("INFO Num input nodes: " << df->getNumOpIn());
    MSG("INFO Num output nodes: " << df->getNumOpOut());
    MSG("INFO Num total nodes: " << df->getNumOp());
    MSG("INFO Num total edges: " << df->getNumEdges());
    MSG("INFO Scheduling time: " << Kmeans::schedulingTime << "ms");
    MSG("INFO CGRA total execution time: " << Kmeans::cgraExecTime << "ms");
    MSG("INFO CGRA execution time: " << Kmeans::cgraHw->getTimeExec() << "ms");
    MSG("INFO CPU execution time: " << Kmeans::cpuExecTime << "ms");
    Kmeans::cgraHw->getAccManagement()->getAccelerator(0).printHwInfo();
    Kmeans::cgraHw->getAccManagement()->printHwInfo();
    delete df;
}

void Kmeans::benchmarking(int numThreads, int data_size) {

    unsigned short ***data_in;
    unsigned short **data_out_cpu;
    unsigned short **data_out_cgra;
    unsigned short **centroids_cpu;
    unsigned short **centroids_cgra;

    data_in = new unsigned short **[numThreads];
    data_out_cpu = new unsigned short *[numThreads];
    data_out_cgra = new unsigned short *[numThreads];
    centroids_cpu = new unsigned short *[numThreads];
    centroids_cgra = new unsigned short *[numThreads];

    for (int i = 0; i < numThreads; ++i) {
        data_in[i] = new unsigned short *[Kmeans::num_dim];
        data_out_cpu[i] = new unsigned short[data_size];
        data_out_cgra[i] = new unsigned short[data_size];
        centroids_cpu[i] = new unsigned short[Kmeans::num_dim * Kmeans::num_clusters];
        centroids_cgra[i] = new unsigned short[Kmeans::num_dim * Kmeans::num_clusters];
        for (int j = 0; j < Kmeans::num_dim; ++j) {
            data_in[i][j] = new unsigned short[data_size];
        }
    }
    for (int t = 0; t < numThreads; ++t) {
        for (int i = 0; i < Kmeans::num_dim * Kmeans::num_clusters; ++i) {
            centroids_cpu[t][i] = (unsigned short) (random() % 64);
            centroids_cgra[t][i] = centroids_cpu[t][i];
        }
        for (int j = 0; j < Kmeans::num_dim; ++j) {
            for (int i = 0; i < data_size; ++i) {
                data_in[t][j][i] = (unsigned short) (random() % 64);
            }
        }
        for (int k = 0; k < data_size; ++k) {
            data_out_cpu[t][k] = 0;
            data_out_cgra[t][k] = 0;
        }
    }

    if (Kmeans::compile(numThreads)) {
        Kmeans::runCGRA(data_in, data_out_cgra, centroids_cgra, data_size, numThreads);
        Kmeans::runCPU(data_in, data_out_cpu, centroids_cpu, data_size, numThreads);
        for (int k = 0; k < numThreads; ++k) {
            for (int j = 0; j < data_size; ++j) {
                if (data_out_cpu[k][j] != data_out_cgra[k][j]) {
                    printf("Error: Thread %d, index %d, expected %d found %d!\n", k, j, data_out_cpu[k][j],
                           data_out_cgra[k][j]);
                    break;
                }
            }
        }
    } else {
        printf("Compilation failed!\n");
    }
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < Kmeans::num_dim; ++j) {
            delete data_in[i][j];
        }
        delete data_in[i];
        delete data_out_cpu[i];
        delete data_out_cgra[i];
        delete centroids_cpu[i];
        delete centroids_cgra[i];
    }
    delete data_in;
    delete data_out_cpu;
    delete data_out_cgra;
    delete centroids_cpu;
    delete centroids_cgra;
}

