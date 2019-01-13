#include <fdam/cgra/scheduler.h>

Scheduler::Scheduler() = default;

Scheduler::Scheduler(CgraArch *cgra) : cgra(cgra) {}

Scheduler::~Scheduler() {
    Scheduler::dataflows.clear();
}

bool Scheduler::addDataFlow(DataFlow *df, int threadID) {
    if (threadID >= 0 && threadID < Scheduler::cgra->getNumThreads()) {
        Scheduler::dataflows[threadID] = df;
        return true;
    }
    return false;
}

void Scheduler::setCgra(CgraArch *cgra) {
    Scheduler::cgra = cgra;
}

CgraArch *Scheduler::getCgra() {
    return Scheduler::cgra;
}

bool Scheduler::scheduling() {

    for (const auto &df : Scheduler::dataflows) {
        int r = mapAndRoute(df.first);
        if (!r) {
            printf("Map and Route error:\n");
            printf("Thread ID: %d\n", df.first);
            printf("DataFlow name: %s\n", df.second->getName().c_str());
        } else {
            printf("Map and Route success!\n");
            printf("Thread ID: %d\n", df.first);
            printf("DataFlow name: %s\n", df.second->getName().c_str());
        }

    }

    return false;
}

bool Scheduler::mapAndRoute(int threadID) {

    auto operators = dataflows[threadID]->getOpArray();
    auto pes = cgra->getPeArray();

    std::vector<int> solution;
    std::deque<int> pe_free;
    std::deque<int> pe_in_free;
    std::deque<int> pe_out_free;

    if (operators.size() > pes.size()) {
        return false;
    }

    solution.reserve(pes.size());
    for (auto pe:pes) {
        solution.push_back(-1);
    }

    int idx = cgra->getNumPeIn() + cgra->getNumPeOut();
    int idx_in = 0;
    int idx_out = 0;
    for (auto op:operators) {
        if (op.second->getType() == OP_IN) {
            solution[idx_in] = op.second->getId();
            idx_in++;
        } else if (op.second->getType() == OP_OUT) {
            solution[idx_out + cgra->getNumPeIn()] = op.second->getId();
            idx_out++;
        } else {
            solution[idx] = op.second->getId();
            idx++;
        }
    }

    for (int k = 0; k < solution.size(); k++) {
        if (solution[k] == -1) {
            if (k < cgra->getNumPeIn()) {
                pe_in_free.push_back(k);
            } else if (k < (cgra->getNumPeIn() + cgra->getNumPeOut())) {
                pe_out_free.push_back(k);
            } else {
                pe_free.push_back(k);
            }
        }
    }
    int j = 0;
    int r = 0;
    do {

        r = Scheduler::placeAndRoute(solution, threadID);
        if (r == -1)
            return true;

        int swap;
        if (r < cgra->getNumPeIn()) {
            if (!pe_in_free.empty()) {
                swap = pe_in_free.back();
                pe_in_free.pop_back();
                pe_in_free.push_front(r);
            }
        } else if (r < (cgra->getNumPeIn() + cgra->getNumPeOut())) {
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
        std::swap(solution[r], solution[swap]);

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

    Scheduler::cgra->reset(threadID);
    Omega *net = Scheduler::cgra->getNet(threadID);
    Omega *net_branch = Scheduler::cgra->getNetBranch(threadID);
    Scheduler::cgra->setDataFlow(Scheduler::dataflows[threadID], threadID);

    for (int i = 0; i < mapping.size(); ++i) {
        if (mapping[i] != -1) {
            op_src = Scheduler::dataflows[threadID]->getOp(mapping[i]);
            pe_src = Scheduler::cgra->getPE(i);
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
                                    pe_dst = Scheduler::cgra->getPE(j);
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
                                        printf("DataFlow error: Operator ID: %d\n", i);
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