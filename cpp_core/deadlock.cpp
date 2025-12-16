#include "deadlock.h"
#include <algorithm>
#include <iostream>


DeadlockDetector::DeadlockDetector() : has_cycle(false) {}

void DeadlockDetector::addEdge(const std::string &from, const std::string &to) {
  adj[from].push_back(to);
  // Ensure nodes exist in adjacency map even if they have no outgoing edges
  if (adj.find(to) == adj.end()) {
    adj[to] = std::vector<std::string>();
  }
}

bool DeadlockDetector::dfs(const std::string &u, std::set<std::string> &visited,
                           std::set<std::string> &recursion_stack,
                           std::vector<std::string> &path) {
  visited.insert(u);
  recursion_stack.insert(u);
  path.push_back(u);

  for (const auto &v : adj[u]) {
    if (recursion_stack.count(v)) {
      // Cycle detected
      // Capture the cycle part of the path
      auto it = std::find(path.begin(), path.end(), v);
      while (it != path.end()) {
        cycle_path.push_back(*it);
        it++;
      }
      cycle_path.push_back(v); // Close the loop
      return true;
    }
    if (visited.find(v) == visited.end()) {
      if (dfs(v, visited, recursion_stack, path))
        return true;
    }
  }

  path.pop_back();
  recursion_stack.erase(u);
  return false;
}

JsonValue DeadlockDetector::detect() {
  std::set<std::string> visited;
  std::set<std::string> recursion_stack;
  std::vector<std::string> path;
  has_cycle = false;
  cycle_path.clear();

  for (const auto &pair : adj) {
    if (visited.find(pair.first) == visited.end()) {
      if (dfs(pair.first, visited, recursion_stack, path)) {
        has_cycle = true;
        break;
      }
    }
  }

  JsonValue result = JsonValue::object();
  result["deadlock"] = has_cycle;

  JsonValue cycle_arr = JsonValue::array();
  if (has_cycle) {
    for (const auto &node : cycle_path) {
      cycle_arr.push_back(node);
    }
  }
  result["cycle"] = cycle_arr;

  return result;
}
