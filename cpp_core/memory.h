#ifndef MEMORY_H
#define MEMORY_H

#include "json_utils.h"
#include <algorithm>
#include <deque>
#include <map>
#include <string>
#include <vector>


class MemoryManager {
private:
  int num_frames;
  std::vector<int> references;
  std::string algorithm; // FIFO, LRU, OPTIMAL

public:
  MemoryManager(int frames, const std::vector<int> &refs, std::string algo);
  JsonValue simulate();

  // Algorithm helpers
  void runFIFO(JsonValue &result, JsonValue &steps);
  void runLRU(JsonValue &result, JsonValue &steps);
  void runOptimal(JsonValue &result, JsonValue &steps);
};

#endif
