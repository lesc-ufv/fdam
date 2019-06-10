#include "kmeans.h"

int main(int argc, char *argv[]) {

    int idx = 0;
    int test = 0;
     
    if(argc > 1)
        test = atoi(argv[1]);
     
    if(argc > 2)
        idx = atoi(argv[2]);

    if(test & 1)
        kmeans(idx, 5, 8);
    if(test & 2)
        kmeans_openmp(idx, 5, 8);
    if(test & 4)
        kmeans_cgra(idx, 5, 8);

    return 0;
}

int kmeans(int idx, int num_clusters, int num_dim) {

    unsigned short *data_in, *data_out, *centroids;

    data_in = new unsigned short[DATA_SIZE * num_dim];
    data_out = new unsigned short[DATA_SIZE];
    centroids = new unsigned short[num_clusters * num_dim];

    for (int i = 0; i < DATA_SIZE * num_dim; i++) {
        data_in[i] = i;
    }
    for (int i = 0; i < DATA_SIZE; i++) {
        data_out[i] = 0;
    }
    for (int i = 0; i < num_clusters * num_dim; i++) {
        centroids[i] = i;
    }

    high_resolution_clock::time_point s;
    duration<double> diff = {};

    for (int sp = 0; sp < SAMPLES; sp++) {
        s = high_resolution_clock::now();
        for (int i = 0; i < DATA_SIZE; i++) {
            unsigned short min = UINT16_MAX, min_id = 0;
            for (unsigned short c = 0; c < num_clusters; c++) {
                unsigned short sum = 0;
                for (int j = 0; j < num_dim; j++) {
                    sum += data_in[j * num_dim + i] - centroids[c * num_dim + j];
                }
                if (sum <= min) {
                    min = sum;
                    min_id = c;
                }
            }
            data_out[i] = min_id;
        }
        diff += high_resolution_clock::now() - s;
    }

    double cpuExecTime = (diff.count() * 1000) / SAMPLES;

    printf("Time(ms) CPU 1 Thread: %5.2lf\n", cpuExecTime);

    int v = data_out[idx];

    delete data_in;
    delete data_out;
    delete centroids;

    return v;
}

int kmeans_openmp(int idx, int num_clusters, int num_dim) {
    unsigned short *data_in, *data_out, *centroids;

    data_in = new unsigned short[DATA_SIZE * num_dim];
    data_out = new unsigned short[DATA_SIZE];
    centroids = new unsigned short[num_clusters * num_dim];

    for (int i = 0; i < DATA_SIZE * num_dim; i++) {
        data_in[i] = i;
    }
    for (int i = 0; i < DATA_SIZE; i++) {
        data_out[i] = 0;
    }
    for (int i = 0; i < num_clusters * num_dim; i++) {
        centroids[i] = i;
    }

    omp_set_num_threads(NUM_THREAD);

    high_resolution_clock::time_point s;
    duration<double> diff = {};

    for (int sp = 0; sp < SAMPLES; sp++) {
        s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for
        for (int i = 0; i < DATA_SIZE; i++) {
            unsigned short min = UINT16_MAX, min_id = 0;
            for (unsigned short c = 0; c < num_clusters; c++) {
                unsigned short sum = 0;
                for (int j = 0; j < num_dim; j++) {
                    sum += data_in[j * num_dim + i] - centroids[c * num_dim + j];
                }
                if (sum <= min) {
                    min = sum;
                    min_id = c;
                }
            }
            data_out[i] = min_id;
        }
        diff += high_resolution_clock::now() - s;
    }

    double cpuExecTime = (diff.count() * 1000) / SAMPLES;

    printf("Time(ms) CPU %d Thread: %5.2lf\n", NUM_THREAD, cpuExecTime);

    int v = data_out[idx];

    delete data_in;
    delete data_out;
    delete centroids;

    return v;
}

int kmeans_cgra(int idx, int num_clusters, int num_dim) {
    auto cgraArch = new CgraArch(0, 128, 8, 8, 4, 0, 2);
    auto cgraHw = new Cgra();
    Scheduler scheduler(cgraArch);
    std::vector<DataFlow *> dfs;
    unsigned short *data_in, *data_out, *centroids;
    int r = 0, v = 0,tries = 0;

    data_in = new unsigned short[DATA_SIZE * num_dim];
    data_out = new unsigned short[DATA_SIZE];
    centroids = new unsigned short[num_clusters * num_dim];

    for (int i = 0; i < DATA_SIZE * num_dim; i++) {
        data_in[i] = i;
    }
    for (int i = 0; i < DATA_SIZE; i++) {
        data_out[i] = 0;
    }
    for (int i = 0; i < num_clusters * num_dim; i++) {
        centroids[i] = i;
    }


    for (int i = 0; i < NUM_THREAD; ++i) {
        dfs.push_back(createDataFlow(i, num_clusters, num_dim));
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
        auto data_size = (size_t) (DATA_SIZE / ((NUM_THREAD)));
        auto sf = (sizeof(unsigned short));
        int k = 0;
        for (int m = 0; m < NUM_THREAD; ++m) {
            helpKmeansUpdateConstants(cgraHw->getCgraProgram(), m, centroids, num_clusters, num_dim);
            for (int i = 0; i < num_dim; ++i) {
                cgraHw->setCgraProgramInputStreamByID(m, (num_clusters * num_dim) + i,
                                                      &data_in[i * num_dim + (k * data_size)], sf * data_size);
            }
            cgraHw->setCgraProgramOutputStreamByID(m, (num_clusters * num_dim) + num_dim, &data_out[k * data_size],
                                                   sf * data_size);
            k++;
        }
        double cgraExecTime = 0;
        for (int i = 0; i < SAMPLES; i++){
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
    delete data_in;
    delete data_out;
    for (auto df:dfs) {
        delete df;
    }
    return v;
}

DataFlow *createDataFlow(int id, int num_clusters, int num_dim) {
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
    if (num_clusters > 1) {
        for (int j = 0; j < num_clusters; ++j) {
            for (int i = 0; i < num_dim; ++i) {
                auto s = new Subi(idx++, j * num_dim + i);
                subs[j].push_back(s);
            }
        }
        inputs.reserve(static_cast<unsigned long>(num_dim));
        for (int i = 0; i < num_dim; ++i) {
            inputs.push_back(new InputStream(idx++));
        }
        outId = idx++;
        for (int j = 0; j < num_clusters; ++j) {
            for (int i = 0; i < num_dim; ++i) {
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

        sort(aux.begin(), aux.end(), compare);

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
        if (num_clusters % 2 != 0) {
            auto rr = addEnd;
            for (int i = 0; i < reg; ++i) {
                auto r = new PassA(idx++);
                df->connect(rr, r, r->getPortA());
                rr = r;
            }
            auto sltEnd = new Slt(idx++);
            auto muxEnd = new Mux(idx++);
            auto reg1 = new PassBi(idx++, num_clusters - 1);

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

bool compare(Operator *a, Operator *b) {
    return a->getId() < b->getId();
}

void helpKmeansUpdateConstants(cgra_program_t *cgra_program, int thread, unsigned short *constants, int num_clusters,
                               int num_dim) {
    for (int i = 0; i < cgra_program->initial_conf.size(); ++i) {
        if (cgra_program->initial_conf[i].pe_constant_conf.conf_type == CGRA_CONF_SET_PE_CONST) {
            if (thread == cgra_program->initial_conf[i].pe_constant_conf.thread_id) {
                int pe_id = (int) cgra_program->initial_conf[i].pe_constant_conf.pe_id;
                std::pair<int, int> key(thread, pe_id);
                if (cgra_program->map_pe_to_op.find(key) != cgra_program->map_pe_to_op.end()) {
                    int op_id = cgra_program->map_pe_to_op[key];
                    if (op_id < num_clusters * num_dim) {
                        cgra_program->initial_conf[i].pe_constant_conf.constant = (unsigned long) constants[op_id];
                    }
                }
            }
        }
    }
}
