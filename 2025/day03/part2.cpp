/**
 * @file part2.cpp
 * @brief High-Capacity Escalator Power Routing
 *
 * After providing the initial joltage, the escalator still refuses to move.
 * The Elf activates the emergency "joltage limit safety override", enabling
 * the system to draw significantly more power. With the override active,
 * each battery bank must now contribute a much larger joltage.
 *
 * As before, each line of the puzzle input represents a bank of batteries,
 * where each digit (1–9) is the joltage rating of a single battery.
 *
 * However, the rules have changed:
 *   - You must activate **exactly twelve batteries** from each bank.
 *   - The joltage produced by a bank is the 12-digit number formed by
 *     concatenating the digits of the activated batteries **in their
 *     original order**.
 *
 * The goal for each bank is therefore to select the **lexicographically
 * largest possible 12-digit subsequence**. Batteries cannot be reordered —
 * the relative order in the bank must be preserved.
 *
 * After determining the maximum possible 12-digit joltage for each bank,
 * sum all of these per-bank values to obtain the total escalator power
 * output.
 *
 * Example:
 *   Input banks:
 *     987654321111111
 *     811111111111119
 *     234234234234278
 *     818181911112111
 *
 *   Largest valid joltages:
 *     987654321111
 *     811111111119
 *     434234234278
 *     888911112111
 *
 *   Total example output:
 *     987654321111
 *   + 811111111119
 *   + 434234234278
 *   + 888911112111
 *   = 3121910778619
 *
 * Given the full puzzle input, this program computes the sum of all
 * maximum 12-digit joltages formed from each battery bank.
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
  constexpr int K = 12;

  while (std::getline(input_file, line)) {
    const int n = line.size();

    std::string best;
    best.reserve(K);

    int idx = 0;

    for (int taken = 0; taken < K; taken++) {
      int remaining_picks = K - taken;
      int max_pos = n - remaining_picks;

      char best_digit = '0';
      int best_digit_pos = idx;

      for (int i = idx; i <= max_pos; i++) {
        if (line[i] > best_digit) {
          best_digit = line[i];
          best_digit_pos = i;
        }
      }

      best.push_back(best_digit);
      idx = best_digit_pos + 1;
    }

    if (debug)
      std::cout << "largest joltage: " << best << std::endl;

    total += std::stoll(best);
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
