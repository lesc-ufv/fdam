#ifndef CGRASCHEDULER_DATAFLOW_H
#define CGRASCHEDULER_DATAFLOW_H

#include <fdam/cgra/data_flow_defs.h>
#include <fdam/cgra/cgra_arch_defs.h>
#include <fdam/cgra/operator.h>
#include <queue>
#include <map>
#include <iostream>
#include <fstream>

class DataFlow {

private:
    int id;
    std::string name;
    std::map<int, Operator *> op_array;
    std::map<int, std::vector<int>> graph;
    int num_op_in;
    int num_op_out;
    int num_op;

    void addOperator(Operator *op);

public:

    explicit DataFlow(int id, std::string name);

    ~DataFlow();

    Operator *removeOperator(int op_id);

    void compute();

    const std::map<int, Operator *> &getOpArray() const;

    Operator *getOp(int id);

    void toDot(std::string fileNamePath);

    void connect(Operator *src, Operator *dst, int dstPort);

    void updateOpLevel();

    int getId() const;

    const std::string &getName() const;

    const std::map<int, std::vector<int>> &getGraph() const;

    int getNumOpIn() const;

    int getNumOpOut() const;

    int getNumOp() const;
};


#endif //CGRASCHEDULER_DATAFLOW_H
