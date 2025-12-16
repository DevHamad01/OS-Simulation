#include <iostream>
#include "banker.h"
#include "deadlock.h"
#include "memory.h"

using namespace std;

int main() {
    int choice;

    cout << "========== OS SIMULATOR ==========\n";
    cout << "1. Banker's Algorithm\n";
    cout << "2. Deadlock Detection\n";
    cout << "3. Memory Management / Page Replacement\n";
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
    case 1:
        runBankersAlgorithm();
        break;
    case 2:
        runDeadlockDetection();
        break;
    case 3:
        runPageReplacementFromFile();
        break;
    default:
        cout << "Invalid choice!\n";
    }

    cout << "\nPress ENTER to exit...";
    cin.ignore();
    cin.get();
    return 0;
}
