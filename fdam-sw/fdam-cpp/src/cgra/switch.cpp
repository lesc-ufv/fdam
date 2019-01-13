#include <fdam/cgra/switch.h>

Switch::Switch(int id, int size) : id(id), size(size) {
    for (int i = 0; i < size; ++i) {
        Switch::port_out.push_back(-1);
    }
}

int Switch::getId() const {
    return id;
}

int Switch::getSize() const {
    return size;
}

bool Switch::connectPort(int port_in, int port_out) {
    if (Switch::port_out[port_out] == -1) {
        Switch::port_out[port_out] = port_in;
        return true;
    } else return Switch::port_out[port_out] == port_in;
}

void Switch::setId(int id) {
    Switch::id = id;
}

void Switch::setSize(int size) {
    Switch::size = size;
    Switch::port_out.clear();
    for (int i = 0; i < size; ++i) {
        Switch::port_out.push_back(-1);
    }
}

Switch::~Switch() {
    Switch::port_out.clear();
}

void Switch::reset() {

    for (int i = 0; i < size; ++i) {
        Switch::port_out[i] = -1;
    }
}

const std::vector<int> &Switch::getPortOut() const {
    return Switch::port_out;
}

void Switch::disconnectPort(int port_out) {
    Switch::port_out[port_out] = -1;
}

initial_conf_t Switch::getSwConf(bool isNetBranch, int threadID, int confAddr) {

    initial_conf_t conf;
    int bits = intlog(Switch::size, 2);
    int mask = (1 << bits) - 1;

    conf.net_switch_conf.conf_type = isNetBranch ? CGRA_CONF_NET_BRANCH_SWITCH : CGRA_CONF_NET_SWITCH;
    conf.net_switch_conf.switch_number = static_cast<unsigned long>(Switch::id) + 1;
    conf.net_switch_conf.thread_id = (unsigned long) threadID;
    conf.net_switch_conf.inst_addr = (unsigned long) confAddr;
    conf.net_switch_conf.switch_conf = 0;

    for (int i = Switch::size - 1; i >= 0; i--) {
        if (Switch::port_out[i] >= 0) {
            conf.net_switch_conf.switch_conf =
                    conf.net_switch_conf.switch_conf << bits | (Switch::port_out[i] & mask);
        } else {
            conf.net_switch_conf.switch_conf = conf.net_switch_conf.switch_conf << bits;
        }
    }
    return conf;
}

initial_conf_t Switch::getSwPcMaxConf(bool isNetBranch, int threadID) {

    initial_conf_t conf;
    conf.net_pc_max_conf.conf_type = isNetBranch ? CGRA_CONF_SET_NET_BRANCH_PC_MAX : CGRA_CONF_SET_NET_PC_MAX;
    conf.net_pc_max_conf.switch_number = static_cast<unsigned long>(Switch::id) + 1;
    conf.net_pc_max_conf.thread_id = (unsigned long) threadID;
    conf.net_pc_max_conf.pc_max = 0;

    return conf;
}

initial_conf_t Switch::getSwLoopConf(bool isNetBranch, int threadID) {

    initial_conf_t conf;
    conf.net_pc_loop_conf.conf_type = isNetBranch ? CGRA_CONF_SET_NET_BRANCH_PC_LOOP : CGRA_CONF_SET_NET_PC_LOOP;
    conf.net_pc_loop_conf.switch_number = static_cast<unsigned long>(Switch::id) + 1;
    conf.net_pc_loop_conf.thread_id = (unsigned long) threadID;
    conf.net_pc_loop_conf.pc_loop = 0;

    return conf;
}

int Switch::intlog(double x, double base) {
    return (int) std::ceil(log(x) / log(base));
}

bool Switch::getUsed() {

    for (auto p:Switch::port_out) {
        if (p != -1) {
            return true;
        }
    }

    return false;
}
