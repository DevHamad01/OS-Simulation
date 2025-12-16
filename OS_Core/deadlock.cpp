#include "deadlock.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

void runDeadlockDetection() {
    int n, m;
    cout << "\n========== DEADLOCK DETECTION ==========\n";

    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter number of resources: ";
    cin >> m;
    cin.ignore();

    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> request(n, vector<int>(m));
    vector<int> available(m);

    cout << "\nEnter Allocation Matrix:\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i << ": ";
        string line; getline(cin, line);
        parseInput(line, allocation[i], m);
    }

    cout << "\nEnter Request Matrix:\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i << ": ";
        string line; getline(cin, line);
        parseInput(line, request[i], m);
    }

    cout << "\nEnter Available Resources:\n";
    string line; getline(cin, line);
    parseInput(line, available, m);

    vector<bool> finished(n, false);
    vector<int> work = available;
    int completed = 0;
    int iteration = 1;

    json steps = json::array(); // JSON object for visualization

    while (true) {
        bool progress = false;
        json step;

        for (int i = 0; i < n; i++) {
            if (!finished[i]) {
                bool canRun = true;
                for (int j = 0; j < m; j++) {
                    if (request[i][j] > work[j]) {
                        canRun = false;
                        break;
                    }
                }

                step["process"] = i;
                step["can_run"] = canRun;
                step["work_before"] = work;

                if (canRun) {
                    for (int j = 0; j < m; j++) work[j] += allocation[i][j];
                    finished[i] = true;
                    completed++;
                    progress = true;
                    step["action"] = "executed";
                    step["work_after"] = work;
                }
                else {
                    step["action"] = "waiting";
                    step["work_after"] = work;
                }
                steps.push_back(step);
            }
        }

        if (!progress) break;
        iteration++;
    }

    // Deadlock summary
    json summary;
    summary["deadlock_detected"] = (completed != n);
    vector<int> deadlocked;
    for (int i = 0; i < n; i++)
        if (!finished[i]) deadlocked.push_back(i);
    summary["deadlocked_processes"] = deadlocked;
    steps.push_back(summary);

    // Save JSON file
    ofstream fout("visualizer/deadlock_steps.json");
    fout << steps.dump(4);
    fout.close();

    cout << "\n✅ Deadlock steps saved to visualizer/deadlock_steps.json\n";
}
