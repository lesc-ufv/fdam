#ifndef CGRASCHEDULER_SWITCH_H
#define CGRASCHEDULER_SWITCH_H

#include <cmath>
#include <vector>
#include <fdam/cgra/cgra_defs.h>
#include <fdam/cgra/global.h>

class Switch {
private:

    int id;

    int size;

    std::vector<int> port_out;

public:

    Switch(int id, int size);

    ~Switch();

    int getId() const;

    int getSize() const;

    bool connectPort(int port_in, int port_out);

    void disconnectPort(int port_out);

    void setId(int id);

    void setSize(int size);

    bool getUsed();

    void reset();

    const std::vector<int> &getPortOut() const;

    initial_conf_t getSwConf(bool isNetBranch, int threadID, int confAddr);

    initial_conf_t getSwPcMaxConf(bool isNetBranch, int threadID);

    initial_conf_t getSwLoopConf(bool isNetBranch, int threadID);

};


#endif //CGRASCHEDULER_SWITCH_H
