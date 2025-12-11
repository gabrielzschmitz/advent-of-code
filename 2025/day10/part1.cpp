/**
 * @file part1.cpp
 * @brief Minimal-Press Configuration of Factory Indicator Light Systems
 *
 * Inside the North Pole factory, each machine must be initialized by
 * configuring its indicator lights to match a target pattern. The
 * initialization procedure has been lost, but the remaining manual pages
 * provide:
 *
 *   - An indicator light diagram: the required final on/off state for each
 * light.
 *   - A list of button wiring schematics: each button toggles specific lights.
 *   - A joltage requirement list: irrelevant to initialization and ignored.
 *
 * Each machine:
 *   - Begins with all indicator lights in the OFF state.
 *   - Defines a desired final pattern using '.' (off) and '#' (on).
 *   - Provides one or more buttons, each toggling a subset of lights indexed
 *     from 0 to N-1.
 *
 * Pressing a button toggles every listed light (ON→OFF, OFF→ON). Buttons may be
 * pressed any non-negative integer number of times, and pressing a button twice
 * is equivalent to pressing it zero times (because toggling twice restores the
 * original state). As a result, each button is effectively a binary decision:
 *
 *        press it (1) or do not press it (0).
 *
 * The problem for each machine is therefore equivalent to solving a system of
 * linear equations over GF(2), where:
 *
 *   - Each button defines a vector of toggles.
 *   - The target pattern defines the required parity of toggles for each light.
 *
 * Objective:
 *   For each machine, determine the *minimum total number of button presses*
 *   required to reach the target light configuration.
 *
 * Example Summary:
 *   - Machine 1 requires a minimum of 2 presses.
 *   - Machine 2 requires a minimum of 3 presses.
 *   - Machine 3 requires a minimum of 2 presses.
 *
 * Summing across all machines gives the final answer for the puzzle.
 *
 * Puzzle Answer: [Total Minimum Button Press Count]
 *
 * @author [gabrielzschmitz]
 * @date [10/12/2025]
 */
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string line;
  int64_t total = 0;

  while (getline(input_file, line)) {
    if (line.empty())
      continue;

    size_t l = line.find('[');
    size_t r = line.find(']');
    std::string pattern = line.substr(l + 1, r - l - 1);

    int n = pattern.size();
    int target = 0;
    for (int i = 0; i < n; i++) {
      if (pattern[i] == '#')
        target |= (1 << i);
    }

    std::vector<int> buttons;
    size_t pos = r + 1;

    while (true) {
      size_t l2 = line.find('(', pos);
      if (l2 == std::string::npos)
        break;
      size_t r2 = line.find(')', l2);
      if (r2 == std::string::npos)
        break;

      std::string inside = line.substr(l2 + 1, r2 - l2 - 1);
      if (inside.find(',') != std::string::npos || !inside.empty()) {
        int mask = 0;
        if (!inside.empty()) {
          std::stringstream ss(inside);
          std::string num;
          while (getline(ss, num, ',')) {
            int bit = std::stoi(num);
            mask |= (1 << bit);
          }
        }
        buttons.push_back(mask);
      }
      pos = r2 + 1;
    }

    int B = buttons.size();
    int best = INT32_MAX;

    for (int subset = 0; subset < (1 << B); subset++) {
      int state = 0;
      for (int i = 0; i < B; i++) {
        if (subset & (1 << i))
          state ^= buttons[i];
      }
      if (state == target) {
        int presses = popcount(subset);
        best = std::min(best, presses);
      }
    }

    if (best == INT32_MAX) {
      std::cerr << "ERROR: no solution found for line:\n" << line << "\n";
      return 1;
    }

    total += best;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
