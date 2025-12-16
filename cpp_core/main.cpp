#include "json_utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


#include "deadlock.h"
#include "memory.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <input.json> <output.json>"
              << std::endl;
    return 1;
  }

  std::string input_file = argv[1];
  std::string output_file = argv[2];

  try {
    JsonValue input = JsonParser::parseFile(input_file);
    std::string mode;
    if (input["mode"].type == JsonValue::STRING) {
      mode = input["mode"].string_val;
    } else {
      std::cerr << "Error: 'mode' field missing or invalid in input JSON."
                << std::endl;
      return 1;
    }

    JsonValue output = JsonValue::object();

    if (mode == "BANKER") {
      JsonValue banker_data = input["banker"];
      int p = (int)banker_data["processes"].number_val;
      int r = (int)banker_data["resources"].number_val;

      std::vector<int> avail;
      for (size_t i = 0; i < banker_data["available"].array_val.size(); i++)
        avail.push_back((int)banker_data["available"][i].number_val);

      std::vector<std::vector<int>> max_mat;
      for (size_t i = 0; i < banker_data["max"].array_val.size(); i++) {
        std::vector<int> r_row;
        for (size_t j = 0; j < banker_data["max"][i].array_val.size(); j++)
          r_row.push_back((int)banker_data["max"][i][j].number_val);
        max_mat.push_back(r_row);
      }

      std::vector<std::vector<int>> alloc_mat;
      for (size_t i = 0; i < banker_data["allocation"].array_val.size(); i++) {
        std::vector<int> r_row;
        for (size_t j = 0; j < banker_data["allocation"][i].array_val.size();
             j++)
          r_row.push_back((int)banker_data["allocation"][i][j].number_val);
        alloc_mat.push_back(r_row);
      }

      BankerAlgorithm banker(p, r, avail, max_mat, alloc_mat);
      output = banker.solve();

    } else if (mode == "DEADLOCK") {
      DeadlockDetector detector;
      JsonValue dd_data = input["deadlock"];
      JsonValue edges = dd_data["edges"];

      for (size_t i = 0; i < edges.array_val.size(); i++) {
        std::string u = edges[i]["from"].string_val;
        std::string v = edges[i]["to"].string_val;
        detector.addEdge(u, v);
      }
      output = detector.detect();

    } else if (mode == "MEMORY") {
      JsonValue mem_data = input["memory"];
      int frames = (int)mem_data["frames"].number_val;
      std::string algo = mem_data["algorithm"].string_val;
      std::vector<int> refs;

      for (size_t i = 0; i < mem_data["references"].array_val.size(); i++) {
        refs.push_back((int)mem_data["references"][i].number_val);
      }

      MemoryManager mgr(frames, refs, algo);
      output = mgr.simulate();

    } else {
      output["error"] = "Unknown mode: " + mode;
    }

    std::ofstream out(output_file);
    out << output.dump(4); // 4 spaces indent
    out.close();

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
