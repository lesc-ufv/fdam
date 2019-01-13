#ifndef CGRASCHEDULER_PE_H
#define CGRASCHEDULER_PE_H

#include <vector>
#include <fdam/cgra/operator.h>
#include <fdam/cgra/cgra_defs.h>
#include <fdam/cgra/cgra_arch_defs.h>
#include <fdam/cgra/data_flow_defs.h>

class PEArch {
private:
    int id;

    int type;

    int numThreads;

    std::vector<int> out;

    std::vector<Operator *> opThreads;

    unsigned long control_mat[4][2][2];

public:
    PEArch(int id, int type, int numThreads);

    Operator *getOperator(int ThreadID) const;

    void setOperator(Operator *op, int threadID);

    int getType() const;

    int getId() const;

    int getOut(int threadID) const;

    void setOut(int out, int threadID);

    unsigned long getPeOpCode(int operatorOpCode);

    initial_conf_t getConf(int threadID);

    initial_conf_t getConstConf(int threadID);

    initial_conf_t getPcMaxConf(int threadID);

    initial_conf_t getPcLoopConf(int threadID);

    initial_conf_t getStoreIgnoreConf(int threadID);

    initial_conf_t getLoadIgnoreConf(int threadID);
};


#endif //CGRASCHEDULER_PE_H
