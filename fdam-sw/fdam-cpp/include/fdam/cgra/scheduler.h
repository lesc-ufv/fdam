#ifndef CGRASCHEDULER_SCHELUDER_H
#define CGRASCHEDULER_SCHELUDER_H

#include <queue>
#include <fdam/cgra/data_flow.h>
#include <fdam/cgra/cgra_arch.h>
#include <fdam/cgra/global.h>

class Scheduler {

private:
    std::map<int, DataFlow *> dataflows;
    CgraArch *cgraArch;

    bool mapAndRoute(int ThreadId);

    int placeAndRoute(std::vector<int> &mapping, int threadID);

    int getRandomPE(int type);

public:
    explicit Scheduler(CgraArch *cgra);

    Scheduler();

    ~Scheduler();

    bool addDataFlow(DataFlow *df, int threadID);

    void setCgra(CgraArch *cgra);

    CgraArch *getCgra();

    bool scheduling();
};


#endif //CGRASCHEDULER_SCHELUDER_H
