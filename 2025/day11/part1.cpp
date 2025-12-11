/**
 * @file part1.cpp
 * @brief Enumerating All Data Paths Through the Reactor Network
 *
 * The factory's toroidal reactor communicates with a nearby server rack through
 * a network of interconnected devices. Each device may forward data to one or
 * more other devices, forming a directed acyclic graph in which data always
 * flows forward along defined output connections.
 *
 * The engineers suspect that a faulty data path—not a specific device—is
 * responsible for the reactor’s communication problems. To isolate the issue,
 * they need to understand every possible route data may take from the device
 * labeled **"you"** to the reactor’s main output device labeled **"out"**.
 *
 * The puzzle input describes the network: each line defines a device and the
 * list of devices receiving output from it. For example:
 *
 *     bbb: ddd eee
 *
 * means that device `bbb` forwards data to devices `ddd` and `eee`.
 *
 * The task:
 *   1. Parse the directed adjacency list describing all devices.
 *   2. Starting at device `"you"`, perform a complete traversal of the network.
 *   3. Enumerate every distinct path that ultimately leads to device `"out"`.
 *   4. Count the total number of such paths.
 *
 * Example:
 *   For the provided example network, there are exactly **5** unique paths
 *   leading from `you` to `out`.
 *
 * Puzzle Answer: [Total Number of Paths from 'you' to 'out']
 *
 * @author [gabrielzschmitz]
 * @date [11/12/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test")
    debug = true;
}

static inline int popcount(int x) { return __builtin_popcount(x); }

using Graph = std::unordered_map<std::string, std::vector<std::string>>;

int64_t dfs_count(const Graph &g, const std::string &cur,
                  const std::string &target) {
  if (cur == target)
    return 1; // stop immediately

  auto it = g.find(cur);
  if (it == g.end())
    return 0;

  int64_t count = 0;
  for (const auto &nx : it->second) {
    count += dfs_count(g, nx, target);
  }
  return count;
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  Graph graph;
  std::string line;

  // Parse: "aaa: you hhh"
  while (getline(input_file, line)) {
    if (line.empty())
      continue;

    std::istringstream iss(line);
    std::string src;
    if (!(iss >> src))
      continue;

    // remove trailing ':' from source
    if (src.back() == ':')
      src.pop_back();

    std::string dst;
    while (iss >> dst) {
      graph[src].push_back(dst);
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  // Compute number of paths from "you" to "out"
  int64_t total = dfs_count(graph, "you", "out");

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
