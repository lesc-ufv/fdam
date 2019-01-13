#include <fdam/cgra/cgra_arch.h>

CgraArch::CgraArch(int id, int num_pe, int num_pe_in, int num_pe_out, int net_radix, int num_extra_stage, int word_size)
        : id(id), num_pe(num_pe),
          num_pe_in(num_pe_in),
          num_pe_out(num_pe_out),
          net_radix(net_radix),
          num_extra_stage(num_extra_stage),
          word_size(word_size) {
    int num_stages1 = CgraArch::intLog(num_pe * 2, net_radix) + num_extra_stage;
    int num_stages2 = CgraArch::intLog(num_pe, net_radix);
    int stage_extra = num_stages1 - num_stages2;
    int numThreads = CgraArch::getNumThreads();
    CgraArch::cgra_program = {};
    for (int j = 0; j < numThreads; ++j) {
        CgraArch::netThreads.push_back(new Omega(num_pe * 2, net_radix, num_extra_stage));
        CgraArch::net_branchThreads.push_back(new Omega(num_pe, net_radix, stage_extra));
    }
    for (int i = 0; i < num_pe_in; ++i) {
        CgraArch::pe_array.push_back(new PEArch(i, PE_IN, numThreads));
    }
    for (int i = num_pe_in; i < num_pe_out + num_pe_in; ++i) {
        CgraArch::pe_array.push_back(new PEArch(i, PE_OUT, numThreads));
    }
    for (int i = num_pe_out + num_pe_in; i < num_pe; ++i) {
        CgraArch::pe_array.push_back(new PEArch(i, PE_BASIC, numThreads));
    }
}

CgraArch::~CgraArch() {
    int numThreads = CgraArch::getNumThreads();
    for (auto it:CgraArch::pe_array) {
        delete it;
    }
    for (int i = 0; i < numThreads; ++i) {
        delete CgraArch::netThreads[i];
        delete CgraArch::net_branchThreads[i];
    }
    CgraArch::pe_array.clear();
    CgraArch::cgra_program.initial_conf.clear();
    CgraArch::cgra_program.input_map.clear();
    CgraArch::cgra_program.output_map.clear();
    CgraArch::netThreads.clear();
    CgraArch::net_branchThreads.clear();
    CgraArch::dataFlows.clear();
}

int CgraArch::getId() const {
    return id;
}

void CgraArch::setId(int id) {
    CgraArch::id = id;
}

int CgraArch::getNumPe() const {
    return num_pe;
}

void CgraArch::setNumPe(int num_pe) {
    CgraArch::num_pe = num_pe;
}

int CgraArch::getNumPeIn() const {
    return num_pe_in;
}

void CgraArch::setNumPeIn(int num_pe_in) {
    CgraArch::num_pe_in = num_pe_in;
}

int CgraArch::getNumPeOut() const {
    return num_pe_out;
}

void CgraArch::setNumPeOut(int num_pe_out) {
    CgraArch::num_pe_out = num_pe_out;
}

int CgraArch::getNetRadix() const {
    return net_radix;
}

void CgraArch::setNetRadix(int net_radix) {
    CgraArch::net_radix = net_radix;
}

PEArch *CgraArch::getPE(int id) {
    if (id >= 0 && CgraArch::num_pe) {
        return CgraArch::pe_array[id];
    }
    return nullptr;
}

Omega *CgraArch::getNet(int threadID) const {
    return CgraArch::netThreads[threadID];
}

Omega *CgraArch::getNetBranch(int threadID) const {
    return CgraArch::net_branchThreads[threadID];
}

void CgraArch::reset() {
    int numThreads = CgraArch::getNumThreads();
    for (int i = 0; i < numThreads; ++i) {
        CgraArch::netThreads[i]->reset();
        CgraArch::net_branchThreads[i]->reset();
        CgraArch::dataFlows.clear();
        for (auto pe:CgraArch::pe_array) {
            pe->setOperator(nullptr, i);
            pe->setOut(-1, i);
        }
    }
}

void CgraArch::reset(int threadID) {


    CgraArch::netThreads[threadID]->reset();
    CgraArch::net_branchThreads[threadID]->reset();
    CgraArch::dataFlows.erase(threadID);
    for (auto pe:CgraArch::pe_array) {
        pe->setOperator(nullptr, threadID);
        pe->setOut(-1, threadID);
    }
}


void CgraArch::writeCgraProgram(const std::string &filePath) {

    std::vector<unsigned short> hearder;
    std::ofstream myfile;

    CgraArch::makeProgram();

    hearder.reserve(9);
    hearder.push_back(CgraArch::cgra_program.cgra_id);
    hearder.push_back(CgraArch::cgra_program.num_pe);
    hearder.push_back(CgraArch::cgra_program.num_pe_io_in);
    hearder.push_back(CgraArch::cgra_program.num_pe_io_out);
    hearder.push_back(CgraArch::cgra_program.net_radix);
    hearder.push_back(CgraArch::cgra_program.num_extra_stage);
    hearder.push_back(CgraArch::cgra_program.word_size);
    hearder.push_back(static_cast<unsigned short>(CgraArch::cgra_program.input_map.size()));
    hearder.push_back(static_cast<unsigned short>(CgraArch::cgra_program.output_map.size()));

    myfile.open(filePath);
    myfile.write((char *) &hearder[0], hearder.size() * sizeof(unsigned short));
    myfile.write((char *) &CgraArch::cgra_program.cgra_intial_conf, sizeof(cgra_intial_conf_t));
    for (auto &i : CgraArch::cgra_program.initial_conf) {
        myfile.write((char *) &i.pad, sizeof(initial_conf_t));
    }
    for (auto it:CgraArch::cgra_program.input_map) {
        int s = static_cast<int>(std::get<3>(it.first).size());
        myfile.write((char *) &s, sizeof(int));
        myfile.write((char *) &it.second, sizeof(int));
        myfile.write((char *) &std::get<0>(it.first), sizeof(int));
        myfile.write((char *) &std::get<1>(it.first), sizeof(int));
        myfile.write((char *) &std::get<2>(it.first), sizeof(int));
        myfile.write(std::get<3>(it.first).c_str(), s * sizeof(char));
    }
    for (auto it:CgraArch::cgra_program.output_map) {
        int s = static_cast<int>(std::get<3>(it.first).size());
        myfile.write((char *) &s, sizeof(int));
        myfile.write((char *) &it.second, sizeof(int));
        myfile.write((char *) &std::get<0>(it.first), sizeof(int));
        myfile.write((char *) &std::get<1>(it.first), sizeof(int));
        myfile.write((char *) &std::get<2>(it.first), sizeof(int));
        myfile.write(std::get<3>(it.first).c_str(), s * sizeof(char));
    }
    myfile.close();
}

void CgraArch::makeProgram() {

    int numThreads = CgraArch::getNumThreads();

    CgraArch::cgra_program.cgra_id = static_cast<unsigned short>(CgraArch::id);
    CgraArch::cgra_program.num_pe = static_cast<unsigned short>(CgraArch::num_pe);
    CgraArch::cgra_program.num_pe_io_in = static_cast<unsigned short>(CgraArch::num_pe_in);
    CgraArch::cgra_program.num_pe_io_out = static_cast<unsigned short>(CgraArch::num_pe_out);
    CgraArch::cgra_program.net_radix = static_cast<unsigned short>(CgraArch::net_radix);
    CgraArch::cgra_program.num_extra_stage = static_cast<unsigned short>(CgraArch::num_extra_stage);
    CgraArch::cgra_program.word_size = static_cast<unsigned short>(CgraArch::word_size);
    CgraArch::cgra_program.cgra_intial_conf.mask_input_fifo = 0;
    CgraArch::cgra_program.cgra_intial_conf.mask_output_fifo = 0;
    CgraArch::cgra_program.cgra_intial_conf.qtd_conf = 0;

    for (auto pe:CgraArch::pe_array) {
        if (pe->getType() == PE_IN || pe->getType() == PE_OUT) {
            for (int i = 0; i < numThreads; ++i) {
                initial_conf_t defaultConf;
                defaultConf.pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
                defaultConf.pe_instruction_conf.pe_id = (unsigned long) pe->getId() + 1;
                defaultConf.pe_instruction_conf.thread_id = (unsigned long) i;
                defaultConf.pe_instruction_conf.inst_addr = 0;
                defaultConf.pe_instruction_conf.alu_op = 0;
                defaultConf.pe_instruction_conf.control = 0;
                defaultConf.pe_instruction_conf.rf_raddr = 0;
                defaultConf.pe_instruction_conf.rf_waddr = 0;
                CgraArch::cgra_program.initial_conf.push_back(defaultConf);
            }
        }
    }

    for (int j = 0; j < numThreads; ++j) {
        auto sw_branch_array = CgraArch::net_branchThreads[j]->getConfArray();
        auto sw_array = CgraArch::netThreads[j]->getConfArray();

        for (auto sw:sw_branch_array) {
            if (sw.getUsed()) {
                CgraArch::cgra_program.initial_conf.push_back(sw.getSwConf(true, j, 0));
                CgraArch::cgra_program.initial_conf.push_back(sw.getSwPcMaxConf(true, j));
                CgraArch::cgra_program.initial_conf.push_back(sw.getSwLoopConf(true, j));
            }
        }

        for (auto sw:sw_array) {
            if (sw.getUsed()) {
                CgraArch::cgra_program.initial_conf.push_back(sw.getSwConf(false, j, 0));
                CgraArch::cgra_program.initial_conf.push_back(sw.getSwPcMaxConf(false, j));
                CgraArch::cgra_program.initial_conf.push_back(sw.getSwLoopConf(false, j));
            }
        }

        for (auto pe:CgraArch::pe_array) {
            Operator *op = pe->getOperator(j);
            if (op != nullptr) {
                DataFlow *df = CgraArch::dataFlows[j];
                CgraArch::cgra_program.initial_conf.push_back(pe->getConf(j));
                CgraArch::cgra_program.initial_conf.push_back(pe->getPcMaxConf(j));
                CgraArch::cgra_program.initial_conf.push_back(pe->getPcLoopConf(j));
                switch (op->getType()) {
                    case OP_IN:
                        CgraArch::cgra_program.initial_conf.push_back(pe->getLoadIgnoreConf(j));
                        CgraArch::cgra_program.cgra_intial_conf.mask_input_fifo =
                                CgraArch::cgra_program.cgra_intial_conf.mask_input_fifo | (1 << pe->getId());
                        CgraArch::cgra_program.input_map[std::tuple<int, int, int, std::string>(j, df->getId(), op->getId(),
                                                                             df->getName())] = pe->getId();
                        break;
                    case OP_OUT:
                        CgraArch::cgra_program.initial_conf.push_back(pe->getStoreIgnoreConf(j));
                        CgraArch::cgra_program.cgra_intial_conf.mask_output_fifo =
                                CgraArch::cgra_program.cgra_intial_conf.mask_output_fifo | (1 << (pe->getId() - CgraArch::num_pe_in));
                        CgraArch::cgra_program.output_map[std::tuple<int, int, int, std::string>(j, df->getId(), op->getId(),
                                                                              df->getName())] = (pe->getId() -
                                                                                                 CgraArch::num_pe_in);
                        break;
                    case OP_IMMEDIATE:
                        CgraArch::cgra_program.initial_conf.push_back(pe->getConstConf(j));
                        break;
                    default:
                        break;
                }
            }
        }
    }
    CgraArch::cgra_program.cgra_intial_conf.qtd_conf = static_cast<unsigned int>(CgraArch::cgra_program.initial_conf.size());
}


int CgraArch::getNumThreads() {

    int num_threads = CgraArch::intLog(CgraArch::num_pe * 2, CgraArch::net_radix) + 1 + 3;
    return num_threads;
}

int CgraArch::intLog(double x, double base) {
    return (int) std::ceil(log(x) / log(base));
}

void CgraArch::setDataFlow(DataFlow *df, int threadID) {
    CgraArch::dataFlows[threadID] = df;
}

DataFlow *CgraArch::getDataFlow(int threadID) {
    return CgraArch::dataFlows[threadID];
}

const std::vector<PEArch *> &CgraArch::getPeArray() const {
    return pe_array;
}

cgra_program_t CgraArch::getCgraProgram() {
    CgraArch::makeProgram();
    return cgra_program;
}
