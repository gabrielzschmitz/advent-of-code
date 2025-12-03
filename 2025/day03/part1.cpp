/**
 * @file part1.cpp
 * @brief Emergency Escalator Power Routing
 *
 * The lobby escalator is offline and requires emergency power. Nearby,
 * several banks of batteries can be used to supply joltage to activate it.
 * Each bank consists of a sequence of digits (1–9), each representing the
 * joltage rating of a single battery. Each line of the puzzle input is
 * one such battery bank.
 *
 * From each bank, **exactly two batteries must be turned on**, and the
 * resulting joltage is formed by concatenating the digits of those two
 * batteries **in the order they appear** within the bank. For example:
 *
 *   - Bank: 12345
 *     Turning on batteries 2 and 4 -> "24" → 24 jolts.
 *
 * The batteries cannot be rearranged, and only pairs of batteries whose
 * positions increase (i < j) are valid.
 *
 * The task is to compute, for each bank:
 *   1. All valid two-digit joltages created by selecting any pair of digits.
 *   2. The largest possible joltage obtainable from that bank.
 *
 * After computing the maximum joltage for each bank, sum all of these
 * per-bank maxima to determine the total output joltage the emergency
 * batteries can provide.
 *
 * Example:
 *   - 987654321111111 → max = 98
 *   - 811111111111119 → max = 89
 *   - 234234234234278 → max = 78
 *   - 818181911112111 → max = 92
 *
 * Total example output: 98 + 89 + 78 + 92 = 357.
 *
 * Given the full puzzle input, this program computes the sum of the
 * maximum per-bank joltages.
 *
 * *Puzzle Answer: [Total Output Joltage]*
 *
 * @author [gabrielzschmitz]
 * @date [03/12/2025]
 */
#include <bits/stdc++.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test")
    debug = true;
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  int64_t total = 0;
  std::string line;
  while (std::getline(input_file, line)) {
    int best = 0;
    int best_left_digit = -1;

    for (char c : line) {
      int d = c - '0';

      if (best_left_digit != -1) {
        int candidate = best_left_digit * 10 + d;
        if (candidate > best)
          best = candidate;
      }

      if (d > best_left_digit)
        best_left_digit = d;
    }

    if (debug)
      std::cout << "largest joltage: " << best << std::endl;

    total += best;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
