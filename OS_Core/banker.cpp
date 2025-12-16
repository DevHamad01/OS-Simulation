#include "banker.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;


void runBankersAlgorithm() {
    int n, m;
    cout << "========== BANKER'S ALGORITHM ==========\n";

    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter number of resources: ";
    cin >> m;
    cin.ignore();

    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> maximum(n, vector<int>(m));
    vector<vector<int>> need(n, vector<int>(m));
    vector<int> available(m);

    cout << "\nEnter Allocation Matrix (0 1 0 OR 010):\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i << ": ";
        string line; getline(cin, line);
        parseInput(line, allocation[i], m);
    }

    cout << "\nEnter Maximum Matrix:\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << i << ": ";
        string line; getline(cin, line);
        parseInput(line, maximum[i], m);
    }

    cout << "\nEnter Available Resources:\n";
    string line; getline(cin, line);
    parseInput(line, available, m);

    // Calculate Need Matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];


    vector<bool> finished(n, false);
    vector<int> safeSeq;
    vector<int> work = available;
    int completed = 0;
    int iteration = 1;

    // JSON object to store visualization steps
    json steps = json::array();

    while (completed < n) {
        bool progress = false;
        json step;

        for (int i = 0; i < n; i++) {
            if (!finished[i]) {
                bool canRun = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canRun = false;
                        break;
                    }
                }

                if (canRun) {
                    for (int j = 0; j < m; j++) work[j] += allocation[i][j];
                    finished[i] = true;
                    safeSeq.push_back(i);
                    completed++;
                    progress = true;

                    step["process"] = i;
                    step["action"] = "executed";
                    step["work"] = work;
                    steps.push_back(step);
                }
            }
        }

        if (!progress) {
            // Deadlock
            step["action"] = "deadlock";
            steps.push_back(step);
            break;
        }

        iteration++;
    }

    // Write steps to JSON file
    ofstream fout("visualizer/banker_steps.json");
    fout << steps.dump(4);
    fout.close();

    cout << "\n✅ JSON steps saved to visualizer/banker_steps.json\n";
}
