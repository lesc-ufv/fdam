#include <utility>

#include <utility>

#include <fdam/cgra/cgra.h>

Cgra::Cgra() {
    Cgra::cgra_program = {};
    Cgra::accManagement = new AccManagement();
}

Cgra::~Cgra() {

    Cgra::cgra_program.initial_conf.clear();
    Cgra::cgra_program.input_map.clear();
    Cgra::cgra_program.output_map.clear();
    delete Cgra::accManagement;
}

int Cgra::intlog(double x, double base) {
    return (int) std::ceil(log(x) / log(base));
}


bool Cgra::loadCgraProgram(cgra_program_t cp){
    Cgra::cgra_program = std::move(cp);
    return true;
}


bool Cgra::loadCgraProgramFromFile(string filePath) {
    return readProgramFile(std::move(filePath));
}

bool Cgra::setCgraProgramInputStreamByID(int dataFlowID, int inputStreamID, const void *inputStreamData, size_t size) {
    int queue_id;
    for (const auto &it:Cgra::cgra_program.input_map) {
        if (dataFlowID == std::get<1>(it.first) && inputStreamID == std::get<2>(it.first)) {
            queue_id = it.second;
            if (queue_id < Cgra::cgra_program.num_pe_io_in) {
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
    for (const auto &it:Cgra::cgra_program.input_map) {
        if (dataFlowName == std::get<3>(it.first) && inputStreamID == std::get<2>(it.first)) {
            queue_id = it.second;
            if (queue_id < Cgra::cgra_program.num_pe_io_in) {
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
    for (const auto &it:Cgra::cgra_program.output_map) {
        if (dataFlowID == std::get<1>(it.first) && outputStreamID == std::get<2>(it.first)) {
            queue_id = it.second;
            if (queue_id < Cgra::cgra_program.num_pe_io_out) {
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
    for (const auto &it:Cgra::cgra_program.output_map) {
        if (dataFlowName == std::get<3>(it.first) && outputStreamID == std::get<2>(it.first)) {
            queue_id = it.second;
            if (queue_id < Cgra::cgra_program.num_pe_io_out) {
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

    std::ifstream myfile;
    myfile.open(filePath);
    if (myfile.is_open()) {
        myfile.read((char *) &Cgra::cgra_program.cgra_id, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program.num_pe, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program.num_pe_io_in, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program.num_pe_io_out, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program.net_radix, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program.num_extra_stage, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program.word_size, sizeof(unsigned short));
        myfile.read((char *) &input_map_size, sizeof(unsigned short));
        myfile.read((char *) &output_map_size, sizeof(unsigned short));
        myfile.read((char *) &Cgra::cgra_program.cgra_intial_conf, sizeof(cgra_intial_conf_t));
        Cgra::cgra_program.initial_conf.reserve(Cgra::cgra_program.cgra_intial_conf.qtd_conf);
        for (int i = 0; i < Cgra::cgra_program.cgra_intial_conf.qtd_conf; ++i) {
            initial_conf_t initial_conf;
            myfile.read((char *) &initial_conf.pad, sizeof(initial_conf_t));
            Cgra::cgra_program.initial_conf.push_back(initial_conf);
        }
        for (int i = 0; i < input_map_size; ++i) {
            int threadID;
            int id_df;
            int id_op;
            int id_in;
            int size;
            myfile.read((char *) &size, sizeof(int));
            myfile.read((char *) &id_in, sizeof(int));
            myfile.read((char *) &threadID, sizeof(int));
            myfile.read((char *) &id_df, sizeof(int));
            myfile.read((char *) &id_op, sizeof(int));
            char name[size];
            myfile.read(name, size * sizeof(char));
            std::string s(name, static_cast<unsigned long>(size));
            Cgra::cgra_program.input_map[std::tuple<int, int, int, std::string>(threadID, id_df, id_op, s)] = id_in;
        }
        for (int i = 0; i < output_map_size; ++i) {
            int threadID;
            int id_df;
            int id_op;
            int id_in;
            int size;
            myfile.read((char *) &size, sizeof(int));
            myfile.read((char *) &id_in, sizeof(int));
            myfile.read((char *) &threadID, sizeof(int));
            myfile.read((char *) &id_df, sizeof(int));
            myfile.read((char *) &id_op, sizeof(int));
            char name[size];
            myfile.read(name, size * sizeof(char));
            std::string s(name, static_cast<unsigned long>(size));
            Cgra::cgra_program.output_map[std::tuple<int, int, int, std::string>(threadID, id_df, id_op, s)] = id_in;
        }
        myfile.close();
    } else {
        printf("Failed open file!\n");
        return false;
    }

    return true;
}

void Cgra::syncExecute(long waitTime) {

    std::map<int, unsigned long> ignore_in[Cgra::cgra_program.num_pe_io_in];
    std::map<int, unsigned long> ignore_out[Cgra::cgra_program.num_pe_io_out];

    unsigned long ignore_in_min = INT64_MAX;
    unsigned long ignore_out_min = INT64_MAX;

    size_t total_size_in[Cgra::cgra_program.num_pe_io_in];
    size_t total_size_out[Cgra::cgra_program.num_pe_io_out];

    for (int j = 0; j < Cgra::cgra_program.num_pe_io_in; ++j) {
        total_size_in[j] = 0;
    }
    for (int j = 0; j < Cgra::cgra_program.num_pe_io_out; ++j) {
        total_size_out[j] = 0;
    }

    for (const auto &it:Cgra::input_queue) {
        for (auto it2:it.second) {
            initial_conf_t conf1, conf2;
            qtd_conf_t qtd;
            qtd.qtd = (it2.second.second / Cgra::cgra_program.word_size);

            conf1.pe_qtd_data_read_low_conf.conf_type = CGRA_CONF_SET_PE_QTD_LOW;
            conf1.pe_qtd_data_read_low_conf.pe_id = (unsigned long) it.first + 1;
            conf1.pe_qtd_data_read_low_conf.thread_id = (unsigned long) it2.first;
            conf1.pe_qtd_data_read_low_conf.qtd = qtd.qtd_low;

            conf2.pe_qtd_data_read_high_conf.conf_type = CGRA_CONF_SET_PE_QTD_HIGH;
            conf2.pe_qtd_data_read_high_conf.pe_id = (unsigned long) it.first + 1;
            conf2.pe_qtd_data_read_high_conf.thread_id = (unsigned long) it2.first;
            conf2.pe_qtd_data_read_high_conf.qtd = qtd.qtd_high;

            Cgra::cgra_program.initial_conf.push_back(conf1);
            Cgra::cgra_program.initial_conf.push_back(conf2);

            total_size_in[it.first] += it2.second.second;
        }
    }

    for (const auto &it:Cgra::output_queue) {
        for (auto it2:it.second) {
            initial_conf_t conf1, conf2;
            qtd_conf_t qtd;
            qtd.qtd = (it2.second.second / Cgra::cgra_program.word_size);
            unsigned long pe_id = (unsigned long) it.first + Cgra::cgra_program.num_pe_io_in + 1;
            conf1.pe_qtd_data_read_low_conf.conf_type = CGRA_CONF_SET_PE_QTD_LOW;
            conf1.pe_qtd_data_read_low_conf.pe_id = pe_id;
            conf1.pe_qtd_data_read_low_conf.thread_id = (unsigned long) it2.first;
            conf1.pe_qtd_data_read_low_conf.qtd = qtd.qtd_low;

            conf2.pe_qtd_data_read_high_conf.conf_type = CGRA_CONF_SET_PE_QTD_HIGH;
            conf2.pe_qtd_data_read_high_conf.pe_id = pe_id;
            conf2.pe_qtd_data_read_high_conf.thread_id = (unsigned long) it2.first;
            conf2.pe_qtd_data_read_high_conf.qtd = qtd.qtd_high;

            Cgra::cgra_program.initial_conf.push_back(conf1);
            Cgra::cgra_program.initial_conf.push_back(conf2);

            total_size_out[it.first] += it2.second.second;
        }
    }
    Cgra::cgra_program.cgra_intial_conf.qtd_conf = static_cast<unsigned int>(Cgra::cgra_program.initial_conf.size());
    size_t cgra_intial_conf_bytes = sizeof(cgra_intial_conf_t);
    size_t conf_bytes = Cgra::cgra_program.cgra_intial_conf.qtd_conf * sizeof(initial_conf_t);

    auto cgra_intial_conf_bytes_align = static_cast<size_t >((std::ceil(cgra_intial_conf_bytes / 64.0)) * 64.0);
    auto conf_bytes_align = static_cast<size_t>((std::ceil(conf_bytes / 64.0)) * 64.0);

    total_size_in[0] += cgra_intial_conf_bytes_align + conf_bytes_align;

    Accelerator &acc = Cgra::accManagement->getAccelerator((Cgra::cgra_program.cgra_id));

    for (const auto &it:Cgra::input_queue) {
        if (total_size_in[it.first] > 0) {
            acc.createInputQueue(static_cast<unsigned char>(it.first),
                                 static_cast<long long int>(total_size_in[it.first]));
        }
    }
    for (const auto &it:Cgra::output_queue) {
        if (total_size_in[it.first] > 0) {
            acc.createOutputQueue(static_cast<unsigned char>(it.first),
                                  static_cast<long long int>(total_size_out[it.first]));
        }
    }

    auto queue_data_ptr = (unsigned char *) acc.getInputQueue(0);
    std::memcpy(queue_data_ptr, &Cgra::cgra_program.cgra_intial_conf, cgra_intial_conf_bytes);
    queue_data_ptr = queue_data_ptr + cgra_intial_conf_bytes_align;

    auto init_conf_ptr = (unsigned long *) queue_data_ptr;
    for (int k = 0; k < Cgra::cgra_program.cgra_intial_conf.qtd_conf; ++k) {
        init_conf_ptr[k] = Cgra::cgra_program.initial_conf[k].pad;
        unsigned long pe_id = Cgra::cgra_program.initial_conf[k].pe_store_ignore_conf.pe_id - 1;
        unsigned long threadID = Cgra::cgra_program.initial_conf[k].pe_store_ignore_conf.thread_id;
        unsigned long ignore = Cgra::cgra_program.initial_conf[k].pe_store_ignore_conf.store_ignore;

        if (Cgra::cgra_program.initial_conf[k].pe_store_ignore_conf.conf_type == CGRA_CONF_SET_PE_IGNORE) {
            if (pe_id < Cgra::cgra_program.num_pe_io_in) {
                ignore_in[pe_id][threadID] = ignore;
                if (ignore < ignore_in_min) {
                    ignore_in_min = ignore;
                }
            } else {
                ignore_out[pe_id - Cgra::cgra_program.num_pe_io_in][threadID] = ignore;
                if (ignore < ignore_out_min) {
                    ignore_out_min = ignore;
                }
            }
        }

    }
    for (int j = 0; j < Cgra::cgra_program.num_pe_io_in; ++j) {
        for (auto i : ignore_in[j]) {
            if (i.second > 0)
                i.second -= ignore_in_min;
        }

    }
    for (int j = 0; j < Cgra::cgra_program.num_pe_io_out; ++j) {
        for (auto i : ignore_out[j]) {
            if (i.second > 0)
                i.second -= ignore_out_min;
        }
    }
    queue_data_ptr = queue_data_ptr + conf_bytes_align;
    total_size_in[0] -= (conf_bytes_align + cgra_intial_conf_bytes_align);
    int ws = Cgra::cgra_program.word_size;
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
}
