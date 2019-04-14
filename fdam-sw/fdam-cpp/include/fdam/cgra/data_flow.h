#ifndef CGRASCHEDULER_DATAFLOW_H
#define CGRASCHEDULER_DATAFLOW_H

#include <queue>
#include <map>
#include <iostream>
#include <fstream>

#include <fdam/cgra/data_flow_defs.h>
#include <fdam/cgra/cgra_arch_defs.h>
#include <fdam/cgra/operator.h>
#include <fdam/cgra/abs.h>
#include <fdam/cgra/add.h>
#include <fdam/cgra/and.h>
#include <fdam/cgra/beq.h>
#include <fdam/cgra/bne.h>
#include <fdam/cgra/input_stream.h>
#include <fdam/cgra/max.h>
#include <fdam/cgra/min.h>
#include <fdam/cgra/mult.h>
#include <fdam/cgra/mux.h>
#include <fdam/cgra/not.h>
#include <fdam/cgra/or.h>
#include <fdam/cgra/output_stream.h>
#include <fdam/cgra/pass_a.h>
#include <fdam/cgra/pass_b.h>
#include <fdam/cgra/sgt.h>
#include <fdam/cgra/shl.h>
#include <fdam/cgra/shr.h>
#include <fdam/cgra/slt.h>
#include <fdam/cgra/sub.h>
#include <fdam/cgra/xor.h>


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

    void setId(int id);

    const std::string &getName() const;

    const std::map<int, std::vector<int>> &getGraph() const;

    int getNumOpIn() const;

    int getNumOpOut() const;

    int getNumOp() const;

    int getNumEdges() const;
};


#endif //CGRASCHEDULER_DATAFLOW_H
