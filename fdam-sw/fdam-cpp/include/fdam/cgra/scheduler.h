#ifndef CGRASCHEDULER_SCHELUDER_H
#define CGRASCHEDULER_SCHELUDER_H

#include <queue>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/cgra_arch.h>

class Scheduler {

private:
    std::map<int, DataFlow *> dataflows;

    CgraArch *cgra;

    bool mapAndRoute(int ThreadId);

public:
    explicit Scheduler(CgraArch *cgra);

    Scheduler();

    ~Scheduler();

    bool addDataFlow(DataFlow *df, int threadID);

    void setCgra(CgraArch *cgra);

    CgraArch *getCgra();

    bool scheduling();

    int placeAndRoute(std::vector<int> &mapping, int threadID);


};


#endif //CGRASCHEDULER_SCHELUDER_H
