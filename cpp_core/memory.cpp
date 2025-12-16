#include "memory.h"
#include <iostream>

MemoryManager::MemoryManager(int frames, const std::vector<int> &refs,
                             std::string algo)
    : num_frames(frames), references(refs), algorithm(algo) {}

JsonValue MemoryManager::simulate() {
  JsonValue result = JsonValue::object();
  JsonValue steps = JsonValue::array();

  if (algorithm == "FIFO") {
    runFIFO(result, steps);
  } else if (algorithm == "LRU") {
    runLRU(result, steps);
  } else if (algorithm == "OPTIMAL") {
    runOptimal(result, steps);
  } else {
    result["error"] = "Unknown algorithm: " + algorithm;
  }

  result["steps"] = steps;
  return result;
}

void MemoryManager::runFIFO(JsonValue &result, JsonValue &steps) {
  std::deque<int> frames;
  int page_faults = 0;
  int hits = 0;

  for (int page : references) {
    JsonValue step = JsonValue::object();
    step["page"] = page;

    bool found = false;
    for (int f : frames) {
      if (f == page) {
        found = true;
        break;
      }
    }

    if (found) {
      hits++;
      step["status"] = "HIT";
    } else {
      page_faults++;
      step["status"] = "MISS";
      if (frames.size() < num_frames) {
        frames.push_back(page);
      } else {
        frames.pop_front();
        frames.push_back(page);
      }
    }

    JsonValue f_arr = JsonValue::array();
    for (int f : frames)
      f_arr.push_back(f);
    step["frames"] = f_arr;
    steps.push_back(step);
  }
  result["page_faults"] = page_faults;
  result["hits"] = hits;
}

void MemoryManager::runLRU(JsonValue &result, JsonValue &steps) {
  std::vector<int> frames;
  std::map<int, int> last_used; // page -> timestamp
  int time = 0;
  int page_faults = 0;
  int hits = 0;

  for (int page : references) {
    time++;
    JsonValue step = JsonValue::object();
    step["page"] = page;

    bool found = false;
    for (int f : frames) {
      if (f == page) {
        found = true;
        break;
      }
    }

    if (found) {
      hits++;
      step["status"] = "HIT";
      last_used[page] = time;
    } else {
      page_faults++;
      step["status"] = "MISS";
      if (frames.size() < num_frames) {
        frames.push_back(page);
      } else {
        // Find LRU
        int lru_page = -1;
        int min_time = 2147483647;

        for (int f : frames) {
          if (last_used[f] < min_time) {
            min_time = last_used[f];
            lru_page = f;
          }
        }

        // Replace
        for (size_t i = 0; i < frames.size(); i++) {
          if (frames[i] == lru_page) {
            frames[i] = page;
            break;
          }
        }
      }
      last_used[page] = time;
    }

    JsonValue f_arr = JsonValue::array();
    for (int f : frames)
      f_arr.push_back(f);
    step["frames"] = f_arr;
    steps.push_back(step);
  }
  result["page_faults"] = page_faults;
  result["hits"] = hits;
}

void MemoryManager::runOptimal(JsonValue &result, JsonValue &steps) {
  std::vector<int> frames;
  int page_faults = 0;
  int hits = 0;

  for (size_t i = 0; i < references.size(); i++) {
    int page = references[i];
    JsonValue step = JsonValue::object();
    step["page"] = page;

    bool found = false;
    for (int f : frames) {
      if (f == page) {
        found = true;
        break;
      }
    }

    if (found) {
      hits++;
      step["status"] = "HIT";
    } else {
      page_faults++;
      step["status"] = "MISS";
      if (frames.size() < num_frames) {
        frames.push_back(page);
      } else {
        // Find Optimal Replacement
        int farthest_idx = -1;
        int pg_to_replace = -1;
        int replace_idx_in_frames = -1;

        for (size_t f_idx = 0; f_idx < frames.size(); f_idx++) {
          int fr = frames[f_idx];
          int next_use = -1;
          for (size_t k = i + 1; k < references.size(); k++) {
            if (references[k] == fr) {
              next_use = k;
              break;
            }
          }

          if (next_use == -1) {
            // Not used again, best candidate
            pg_to_replace = fr;
            replace_idx_in_frames = f_idx;
            break;
          } else {
            if (next_use > farthest_idx) {
              farthest_idx = next_use;
              pg_to_replace = fr;
              replace_idx_in_frames = f_idx;
            }
          }
        }

        // If we didn't find one that is never used again (break didn't hit),
        // replace the farthest If replace_idx_in_frames is still -1 (shouldn't
        // happen if logic is correct), default 0
        if (replace_idx_in_frames == -1)
          replace_idx_in_frames = 0;

        frames[replace_idx_in_frames] = page;
      }
    }

    JsonValue f_arr = JsonValue::array();
    for (int f : frames)
      f_arr.push_back(f);
    step["frames"] = f_arr;
    steps.push_back(step);
  }
  result["page_faults"] = page_faults;
  result["hits"] = hits;
}
