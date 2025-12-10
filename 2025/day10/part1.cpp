/**
 * @file part1.cpp
 * @brief Largest Red-Corner Rectangle
 *
 * Puzzle Answer: [Largest Rectangle Area]
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

    // -------------------------------
    // PARSE INDICATOR LIGHT PATTERN
    // -------------------------------
    size_t l = line.find('[');
    size_t r = line.find(']');
    std::string pattern = line.substr(l + 1, r - l - 1);

    int n = pattern.size();
    int target = 0;
    for (int i = 0; i < n; i++) {
      if (pattern[i] == '#')
        target |= (1 << i);
    }

    // -------------------------------
    // PARSE BUTTONS (PARENTHESES)
    // -------------------------------
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

    // --------------------------------
    // BRUTE FORCE ALL SUBSETS OF BUTTONS
    // --------------------------------
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
