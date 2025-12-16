#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Process {
    int pid;
    std::vector<int> reference_string;
};

enum class Algorithm { FIFO, LRU, OPTIMAL };

// Function declarations only
json runPageReplacement(const std::vector<Process>& processes, int ram_size, int frame_size, Algorithm algo);
void runPageReplacementFromFile();
