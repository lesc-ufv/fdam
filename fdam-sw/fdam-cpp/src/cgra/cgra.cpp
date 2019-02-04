#include <fdam/cgra/cgra.h>

Cgra::Cgra() {
    Cgra::cgra_program = nullptr;
    Cgra::accManagement = new AccManagement();
}

Cgra::~Cgra() {
    delete Cgra::accManagement;
    Cgra::freeProgram(Cgra::cgra_program);
}

int Cgra::intlog(double x, double base) {
    return (int) std::ceil(log(x) / log(base));
}

bool Cgra::loadCgraProgram(cgra_program_t cp) {
    Cgra::cgra_program = Cgra::makeCopy(std::move(cp));
    return true;
}

bool Cgra::loadCgraProgram(string filePath) {
    return readProgramFile(std::move(filePath));
}

bool Cgra::setCgraProgramInputStreamByID(int dataFlowID, int inputStreamID, const void *inputStreamData, size_t size) {
    int queue_id;
    for (const auto &it:Cgra::cgra_program->input_map) {
        if (dataFlowID == std::get<1>(it.first) && inputStreamID == std::get<2>(it.first)) {
            queue_id = it.second;
            if (queue_id < Cgra::cgra_program->num_pe_io_in) {
                Cgra::input_queue[queue_id][std::get<0>(it.first)] = std::pair<unsigned char *, size_t>(
                        (unsigned char *) inputStreamData, size);
                return true;
            }
        }
    }
    return false;
}

bool
Cgra::setCgraProgramInputStreamByName(const std::string dataFlowName, int inputStreamID, const void *inputStreamData,
                                      size_t size) {
    int queue_id;
    for (const auto &it:Cgra::cgra_program->input_map) {
        if (dataFlowName == std::get<3>(it.first) && inputStreamID == std::get<2>(it.first)) {
            queue_id = it.second;
            if (queue_id < Cgra::cgra_program->num_pe_io_in) {
                Cgra::input_queue[queue_id][std::get<0>(it.first)] = std::pair<unsigned char *, size_t>(
                        (unsigned char *) inputStreamData, size);
                return true;
            }
        }
    }

    return false;
}

bool Cgra::setCgraProgramOutputStreamByID(int dataFlowID, int outputStreamID, void *outputStreamData, size_t size) {

    int queue_id;
    for (const auto &it:Cgra::cgra_program->output_map) {
        if (dataFlowID == std::get<1>(it.first) && outputStreamID == std::get<2>(it.first)) {
            queue_id = it.second;
            if (queue_id < Cgra::cgra_program->num_pe_io_out) {
                Cgra::output_queue[queue_id][std::get<0>(it.first)] = std::pair<unsigned char *, size_t>(
                        (unsigned char *) outputStreamData, size);
                return true;
            }
        }
    }
    return false;
}

bool
Cgra::setCgraProgramOutputStreamByName(const std::string dataFlowName, int outputStreamID, const void *outputStreamData,
                                       size_t size) {
    int queue_id;
    for (const auto &it:Cgra::cgra_program->output_map) {
        if (dataFlowName == std::get<3>(it.first) && outputStreamID == std::get<2>(it.first)) {
            queue_id = it.second;
            if (queue_id < Cgra::cgra_program->num_pe_io_out) {
                Cgra::output_queue[queue_id][std::get<0>(it.first)] = std::pair<unsigned char *, size_t>(
                        (unsigned char *) outputStreamData, size);
                return true;
            }
        }
    }

    return false;
}

bool Cgra::readProgramFile(std::string filePath) {

    unsigned short input_map_size;
    unsigned short output_map_size;
    unsigned short map_pe_to_op_size;
    std::ifstream myfile;
    Cgra::cgra_program = new cgra_program_t;
    int threadID;
    int id_df;
    int id_op;
    int id_in;
    int id_pe;
    int size;
    myfile.open(filePath);
    if (myfile.is_open()) {
        myfile.read((char *) &Cgra::cgra_program->cgra_id, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program->num_pe, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program->num_pe_io_in, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program->num_pe_io_out, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program->net_radix, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program->num_extra_stage, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program->word_size, sizeof(unsigned short));
        myfile.read((char *) &input_map_size, sizeof(unsigned short));
        myfile.read((char *) &output_map_size, sizeof(unsigned short));
        myfile.read((char *) &map_pe_to_op_size, sizeof(unsigned short));
        
        myfile.read((char *) &Cgra::cgra_program->cgra_intial_conf, sizeof(cgra_intial_conf_t));
        Cgra::cgra_program->initial_conf.reserve(Cgra::cgra_program->cgra_intial_conf.qtd_conf);
        for (int i = 0; i < Cgra::cgra_program->cgra_intial_conf.qtd_conf; ++i) {
            initial_conf_t initial_conf;
            myfile.read((char *) &initial_conf.pad, sizeof(initial_conf_t));
            Cgra::cgra_program->initial_conf.push_back(initial_conf);
        }
        for (int i = 0; i < input_map_size; ++i) {
            myfile.read((char *) &size, sizeof(int));
            myfile.read((char *) &id_in, sizeof(int));
            myfile.read((char *) &threadID, sizeof(int));
            myfile.read((char *) &id_df, sizeof(int));
            myfile.read((char *) &id_op, sizeof(int));
            char name[size];
            myfile.read(name, size * sizeof(char));
            std::string s(name, static_cast<unsigned long>(size));
            Cgra::cgra_program->input_map[std::tuple<int, int, int, std::string>(threadID, id_df, id_op, s)] = id_in;
        }
        for (int i = 0; i < output_map_size; ++i) {
            myfile.read((char *) &size, sizeof(int));
            myfile.read((char *) &id_in, sizeof(int));
            myfile.read((char *) &threadID, sizeof(int));
            myfile.read((char *) &id_df, sizeof(int));
            myfile.read((char *) &id_op, sizeof(int));
            char name[size];
            myfile.read(name, size * sizeof(char));
            std::string s(name, static_cast<unsigned long>(size));
            Cgra::cgra_program->output_map[std::tuple<int, int, int, std::string>(threadID, id_df, id_op, s)] = id_in;
        }
        for (int i = 0; i < map_pe_to_op_size; ++i) {
            myfile.read((char *) &threadID, sizeof(int));
            myfile.read((char *) &id_pe, sizeof(int)); 
            myfile.read((char *) &id_op, sizeof(int));
            Cgra::cgra_program->map_pe_to_op[std::pair<int, int>(threadID,id_pe)] = id_op;
        }
        myfile.close();
    } else {
        printf("Failed open file!\n");
        return false;
    }
    return true;
}

void Cgra::syncExecute(long waitTime) {
    auto cp = makeCopy(*Cgra::cgra_program);
    std::map<int, int> pe_in_map;
    std::map<int, int> pe_out_map;
    std::map<int, int> in_pe_map;
    std::map<int, int> out_pe_map;
    std::map<int, unsigned long> ignore_in[cp->num_pe_io_in];
    std::map<int, unsigned long> ignore_out[cp->num_pe_io_out];
    unsigned long ignore_in_min = INT64_MAX;
    unsigned long ignore_out_min = INT64_MAX;
    size_t total_size_in[cp->num_pe_io_in];
    size_t total_size_out[cp->num_pe_io_out];
    auto pe_list = Cgra::makeListPe(cp->num_pe, cp->num_pe_io_in, cp->num_pe_io_out);
    auto flag_used_in_0 = false;

    for (int j = 0; j < cp->num_pe_io_in; ++j) {
        total_size_in[j] = 0;
    }
    for (int j = 0; j < cp->num_pe_io_out; ++j) {
        total_size_out[j] = 0;
    }

    int inId = 0;
    int outId = 0;
    for (auto p:pe_list) {
        if (p.second == PE_IN) {
            pe_in_map[p.first] = inId;
            in_pe_map[inId] = p.first;
            inId++;

        } else if (p.second == PE_OUT) {
            pe_out_map[p.first] = outId;
            out_pe_map[outId] = p.first;
            outId++;
        }
    }

    for (const auto &it:Cgra::input_queue) {
        for (auto it2:it.second) {
            initial_conf_t conf1, conf2;
            qtd_conf_t qtd;
            qtd.qtd = (it2.second.second / cp->word_size);

            conf1.pe_qtd_data_read_low_conf.conf_type = CGRA_CONF_SET_PE_QTD_LOW;
            conf1.pe_qtd_data_read_low_conf.pe_id = (unsigned long) in_pe_map[it.first] + 1;
            conf1.pe_qtd_data_read_low_conf.thread_id = (unsigned long) it2.first;
            conf1.pe_qtd_data_read_low_conf.qtd = qtd.qtd_low;

            conf2.pe_qtd_data_read_high_conf.conf_type = CGRA_CONF_SET_PE_QTD_HIGH;
            conf2.pe_qtd_data_read_high_conf.pe_id = (unsigned long) in_pe_map[it.first] + 1;
            conf2.pe_qtd_data_read_high_conf.thread_id = (unsigned long) it2.first;
            conf2.pe_qtd_data_read_high_conf.qtd = qtd.qtd_high;

            cp->initial_conf.push_back(conf1);
            cp->initial_conf.push_back(conf2);

            total_size_in[it.first] += it2.second.second;
        }
    }

    for (const auto &it:Cgra::output_queue) {
        for (auto it2:it.second) {
            initial_conf_t conf1, conf2;
            qtd_conf_t qtd;
            qtd.qtd = (it2.second.second / cp->word_size);
            conf1.pe_qtd_data_read_low_conf.conf_type = CGRA_CONF_SET_PE_QTD_LOW;
            conf1.pe_qtd_data_read_low_conf.pe_id = (unsigned long) out_pe_map[it.first] + 1;
            conf1.pe_qtd_data_read_low_conf.thread_id = (unsigned long) it2.first;
            conf1.pe_qtd_data_read_low_conf.qtd = qtd.qtd_low;

            conf2.pe_qtd_data_read_high_conf.conf_type = CGRA_CONF_SET_PE_QTD_HIGH;
            conf2.pe_qtd_data_read_high_conf.pe_id = (unsigned long) out_pe_map[it.first] + 1;
            conf2.pe_qtd_data_read_high_conf.thread_id = (unsigned long) it2.first;
            conf2.pe_qtd_data_read_high_conf.qtd = qtd.qtd_high;

            cp->initial_conf.push_back(conf1);
            cp->initial_conf.push_back(conf2);

            total_size_out[it.first] += it2.second.second;
        }
    }
    cp->cgra_intial_conf.qtd_conf = static_cast<unsigned int>(cp->initial_conf.size());
    size_t cgra_intial_conf_bytes = sizeof(cgra_intial_conf_t);
    size_t conf_bytes = cp->cgra_intial_conf.qtd_conf * sizeof(initial_conf_t);

    auto cgra_intial_conf_bytes_align = static_cast<size_t >((std::ceil(cgra_intial_conf_bytes / 64.0)) * 64.0);
    auto conf_bytes_align = static_cast<size_t>((std::ceil(conf_bytes / 64.0)) * 64.0);

    total_size_in[0] += cgra_intial_conf_bytes_align + conf_bytes_align;

    Accelerator &acc = Cgra::accManagement->getAccelerator((cp->cgra_id));

    for (const auto &it:Cgra::input_queue) {
        if (total_size_in[it.first] > 0) {
            acc.createInputQueue(static_cast<unsigned char>(it.first),
                                 static_cast<long long int>(total_size_in[it.first]));
            if (static_cast<unsigned char>(it.first) == 0) {
                flag_used_in_0 = true;
            }
        }
    }
    for (const auto &it:Cgra::output_queue) {
        if (total_size_out[it.first] > 0) {
            acc.createOutputQueue(static_cast<unsigned char>(it.first),
                                  static_cast<long long int>(total_size_out[it.first]));
        }
    }

    if (!flag_used_in_0) {
        acc.createInputQueue(0, static_cast<long long int>(total_size_in[0]));
    }

    auto queue_data_ptr = (unsigned char *) acc.getInputQueue(0);
    std::memcpy(queue_data_ptr, &cp->cgra_intial_conf, cgra_intial_conf_bytes);
    queue_data_ptr = queue_data_ptr + cgra_intial_conf_bytes_align;

    auto init_conf_ptr = (unsigned long *) queue_data_ptr;
    for (int k = 0; k < cp->cgra_intial_conf.qtd_conf; ++k) {
        init_conf_ptr[k] = cp->initial_conf[k].pad;
        unsigned long pe_id = cp->initial_conf[k].pe_store_ignore_conf.pe_id - 1;
        unsigned long threadID = cp->initial_conf[k].pe_store_ignore_conf.thread_id;
        unsigned long ignore = cp->initial_conf[k].pe_store_ignore_conf.store_ignore;
        inId = pe_in_map[pe_id];
        outId = pe_out_map[pe_id];
        if (cp->initial_conf[k].pe_store_ignore_conf.conf_type == CGRA_CONF_SET_PE_IGNORE) {
            if (pe_list[pe_id] == PE_IN) {
                ignore_in[inId][threadID] = ignore;
                if (ignore < ignore_in_min) {
                    ignore_in_min = ignore;
                }
            } else if (pe_list[pe_id] == PE_OUT) {
                ignore_out[outId][threadID] = ignore;
                if (ignore < ignore_out_min) {
                    ignore_out_min = ignore;
                }
            }
        }
    }
    for (int j = 0; j < cp->num_pe_io_in; ++j) {
        for (auto i : ignore_in[j]) {
            if (i.second > 0)
                i.second -= ignore_in_min;
        }
    }
    for (int j = 0; j < cp->num_pe_io_out; ++j) {
        for (auto i : ignore_out[j]) {
            if (i.second > 0)
                i.second -= ignore_out_min;
        }
    }
    queue_data_ptr = queue_data_ptr + conf_bytes_align;
    total_size_in[0] -= (conf_bytes_align + cgra_intial_conf_bytes_align);
    int ws = cp->word_size;
    for (const auto &it:Cgra::input_queue) {
        auto queue_ptr = (unsigned char *) acc.getInputQueue((unsigned char) it.first);
        auto len = total_size_in[it.first];
        std::map<int, size_t> count;
        if (it.first == 0) {
            queue_ptr = queue_data_ptr;
        }
        for (auto i:it.second) {
            count[i.first] = 0;
        }
        for (int k = 0; k < len;) {
            for (auto it2:it.second) {
                auto ptr_in = it2.second.first;
                if (ignore_in[it.first][it2.first] == 0) {
                    for (int j = 0; j < ws; ++j) {
                        if (count[it2.first] < it2.second.second) {
                            queue_ptr[k] = ptr_in[count[it2.first]];
                            count[it2.first]++;
                            k++;
                        }
                    }
                } else {
                    ignore_in[it.first][it2.first]--;
                }
            }
        }
    }

    acc.start();
    acc.waitDone(waitTime);
    acc.reset();

    for (const auto &it:Cgra::output_queue) {
        auto queue_ptr = (unsigned char *) acc.getOutputQueue((unsigned char) it.first);
        auto len = total_size_out[it.first];
        std::map<int, size_t> count;
        for (auto i:it.second) {
            count[i.first] = 0;
        }
        for (int k = 0; k < len;) {
            for (auto it2:it.second) {
                auto ptr_out = it2.second.first;
                if (ignore_out[it.first][it2.first] == 0) {
                    for (int j = 0; j < ws; ++j) {
                        if (count[it2.first] < it2.second.second) {
                            ptr_out[count[it2.first]] = queue_ptr[k];
                            count[it2.first]++;
                            k++;
                        }
                    }
                } else {
                    ignore_out[it.first][it2.first]--;
                }
            }
        }
    }
    freeProgram(cp);
}

std::map<int, int> Cgra::makeListPe(int num_pe, int num_pe_in, int num_pe_out) {

    std::map<int, int> pelist;
    for (int j = 0; j < num_pe_in; ++j) {
        pelist[j] = PE_IN;
    }
    for (int j = num_pe_in; j < num_pe - num_pe_out; ++j) {
        pelist[j] = PE_BASIC;
    }
    for (int j = num_pe - num_pe_out; j < num_pe; ++j) {
        pelist[j] = PE_OUT;
    }
    /*
    srand(3);
    for (int k = 0; k < num_pe_in; ++k) {
        bool repeat = true;
        while (repeat) {
            int pe = (int)random() % num_pe;
            if(pelist[pe] == PE_BASIC){
                pelist[pe] = PE_IN;
                repeat = false;
            }
        }
    }
    for (int l = 0; l < num_pe_out; ++l) {
        bool repeat = true;
        while (repeat) {
            int pe = (int)random() % num_pe;
            if(pelist[pe] == PE_BASIC){
                pelist[pe] = PE_OUT;
                repeat = false;
            }
        }
    }
     */
    return pelist;
}

cgra_program_t *Cgra::getCgraProgram() {
    return Cgra::cgra_program;
}

AccManagement *Cgra::getAccManagement() {
    return Cgra::accManagement;
}

cgra_program_t *Cgra::makeCopy(cgra_program_t cp) {

    auto cgra_program = new cgra_program_t;
    cgra_program->cgra_id = cp.cgra_id;
    cgra_program->num_pe = cp.num_pe;
    cgra_program->num_pe_io_in = cp.num_pe_io_in;
    cgra_program->num_pe_io_out = cp.num_pe_io_out;
    cgra_program->net_radix = cp.net_radix;
    cgra_program->num_extra_stage = cp.num_extra_stage;
    cgra_program->word_size = cp.word_size;
    cgra_program->cgra_intial_conf = cp.cgra_intial_conf;
    for (auto c:cp.initial_conf) {
        cgra_program->initial_conf.push_back(c);
    }
    for (const auto &c:cp.input_map) {
        cgra_program->input_map.insert(c);
    }
    for (const auto &c:cp.output_map) {
        cgra_program->output_map.insert(c);
    }
    for (const auto &c:cp.map_pe_to_op) {
        cgra_program->map_pe_to_op.insert(c);
    }
    return cgra_program;
}

void Cgra::freeProgram(cgra_program_t *cp) {
    if (cp != nullptr) {
        cp->initial_conf.clear();
        cp->input_map.clear();
        cp->output_map.clear();
        delete cp;
    }
}
