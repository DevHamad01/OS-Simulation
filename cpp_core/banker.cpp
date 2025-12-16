#include "banker.h"
#include <iostream>

BankerAlgorithm::BankerAlgorithm(int p, int r, const std::vector<int>& avail, 
                               const std::vector<std::vector<int>>& max,
                               const std::vector<std::vector<int>>& alloc) 
    : num_processes(p), num_resources(r), available(avail), maximum(max), allocation(alloc) {
    
    // Calculate Need Matrix
    need.resize(num_processes, std::vector<int>(num_resources));
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

JsonValue BankerAlgorithm::solve() {
    JsonValue result = JsonValue::object();
    std::vector<int> work = available;
    std::vector<bool> finish(num_processes, false);
    std::vector<std::string> safe_seq;
    JsonValue steps = JsonValue::array();

    int completed = 0;
    bool made_progress = true;

    while (completed < num_processes && made_progress) {
        made_progress = false;
        
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) {
                bool can_allocate = true;
                for (int j = 0; j < num_resources; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }

                if (can_allocate) {
                    // Execute process
                    JsonValue step = JsonValue::object();
                    step["process"] = "P" + std::to_string(i);
                    JsonValue work_arr = JsonValue::array();
                    for(int w : work) work_arr.push_back(w);
                    step["work_before"] = work_arr;
                    
                    // Release resources
                    for (int j = 0; j < num_resources; j++) {
                        work[j] += allocation[i][j];
                    }
                    
                    JsonValue work_after = JsonValue::array();
                    for(int w : work) work_after.push_back(w);
                    step["work_after"] = work_after;
                    
                    steps.push_back(step);

                    finish[i] = true;
                    safe_seq.push_back("P" + std::to_string(i));
                    completed++;
                    made_progress = true;
                }
            }
        }
    }

    if (completed == num_processes) {
        result["safe"] = true;
        JsonValue seq_arr = JsonValue::array();
        for(const auto& s : safe_seq) seq_arr.push_back(s);
        result["safe_sequence"] = seq_arr;
    } else {
        result["safe"] = false;
        result["safe_sequence"] = JsonValue::array(); // Empty
    }
    
    result["steps"] = steps;
    return result;
}
