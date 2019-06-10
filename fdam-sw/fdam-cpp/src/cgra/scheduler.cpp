#include <fdam/cgra/scheduler.h>
#include <fdam/cgra/scheduler_defs.h>

Scheduler::Scheduler() = default;

Scheduler::Scheduler(CgraArch *cgra) : cgraArch(cgra) {
    int num_thread = Scheduler::cgraArch->getNumThreads();
    for(int i = 0; i < num_thread; i++){
         Scheduler::cgraArch->reset(i);
    }
}

Scheduler::~Scheduler() {
    Scheduler::dataflows.clear();
}

bool Scheduler::addDataFlow(DataFlow *df, int threadID, int groupID) {
    if (Scheduler::cgraArch) {
        if (threadID >= 0 && threadID <= (Scheduler::cgraArch->getNumThreads() - 1)) {
            
            Scheduler::dataflows[threadID] = df;
            Scheduler::dataflow_group[df->getId()] = groupID;
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

int Scheduler::scheduling() {

    for (const auto &df : Scheduler::dataflows) {
        int r = mapAndRoute(df.first);
        if (r != SCHEDULE_SUCCESS) {
            return r;
        }
    }

    return SCHEDULE_SUCCESS;
}

int Scheduler::mapAndRoute(int threadID) {

    if (dataflows[threadID]->getNumOp() > cgraArch->getNumPe()) {
        return SCHEDULER_INSUFFICIENT_PE;
    }
    if (dataflows[threadID]->getNumOpIn() > cgraArch->getNumPeIn()) {
        return SCHEDULER_INSUFFICIENT_PE_IN;
    }
    if (dataflows[threadID]->getNumOpOut() > cgraArch->getNumPeOut()) {
        return SCHEDULER_INSUFFICIENT_PE_OUT;
    }
    auto operators = dataflows[threadID]->getOpArray();
    auto pes = cgraArch->getPeArray();
    std::vector<int> solution;
    std::deque<int> pe_free;
    std::deque<int> pe_in_free;
    std::deque<int> pe_out_free;
    int pe_swap;
    int swapness[pes.size()];
    int i = 0;
    int group = Scheduler::dataflow_group[Scheduler::dataflows[threadID]->getId()];
    std::vector<int>pe_list_aux;
    random_selector<> selector{};

    if (Scheduler::data_flow_mapping.find(group) != Scheduler::data_flow_mapping.end()) {
        solution = Scheduler::data_flow_mapping[group];
    } else {
        solution.reserve(pes.size());
        for (auto pe:pes) {
            solution.push_back(-1);
            swapness[i++] = -1;
        }
        int idx = 0;
        for (auto op:operators) {
            if (op.second->getType() == OP_IN) {
                pe_list_aux.clear();
                for (auto p:pes) {
                    if(solution[p.first] == -1 && p.second->getType() == PE_IN){
                        pe_list_aux.push_back(p.first);
                    }
                }
            } else if (op.second->getType() == OP_OUT) {
                pe_list_aux.clear();
                for (auto p:pes) {
                    if(solution[p.first] == -1 && p.second->getType() == PE_OUT){
                        pe_list_aux.push_back(p.first);
                    }
                }
            } else {
                pe_list_aux.clear();
                for (auto p:pes) {
                    if(solution[p.first] == -1 && p.second->getType() == PE_BASIC){
                        pe_list_aux.push_back(p.first);
                    }
                }
                if(pe_list_aux.empty()){
                    for (auto p:pes) {
                        if(solution[p.first] == -1){
                            pe_list_aux.push_back(p.first);
                        }
                    }
                }
            }
            idx = selector(pe_list_aux);
            solution[idx] = op.second->getId();
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
    }
    do {
        pe_swap = Scheduler::placeAndRoute(solution, threadID);
        if (pe_swap == -1) {
            Scheduler::data_flow_mapping[group] = solution;
            return SCHEDULE_SUCCESS;
        }
        int swap = -1;
        if (pe_swap < cgraArch->getNumPeIn()) {
            if (!pe_in_free.empty()) {
                swap = pe_in_free.back();
                pe_in_free.pop_back();
                pe_in_free.push_front(pe_swap);
            }
        } else if (pe_swap < (cgraArch->getNumPeIn() + cgraArch->getNumPeOut())) {
            if (!pe_out_free.empty()) {
                swap = pe_out_free.back();
                pe_out_free.pop_back();
                pe_out_free.push_front(pe_swap);
            }
        } else {
            if (!pe_free.empty()) {
                swap = pe_free.back();
                pe_free.pop_back();
                pe_free.push_front(pe_swap);
            }
        }
        if (swap == -1) {
            return SCHEDULE_PE_FREE_IS_GONE;
        };
        if (swapness[pe_swap] == swap) {
            return SCHEDULE_HAS_LOOPED;
        }
        std::swap(solution[pe_swap], solution[swap]);
        swapness[pe_swap] = swap;
    } while (true);

}

int Scheduler::placeAndRoute(std::vector<int> &mapping, int threadID) {

    std::map<int, int> mapping_op;
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

    for (int j = 0; j < mapping.size(); ++j) {
        mapping_op[mapping[j]] = j;
    }
    printf("TRY\n");
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
                        return i;
                    } else {
                        for (auto p:op_src->getDst()) {
                            op_dst = Scheduler::dataflows[threadID]->getOp(p);
                            pe_dst = Scheduler::cgraArch->getPE(mapping_op[p]);
                            if (pe_dst && op_dst) {
                                if (op_dst->getBranchIn() == op_src->getId()) {
                                    if (!net_branch->addRoute(pe_src->getId(), pe_dst->getId())) {
                                        return i;
                                    }
                                    //printf("1 %d %d\n",pe_src->getId(),pe_dst->getId());
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
                                        return i;
                                    }
                                    
                                    if (pe_src->getOut(threadID) == PORT_A) {
                                        if (!net->addRoute(pe_src_port_a, pe_dst_port_in)) {
                                            return i;
                                        }else{
                                            //printf("%d %d\n",pe_src_port_a,pe_dst_port_in);
                                        }
                                    } else if (pe_src->getOut(threadID) == PORT_B) {
                                        if (!net->addRoute(pe_src_port_b, pe_dst_port_in)) {
                                            return i;
                                        }else{
                                             //printf("%d %d\n",pe_src_port_b,pe_dst_port_in);
                                        }
                                    } else {
                                        if (!net->addRoute(pe_src_port_a, pe_dst_port_in)) {
                                            if (!net->addRoute(pe_src_port_b, pe_dst_port_in)) {
                                                return i;
                                            } else {
                                                pe_src->setOut(PORT_B, threadID);
                                                //printf("%d %d\n",pe_src_port_b,pe_dst_port_in);
                                            }
                                        } else {
                                            pe_src->setOut(PORT_A, threadID);
                                            //printf("%d %d\n",pe_src_port_a,pe_dst_port_in);
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

void Scheduler::reset() {
    Scheduler::dataflows.clear();
    Scheduler::dataflow_group.clear();
    Scheduler::data_flow_mapping.clear();
}

std::vector<int> Scheduler::getMapping(int dataFlowId, int threadID){
    int group = Scheduler::dataflow_group[Scheduler::dataflows[threadID]->getId()];
    return  Scheduler::data_flow_mapping[group];
}

