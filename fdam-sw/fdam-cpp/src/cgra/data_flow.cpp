#include <fdam/cgra/data_flow.h>


DataFlow::DataFlow(int id, std::string name) : id(id), name(std::move(name)) {}

DataFlow::~DataFlow() = default;

void DataFlow::addOperator(Operator *op) {
    op->setDataFlowId(DataFlow::id);
    DataFlow::op_array[op->getId()] = op;
}

Operator *DataFlow::removeOperator(int op_id) {
    Operator *r = DataFlow::op_array[op_id];
    DataFlow::op_array.erase(op_id);
    r->setDataFlowId(-1);
    return r;
}

void DataFlow::compute() {

}

const std::map<int, Operator *> &DataFlow::getOpArray() const {
    return op_array;
}

Operator *DataFlow::getOp(int id) {
    if (DataFlow::op_array.find(id) != DataFlow::op_array.end()) {
        return DataFlow::op_array[id];
    }
    return nullptr;
}

void DataFlow::toDot(std::string fileNamePath) {

    std::ofstream myfile;
    myfile.open(fileNamePath);
    myfile << "digraph " << DataFlow::name << "{" << std::endl;
    for (auto op:DataFlow::op_array) {
        if (op.second->getType() == OP_IN) {
            myfile << " " << op.first << " [ label = IN" << op.second->getId() << " ]" << std::endl;
        } else if (op.second->getType() == OP_OUT) {
            myfile << " " << op.first << " [ label = OUT" << op.second->getId() << " ]" << std::endl;
        } else if (op.second->getType() == OP_IMMEDIATE) {
            myfile << " " << op.first;
            myfile << " [ label = " << op_label[op.second->getOpCode()] << "i";
            myfile << ", VALUE = " << op.second->getConstant();
            myfile << "]" << std::endl;
            myfile << " " << op.first << "." << op.second->getConstant() << " [ label = " << op.second->getConstant()
                   << " ]" << std::endl;

        } else {
            myfile << " " << op.first << " [ label = " << op_label[op.second->getOpCode()] << "]" << std::endl;
        }

    }
    for (auto op:DataFlow::op_array) {
        if (op.second->getType() == OP_IMMEDIATE) {
            myfile << " " << op.first << "." << op.second->getConstant() << " -> " << op.first << std::endl;
        }
        for (auto op_dst:op.second->getDst()) {
            myfile << " " << op.first << " -> " << op_dst << std::endl;
        }
    }
    myfile << "}" << std::endl;
    myfile.close();
}

void DataFlow::connect(Operator *src, Operator *dst, int dstPort) {

    DataFlow::graph[src->getId()].push_back(dst->getId());

    src->getDst().push_back(dst->getId());

    if (dstPort == PORT_A) {
        dst->setSrcA(src->getId());
    } else if (dstPort == PORT_B) {
        dst->setSrcB(src->getId());
    } else if (dstPort == PORT_BRANCH) {
        dst->setBranchIn(src->getId());
    }
    DataFlow::updateOpLevel();
}

void DataFlow::updateOpLevel() {
    std::queue<int> q;
    int parent;
    for (auto op:DataFlow::op_array) {
        if (op.second->getType() == OP_IN) {
            q.push(op.first);
            while (!q.empty()) {
                parent = q.front();
                q.pop();
                for (auto child:DataFlow::graph[parent]) {
                    int lp = DataFlow::op_array[parent]->getLevel();
                    int lc = DataFlow::op_array[child]->getLevel();
                    if (lp >= lc) {
                        DataFlow::op_array[child]->setLevel(lp + 1);
                    }
                    q.push(child);
                }

            }
        }
    }
    for (auto op:DataFlow::op_array) {
        if (op.second->getType() == OP_IN) {
            int level = 1;
            for (int i = 0; i < DataFlow::graph[op.first].size(); i++) {
                int child = DataFlow::graph[op.first][i];
                if (i == 0) {
                    level = DataFlow::op_array[child]->getLevel();
                } else {
                    if (DataFlow::op_array[child]->getLevel() < level) {
                        level = DataFlow::op_array[child]->getLevel();
                    }
                }
            }
            op.second->setLevel(level - 1);
        }
    }
}

int DataFlow::getId() const {
    return id;
}

const std::string &DataFlow::getName() const {
    return name;
}

const std::map<int, std::vector<int>> &DataFlow::getGraph() const {
    return graph;
}
