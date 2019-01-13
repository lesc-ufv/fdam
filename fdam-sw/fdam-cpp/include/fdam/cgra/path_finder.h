#ifndef CGRASCHEDULER_PATHFINDER_H
#define CGRASCHEDULER_PATHFINDER_H

#include <vector>
#include <map>
#include <queue>

class PathFinder {
public:
    static bool
    pathFinder(int start, int target, std::map<int, std::vector<int>> graph, std::vector<std::vector<int>> &paths);
};


#endif //CGRASCHEDULER_PATHFINDER_H
