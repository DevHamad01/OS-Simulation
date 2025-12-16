#ifndef DEADLOCK_H
#define DEADLOCK_H

#include "json_utils.h"
#include <map>
#include <set>
#include <string>
#include <vector>


class DeadlockDetector {
private:
  std::vector<std::string> processes;
  std::vector<std::string> resources;
  std::map<std::string, std::vector<std::string>> adj; // Adjacency list
  std::vector<std::string> cycle_path;
  bool has_cycle;

  // DFS helper
  bool dfs(const std::string &u, std::set<std::string> &visited,
           std::set<std::string> &recursion_stack,
           std::vector<std::string> &path);

public:
  DeadlockDetector();
  void addEdge(const std::string &from, const std::string &to);
  JsonValue detect(); // Returns deadlock status and cycle if any
};

#endif
