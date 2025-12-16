#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include "memory.h"

using namespace std;

// Helper to check if page exists in frame
bool inFrame(const vector<int>& frame, int page) {
    return find(frame.begin(), frame.end(), page) != frame.end();
}

json runPageReplacement(const vector<Process>& processes, int ram_size, int frame_size, Algorithm algo) {
    json output;
    vector<int> frame(frame_size, -1);
    vector<json> steps;
    int page_faults = 0;

    for (const auto& proc : processes) {
        queue<int> fifo_queue;                // ✅ STORES FRAME INDICES
        unordered_map<int, int> last_used;    // page → last used time

        for (int i = 0; i < proc.reference_string.size(); i++) {
            int ref = proc.reference_string[i];
            bool fault = false;

            if (!inFrame(frame, ref)) {
                fault = true;
                page_faults++;

                int idx = -1;

                // Empty frame available
                auto it = find(frame.begin(), frame.end(), -1);
                if (it != frame.end()) {
                    idx = it - frame.begin();
                }
                else {
                    // Replacement needed
                    if (algo == Algorithm::FIFO) {
                        if (fifo_queue.empty()) {
                            idx = 0; // fallback safety
                        }
                        else {
                            idx = fifo_queue.front();
                            fifo_queue.pop();
                        }
                    }

                    else if (algo == Algorithm::LRU) {
                        int min_time = INT_MAX;
                        for (int j = 0; j < frame_size; j++) {
                            int page = frame[j];
                            if (last_used[page] < min_time) {
                                min_time = last_used[page];
                                idx = j;
                            }
                        }
                    }
                    else if (algo == Algorithm::OPTIMAL) {
                        int farthest = -1;
                        for (int j = 0; j < frame_size; j++) {
                            int page = frame[j];
                            int next_use = INT_MAX;

                            for (int k = i + 1; k < proc.reference_string.size(); k++) {
                                if (proc.reference_string[k] == page) {
                                    next_use = k;
                                    break;
                                }
                            }

                            if (next_use > farthest) {
                                farthest = next_use;
                                idx = j;
                            }
                        }
                    }
                }

                // 🔐 Final safety check
                if (idx < 0 || idx >= frame_size) {
                    cout << "❌ Internal error: invalid frame index\n";
                    continue;
                }
                if (idx < 0 || idx >= frame_size) {
                    cout << "❌ Fatal error: frame index out of bounds\n";
                    return output;
                }

                frame[idx] = ref;

                // Track FIFO correctly
                if (algo == Algorithm::FIFO) {
                    fifo_queue.push(idx);
                }
            }

            if (algo == Algorithm::LRU) {
                last_used[ref] = i;
            }

            json step;
            step["process"] = proc.pid;
            step["reference"] = ref;
            step["frames"] = frame;
            step["page_fault"] = fault;
            steps.push_back(step);
        }
    }

    output["steps"] = steps;
    output["total_page_faults"] = page_faults;
    return output;
}

void runPageReplacementFromFile() {
    ifstream fin("input.json");
    if (!fin.is_open()) {
        cout << "❌ Error: input.json not found!\n";
        return;
    }

    json input;
    try {
        fin >> input;
    }
    catch (const json::exception& e) {
        cout << "❌ JSON error: " << e.what() << endl;
        return;
    }
    fin.close();

    int ram_size = input["ram_size"];
    int frame_size = input["frame_size"];
    string algo_str = input["algorithm"];

    Algorithm algo = Algorithm::FIFO;
    if (algo_str == "LRU") algo = Algorithm::LRU;
    else if (algo_str == "OPTIMAL") algo = Algorithm::OPTIMAL;

    vector<Process> processes;
    for (auto& p : input["processes"]) {
        Process proc;
        proc.pid = p["pid"];
        for (auto& page : p["pages"]) {
            proc.reference_string.push_back(page);
        }
        processes.push_back(proc);
    }

    json output = runPageReplacement(processes, ram_size, frame_size, algo);

    ofstream fout("output.json");
    fout << output.dump(4);
    fout.close();

    cout << "✅ output.json generated successfully!\n";
}
