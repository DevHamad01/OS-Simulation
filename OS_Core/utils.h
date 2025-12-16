#pragma once
#include <string>
#include <vector>

inline void parseInput(const std::string& line, std::vector<int>& row, int m) {
    row.clear();
    if (line.find(' ') != std::string::npos) {
        std::string temp = "";
        for (char c : line) {
            if (c == ' ') {
                if (!temp.empty()) {
                    row.push_back(std::stoi(temp));
                    temp = "";
                }
            }
            else temp += c;
        }
        if (!temp.empty()) row.push_back(std::stoi(temp));
    }
    else {
        for (char c : line)
            row.push_back(c - '0');
    }
}
