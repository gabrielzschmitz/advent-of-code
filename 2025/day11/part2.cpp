/**
 * @file part2.cpp
 * @brief Enumerating Constrained Data Paths Through the Reactor Network
 *
 * After cataloging all possible data routes through the reactor’s device
 * network, the engineering team has discovered additional constraints regarding
 * the problematic communication path. Specifically, the faulty behavior must
 * occur along a path that passes through *both* of the following devices:
 *
 *   - `dac` : a digital-to-analog converter
 *   - `fft` : a fast Fourier transform processor
 *
 * The objective of this task is therefore more restrictive than in Part One.
 *
 * Problem Summary:
 *   - The puzzle input again defines a forward-only device network via
 *     adjacency lists, where each device lists the devices it outputs to.
 *   - This time, analysis must begin at device `"svr"` (the server rack).
 *   - Data ultimately flows toward the device `"out"` (the main reactor
 * output).
 *   - A path is only considered valid if it:
 *       1. Begins at `svr`,
 *       2. Ends at `out`, and
 *       3. Includes *both* `dac` and `fft` somewhere along the path, in any
 * order.
 *
 * The task:
 *   1. Parse the directed graph describing all device connections.
 *   2. Enumerate every path from `svr` to `out`.
 *   3. Count only the subset of those paths that visit both `dac` and `fft`.
 *
 * Example:
 *   In the provided example network, there are eight total paths from
 *   `svr` to `out`, but only **2** of them include both required devices.
 *
 * Puzzle Answer:
 *   [Number of Paths from 'svr' to 'out' Containing Both 'dac' and 'fft']
 *
 * @author [gabrielzschmitz]
 * @date [11/12/2025]
 */
#include <array>
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
  std::string last = (pos == std::string::npos ? input : input.substr(pos + 1));
  if (last == "test" || last == "test2")
    debug = true;
}

// Convert ".../test" → ".../test2"
std::string normalizeTestPath(std::string path) {
  if (path.size() >= 4 && path.substr(path.size() - 4) == "test")
    return path + "2";
  if (path.size() >= 5 && path.substr(path.size() - 5) == "/test")
    return path + "2";
  return path;
}

using Graph = std::unordered_map<std::string, std::vector<std::string>>;
using Memo = std::unordered_map<std::string, std::array<int64_t, 4>>;

int64_t dfs(const Graph &g, Memo &memo, const std::string &cur,
            const std::string &target, bool seen_dac, bool seen_fft) {
  if (cur == "dac")
    seen_dac = true;
  if (cur == "fft")
    seen_fft = true;

  int state = (seen_dac ? 2 : 0) | (seen_fft ? 1 : 0);
  auto &entry = memo[cur];

  if (entry[state] != -1) {
    if (debug) {
      if (entry[state] == 1)
        std::cerr << "[MEMO HIT]  cur=" << cur << " st=" << state
                  << " ret=1 <- valid path\n";
      else
        std::cerr << "[MEMO HIT]  cur=" << cur << " st=" << state << " ret=0\n";
    }
    return entry[state];
  }

  if (cur == target) {
    int64_t ret = (seen_dac && seen_fft) ? 1 : 0;
    entry[state] = ret;

    if (debug) {
      if (ret == 1)
        std::cerr << "[TARGET HIT] cur=" << cur << " st=" << state
                  << " ret=1 <- valid path\n";
      else
        std::cerr << "[TARGET HIT] cur=" << cur << " st=" << state
                  << " ret=0\n";
    }

    return ret;
  }

  auto it = g.find(cur);
  if (it == g.end())
    return entry[state] = 0;

  int64_t count = 0;
  for (const auto &nx : it->second)
    count += dfs(g, memo, nx, target, seen_dac, seen_fft);

  entry[state] = count;
  return count;
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";

  user_input = normalizeTestPath(user_input);

  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  Graph graph;
  std::string line;

  while (getline(input_file, line)) {
    if (line.empty())
      continue;

    std::istringstream iss(line);
    std::string src;
    if (!(iss >> src))
      continue;

    if (src.back() == ':')
      src.pop_back();

    std::string dst;
    while (iss >> dst)
      graph[src].push_back(dst);
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  Memo memo;
  for (auto &p : graph)
    memo[p.first] = {-1, -1, -1, -1};
  memo["out"] = {-1, -1, -1, -1};

  int64_t total = dfs(graph, memo, "svr", "out", false, false);

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
