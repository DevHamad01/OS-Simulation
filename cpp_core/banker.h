#ifndef BANKER_H
#define BANKER_H

#include <vector>
#include <string>
#include "json_utils.h"

class BankerAlgorithm {
private:
    int num_processes;
    int num_resources;
    std::vector<int> available;
    std::vector<std::vector<int>> maximum;
    std::vector<std::vector<int>> allocation;
    std::vector<std::vector<int>> need;

public:
    BankerAlgorithm(int p, int r, const std::vector<int>& avail, 
                   const std::vector<std::vector<int>>& max,
                   const std::vector<std::vector<int>>& alloc);

    JsonValue solve();
};

#endif
