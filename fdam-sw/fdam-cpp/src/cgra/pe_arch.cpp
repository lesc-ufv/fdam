#include <fdam/cgra/pe_arch.h>

PEArch::PEArch(int id, int type, int numThreads) : id(id), type(type), numThreads(numThreads) {

    PEArch::control_mat[CONTROL_PORT_OUT_A][CONTROL_PORT_IN_A][CONTROL_PORT_IN_B] = 0;
    PEArch::control_mat[CONTROL_PORT_OUT_A][CONTROL_PORT_IN_A][CONTROL_RF_IN] = 1;
    PEArch::control_mat[CONTROL_PORT_OUT_A][CONTROL_FIFO_IN][CONTROL_RF_IN] = 2;
    PEArch::control_mat[CONTROL_PORT_OUT_A][CONTROL_FIFO_IN][CONTROL_PORT_IN_B] = 3;
    PEArch::control_mat[CONTROL_PORT_OUT_B][CONTROL_PORT_IN_A][CONTROL_PORT_IN_B] = 4;
    PEArch::control_mat[CONTROL_PORT_OUT_B][CONTROL_PORT_IN_A][CONTROL_RF_IN] = 5;
    PEArch::control_mat[CONTROL_PORT_OUT_B][CONTROL_FIFO_IN][CONTROL_RF_IN] = 6;
    PEArch::control_mat[CONTROL_PORT_OUT_B][CONTROL_FIFO_IN][CONTROL_PORT_IN_B] = 7;
    PEArch::control_mat[CONTROL_RF_OUT][CONTROL_PORT_IN_A][CONTROL_PORT_IN_B] = 8;
    PEArch::control_mat[CONTROL_RF_OUT][CONTROL_PORT_IN_A][CONTROL_RF_IN] = 9;
    PEArch::control_mat[CONTROL_RF_OUT][CONTROL_FIFO_IN][CONTROL_RF_IN] = 10;
    PEArch::control_mat[CONTROL_RF_OUT][CONTROL_FIFO_IN][CONTROL_PORT_IN_B] = 11;
    PEArch::control_mat[CONTROL_FIFO_OUT][CONTROL_PORT_IN_A][CONTROL_PORT_IN_B] = 12;
    PEArch::control_mat[CONTROL_FIFO_OUT][CONTROL_PORT_IN_A][CONTROL_RF_IN] = 13;
    PEArch::control_mat[CONTROL_FIFO_OUT][CONTROL_FIFO_IN][CONTROL_RF_IN] = 14;
    PEArch::control_mat[CONTROL_FIFO_OUT][CONTROL_FIFO_IN][CONTROL_PORT_IN_B] = 15;

    auto nt = static_cast<unsigned long>(numThreads);
    PEArch::out.reserve(nt);
    PEArch::opThreads.reserve(nt);
    for (int i = 0; i < numThreads; ++i) {
        PEArch::out[i] = -1;
        PEArch::opThreads[i] = nullptr;
    }


}

Operator *PEArch::getOperator(int ThreadID) const {
    return PEArch::opThreads[ThreadID];
}

void PEArch::setOperator(Operator *op, int threadID) {
    PEArch::opThreads[threadID] = op;
}

int PEArch::getType() const {
    return type;
}

int PEArch::getId() const {
    return id;
}

int PEArch::getOut(int threadID) const {
    return PEArch::out[threadID];
}

void PEArch::setOut(int out, int threadID) {
    PEArch::out[threadID] = out;
}

initial_conf_t PEArch::getConf(int threadID) {

    int confAddr = 0;
    int dst = 0;
    int srca = 0;
    int srcb = 0;
    bool isCond = false;

    if (PEArch::opThreads[threadID]->getOpCode() == OP_BEQ || PEArch::opThreads[threadID]->getOpCode() == OP_BNE ||
        PEArch::opThreads[threadID]->getOpCode() == OP_SLT || PEArch::opThreads[threadID]->getOpCode() == OP_SGT) {
        isCond = true;
    }

    if (PEArch::getOut(threadID) == -1) {
        if (PEArch::opThreads[threadID]->getType() == OP_OUT) {
            dst = CONTROL_FIFO_OUT;
        } else if (!isCond) {
            dst = CONTROL_RF_OUT;
        } else {
            dst = CONTROL_PORT_OUT_A;
        }
    } else {
        dst = PEArch::getOut(threadID);
    }

    if (PEArch::opThreads[threadID]->getType() == OP_IN) {
        srca = CONTROL_FIFO_IN;
    } else {
        srca = CONTROL_PORT_IN_A;
    }

    if (PEArch::opThreads[threadID]->getType() == OP_IMMEDIATE) {
        srcb = CONTROL_RF_IN;
    } else {
        srcb = CONTROL_PORT_IN_B;
    }

    initial_conf_t conf;
    conf.pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
    conf.pe_instruction_conf.pe_id = static_cast<unsigned long>(PEArch::id) + 1;
    conf.pe_instruction_conf.thread_id = static_cast<unsigned long>(threadID);
    conf.pe_instruction_conf.inst_addr = static_cast<unsigned long>(confAddr);
    conf.pe_instruction_conf.alu_op = PEArch::getPeOpCode(PEArch::opThreads[threadID]->getOpCode());
    conf.pe_instruction_conf.control = PEArch::control_mat[dst][srca][srcb];
    conf.pe_instruction_conf.rf_raddr = 0;
    conf.pe_instruction_conf.rf_waddr = 0;

    return conf;
}

initial_conf_t PEArch::getConstConf(int threadID) {
    initial_conf_t conf;
    int constAddr = 0;
    conf.pe_constant_conf.conf_type = CGRA_CONF_SET_PE_CONST;
    conf.pe_constant_conf.pe_id = static_cast<unsigned long>(PEArch::id) + 1;
    conf.pe_constant_conf.thread_id = static_cast<unsigned long>(threadID);
    conf.pe_constant_conf.const_addr = static_cast<unsigned long>(constAddr);
    conf.pe_constant_conf.constant = static_cast<unsigned long>(PEArch::opThreads[threadID]->getConstant());

    return conf;
}

initial_conf_t PEArch::getPcMaxConf(int threadID) {
    initial_conf_t conf;
    conf.pe_pc_max_conf.conf_type = CGRA_CONF_SET_PE_PC_MAX;
    conf.pe_pc_max_conf.pe_id = static_cast<unsigned long>(PEArch::id) + 1;
    conf.pe_pc_max_conf.thread_id = static_cast<unsigned long>(threadID);
    conf.pe_pc_max_conf.pc_max = 0;

    return conf;
}

initial_conf_t PEArch::getPcLoopConf(int threadID) {
    initial_conf_t conf;
    conf.pe_pc_loop_conf.conf_type = CGRA_CONF_SET_PE_PC_LOOP;
    conf.pe_pc_loop_conf.pe_id = static_cast<unsigned long>(PEArch::id) + 1;
    conf.pe_pc_loop_conf.thread_id = static_cast<unsigned long>(threadID);
    conf.pe_pc_loop_conf.pc_loop = 0;

    return conf;
}

initial_conf_t PEArch::getStoreIgnoreConf(int threadID) {

    initial_conf_t conf;
    conf.pe_store_ignore_conf.conf_type = CGRA_CONF_SET_PE_IGNORE;
    conf.pe_store_ignore_conf.pe_id = static_cast<unsigned long>(PEArch::id) + 1;
    conf.pe_store_ignore_conf.thread_id = static_cast<unsigned long>(threadID);
    conf.pe_store_ignore_conf.store_ignore = static_cast<unsigned long>(PEArch::opThreads[threadID]->getLevel());

    return conf;
}

initial_conf_t PEArch::getLoadIgnoreConf(int threadID) {

    initial_conf_t conf;
    conf.pe_load_ignore_conf.conf_type = CGRA_CONF_SET_PE_IGNORE;
    conf.pe_load_ignore_conf.pe_id = static_cast<unsigned long>(PEArch::id) + 1;
    conf.pe_load_ignore_conf.thread_id = static_cast<unsigned long>(threadID);
    conf.pe_load_ignore_conf.load_ignore = static_cast<unsigned long>(PEArch::opThreads[threadID]->getLevel());

    return conf;
}

unsigned long PEArch::getPeOpCode(int operatorOpCode) {
    return pe_op_code[operatorOpCode];
}
