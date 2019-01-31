#ifndef CGRASCHEDULER_OMEGA_H
#define CGRASCHEDULER_OMEGA_H

#include <cmath>
#include <map>
#include <vector>
#include <fdam/cgra/path_finder.h>
#include <fdam/cgra/switch.h>
#include <fdam/cgra/global.h>

class Omega {

private:
    int size;

    int radix;

    int num_stagies;

    int num_swicth_stages;

    int num_total_switch;

    std::map<int, std::vector<int>> graph;

    std::map<std::pair<int, int>, std::vector<std::vector<int>>> route_table;

    std::map<std::pair<int, int>, bool> used_route;

    std::vector<Switch> conf_array;

    void createSwitchGraph();

    int getPortOut(int sw1, int sw2);

public:

    Omega(int size, int radix, int num_extra_stagies);

    ~Omega();

    bool addRoute(int port_in, int port_out);

    void removeRoute(int port_in, int port_out);

    void createRouteTable();

    void printRouteTable();

    void reset();

    const std::vector<Switch> &getConfArray() const;

    int getNumSwicthStages() const;

    int getNumStages();

    int getSize();

    int getRadix();

};


#endif //CGRASCHEDULER_OMEGA_H
