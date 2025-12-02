/**
 * @file part1.cpp
 * @brief North Pole Gift Shop ID Validation
 *
 * This program assists the North Pole gift shop clerks in identifying
 * invalid product IDs that were accidentally added to the shop’s
 * database by a mischievous young Elf.
 *
 * Product IDs are listed as a set of ranges. Each range is provided in
 * the form:
 *
 *      A-B
 *
 * where A is the first product ID of the range and B is the last.
 * Ranges are separated by commas on a single long line.
 *
 * A product ID is considered **invalid** if it consists of a sequence
 * of digits repeated exactly twice. Examples of invalid IDs include:
 *
 * - 11   → "1" repeated twice
 * - 22   → "2" repeated twice
 * - 6464 → "64" repeated twice
 * - 123123 → "123" repeated twice
 *
 * Valid IDs never have leading zeroes; for example, "0101" is not an ID.
 *
 * The task is to:
 *  1. Parse all provided ID ranges.
 *  2. Determine which product IDs inside those ranges match the
 *     repeated-pattern rule.
 *  3. Sum all invalid IDs and output the final result.
 *
 * Example:
 *   In the range 11-22, the invalid IDs are 11 and 22.
 *   In 95-115, the invalid ID is 99.
 *   In 998-1012, the invalid ID is 1010.
 *   (And so on.)
 *
 * Given the full puzzle input, this program computes the sum of all
 * invalid product IDs that appear within the listed ranges.
 *
 * *Puzzle Answer: [Sum of All Invalid IDs]*
 *
 * @author [gabrielzschmitz]
 * @date [02/12/2025]
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

bool isInvalid(int64_t x) {
  std::string s = std::to_string(x);
  if (s.size() % 2 != 0)
    return false;
  int half = s.size() / 2;
  return s.substr(0, half) == s.substr(half);
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
  std::getline(input_file, line);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::stringstream ss(line);
  std::string token;

  while (std::getline(ss, token, ',')) {
    if (token.empty())
      continue;

    auto dash = token.find('-');
    if (dash == std::string::npos)
      continue;

    int64_t L = std::stoll(token.substr(0, dash));
    int64_t R = std::stoll(token.substr(dash + 1));

    for (int64_t x = L; x <= R; x++) {
      if (isInvalid(x)) {
        total += x;
      }
    }
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
