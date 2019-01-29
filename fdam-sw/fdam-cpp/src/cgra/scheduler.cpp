#include <fdam/cgra/scheduler.h>

Scheduler::Scheduler() = default;

Scheduler::Scheduler(CgraArch *cgra) : cgraArch(cgra) {}

Scheduler::~Scheduler() {
    Scheduler::dataflows.clear();
}

bool Scheduler::addDataFlow(DataFlow *df, int threadID) {
    if (Scheduler::cgraArch) {
        if (threadID >= 0 && threadID < Scheduler::cgraArch->getNumThreads()) {
            Scheduler::dataflows[threadID] = df;
            return true;
        }
    }
    return false;
}

void Scheduler::setCgra(CgraArch *cgra) {
    Scheduler::cgraArch = cgra;
}

CgraArch *Scheduler::getCgra() {
    return Scheduler::cgraArch;
}

bool Scheduler::scheduling() {

    for (const auto &df : Scheduler::dataflows) {
        int r = mapAndRoute(df.first);
        if (!r) {
            printf("Map and Route error:\n");
            printf("Thread ID: %d\n", df.first);
            printf("DataFlow name: %s\n", df.second->getName().c_str());
            return false;
        } else {
            printf("Map and Route success!\n");
            printf("Thread ID: %d\n", df.first);
            printf("DataFlow name: %s\n", df.second->getName().c_str());
        }
    }

    return true;
}

bool Scheduler::mapAndRoute(int threadID) {

    if (dataflows[threadID]->getNumOp() > cgraArch->getNumPe()) {
        return false;
    }
    if (dataflows[threadID]->getNumOpIn() > cgraArch->getNumPeIn()) {
        return false;
    }
    if (dataflows[threadID]->getNumOpOut() > cgraArch->getNumPeOut()) {
        return false;
    }
    srand(0);
    auto operators = dataflows[threadID]->getOpArray();
    auto pes = cgraArch->getPeArray();
    std::vector<int> solution;
    std::deque<int> pe_free;
    std::deque<int> pe_in_free;
    std::deque<int> pe_out_free;

    solution.reserve(pes.size());
    for (auto pe:pes) {
        solution.push_back(-1);
    }
    int idx = 0;
    for (auto op:operators) {
        if (op.second->getType() == OP_IN) {
            do {
                idx = Scheduler::getRandomPE(PE_IN);
            } while (solution[idx] != -1);
            solution[idx] = op.second->getId();
        } else if (op.second->getType() == OP_OUT) {
            do {
                idx = Scheduler::getRandomPE(PE_OUT);
            } while (solution[idx] != -1);
            solution[idx] = op.second->getId();
        } else{
            do {
                idx = Scheduler::getRandomPE(PE_BASIC);
            } while (solution[idx] != -1);
            solution[idx] = op.second->getId();
        }
    }

    for (int k = 0; k < solution.size(); k++) {
        if (solution[k] == -1) {
            if (pes[k]->getType() == PE_IN) {
                pe_in_free.push_back(k);
            } else if (pes[k]->getType() == PE_OUT) {
                pe_out_free.push_back(k);
            } else {
                pe_free.push_back(k);
            }
        }
    }
    int j = 0;
    int r = 0;
    do {

        printf("%d ", j++);
        for (int k = 0; k < solution.size(); k++) {
            if (solution[k] != -1) {
                printf("PE%d[OP%d] ", k, solution[k]);
            }
        }
        printf("\n");

        r = Scheduler::placeAndRoute(solution, threadID);

        if (r == -1) {
            return true;
        }

        int swap = -1;
        if (r < cgraArch->getNumPeIn()) {
            if (!pe_in_free.empty()) {
                swap = pe_in_free.back();
                pe_in_free.pop_back();
                pe_in_free.push_front(r);
            }
        } else if (r < (cgraArch->getNumPeIn() + cgraArch->getNumPeOut())) {
            if (!pe_out_free.empty()) {
                swap = pe_out_free.back();
                pe_out_free.pop_back();
                pe_out_free.push_front(r);
            }
        } else {
            if (!pe_free.empty()) {
                swap = pe_free.back();
                pe_free.pop_back();
                pe_free.push_front(r);
            }
        }
        if (swap == -1) break;
        std::swap(solution[r], solution[swap]);
        printf("\nSWAP: %d -> %d: ", r, swap);


    } while (r >= 0);

    return false;

}


int Scheduler::placeAndRoute(std::vector<int> &mapping, int threadID) {

    Operator *op_src = nullptr;
    Operator *op_dst = nullptr;
    PEArch *pe_dst = nullptr;
    PEArch *pe_src = nullptr;
    int pe_src_port_a;
    int pe_src_port_b;
    int pe_dst_port_in;

    Scheduler::cgraArch->reset(threadID);
    Omega *net = Scheduler::cgraArch->getNet(threadID);
    Omega *net_branch = Scheduler::cgraArch->getNetBranch(threadID);
    Scheduler::cgraArch->setDataFlow(Scheduler::dataflows[threadID], threadID);

    for (int i = 0; i < mapping.size(); ++i) {
        if (mapping[i] != -1) {
            op_src = Scheduler::dataflows[threadID]->getOp(mapping[i]);
            pe_src = Scheduler::cgraArch->getPE(i);
            if (pe_src && op_src) {
                if (op_src->getType() == OP_IN && pe_src->getType() != PE_IN) {
                    return i;
                } else if (op_src->getType() == OP_OUT && pe_src->getType() != PE_OUT) {
                    return i;
                } else {
                    if (op_src->getDst().empty() && op_src->getType() != OP_OUT) {
                        printf("Data Flow Error: Operator ID: %d", op_src->getId());
                        exit(2);
                    } else {
                        for (auto p:op_src->getDst()) {
                            op_dst = nullptr;
                            pe_dst = nullptr;
                            for (int j = 0; j < mapping.size(); ++j) {
                                if (mapping[j] == p) {
                                    op_dst = Scheduler::dataflows[threadID]->getOp(mapping[j]);
                                    pe_dst = Scheduler::cgraArch->getPE(j);
                                    break;
                                }
                            }
                            if (pe_dst && op_dst) {
                                if (op_dst->getBranchIn() == op_src->getId()) {
                                    if (!net_branch->addRoute(pe_src->getId(), pe_dst->getId())) {
                                        return i;
                                    }
                                    pe_src->setOperator(op_src, threadID);
                                    pe_dst->setOperator(op_dst, threadID);

                                } else {
                                    pe_src_port_a = pe_src->getId() * 2;
                                    pe_src_port_b = (pe_src->getId() * 2) + 1;
                                    if (op_dst->getSrcA() == op_dst->getSrcB() && pe_src->getOut(threadID) != -1) {
                                        pe_dst_port_in = (pe_dst->getId() * 2) + 1;
                                    } else if (op_dst->getSrcA() == op_src->getId())
                                        pe_dst_port_in = pe_dst->getId() * 2;
                                    else if (op_dst->getSrcB() == op_src->getId())
                                        pe_dst_port_in = (pe_dst->getId() * 2) + 1;
                                    else {
                                        printf("DataFlow error: Operator ID: %d\n", op_src->getId());
                                        exit(2);
                                    }
                                    if (pe_src->getOut(threadID) == PORT_A) {
                                        if (!net->addRoute(pe_src_port_a, pe_dst_port_in)) {
                                            return i;
                                        }
                                    } else if (pe_src->getOut(threadID) == PORT_B) {
                                        if (!net->addRoute(pe_src_port_b, pe_dst_port_in)) {
                                            return i;
                                        }
                                    } else {
                                        if (!net->addRoute(pe_src_port_a, pe_dst_port_in)) {
                                            if (!net->addRoute(pe_src_port_b, pe_dst_port_in)) {
                                                return i;
                                            } else {
                                                pe_src->setOut(PORT_B, threadID);
                                            }
                                        } else {
                                            pe_src->setOut(PORT_A, threadID);
                                        }
                                    }
                                    pe_src->setOperator(op_src, threadID);
                                    pe_dst->setOperator(op_dst, threadID);

                                }

                            } else {
                                return i;
                            }
                        }
                    }
                }
            } else {
                return i;
            }
        }
    }
    return -1;
}

int Scheduler::getRandomPE(int type) {

    int value = 0;
    if (type == PE_IN) {
        value = static_cast<int>(random() % Scheduler::cgraArch->getNumPeIn());
        value = cgraArch->pe_list_in[value];
    } else if (type == PE_OUT) {
        value = static_cast<int>(random() % Scheduler::cgraArch->getNumPeOut());
        value = cgraArch->pe_list_out[value];
    } else {
        value = static_cast<int>(random() % (Scheduler::cgraArch->getNumPe() -
                                             (Scheduler::cgraArch->getNumPeOut() + Scheduler::cgraArch->getNumPeIn())));
        value = cgraArch->pe_list_basic[value];
    }

    return value;
}
