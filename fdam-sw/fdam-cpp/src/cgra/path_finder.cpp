#include <fdam/cgra/path_finder.h>

bool PathFinder::pathFinder(int start, int target, std::map<int, std::vector<int>> graph,
                            std::vector<std::vector<int>> &paths) {

    std::queue<std::vector<int>> queue_path;
    std::vector<int> current_path;

    current_path.push_back(start);
    queue_path.push(current_path);

    while (!queue_path.empty()) {
        current_path = queue_path.front();
        queue_path.pop();
        int last = current_path.at(current_path.size() - 1);
        if (last == target) {
            paths.push_back(current_path);
        }
        for (auto child:graph[last]) {
            bool notInPath = true;
            for (auto p:current_path) {
                if (p == child) {
                    notInPath = false;
                    break;
                }
            }
            if (notInPath) {
                std::vector<int> newpath(current_path);
                newpath.push_back(child);
                queue_path.push(newpath);
            }
        }

    }

    return !paths.empty();
}
