/**
 * @file part2.cpp
 * @brief North Pole Gift Shop Extended ID Validation
 *
 * After identifying the first batch of invalid product IDs, the clerks
 * discover that the mischievous young Elf introduced even more unusual
 * patterns into the gift shop database. This second part expands the
 * validation rules to detect a wider class of invalid IDs.
 *
 * Product IDs appear in ranges formatted as:
 *
 *      A-B
 *
 * where A is the first ID and B is the last. Ranges are separated by
 * commas and listed on a single continuous line.
 *
 * In this extended validation method, a product ID is considered
 * **invalid** if it consists solely of a sequence of digits repeated
 * **two or more times**. Examples of invalid IDs include:
 *
 * - 12341234      → "1234" repeated twice
 * - 123123123     → "123" repeated three times
 * - 1212121212    → "12" repeated five times
 * - 1111111       → "1" repeated seven times
 *
 * As before, IDs never contain leading zeroes.
 *
 * The task is to:
 *  1. Parse all provided ID ranges.
 *  2. For each ID, determine whether it is made entirely from a repeated
 *     digit-sequence of length L (L >= 1), repeated at least twice.
 *  3. Sum all such invalid IDs and output the resulting total.
 *
 * Example (from the given ranges):
 *   - 11-22 contains the invalid IDs 11 and 22.
 *   - 95-115 contains 99 and 111.
 *   - 998-1012 contains 999 and 1010.
 *   - 565653-565659 contains 565656.
 *   - 824824821-824824827 contains 824824824.
 *   - 2121212118-2121212124 contains 2121212121.
 *
 * Using these extended rules, the example input produces:
 *   4174379265
 *
 * Given the full puzzle input, this program computes the sum of all
 * product IDs which consist of a digit-sequence repeated two or more times.
 *
 * *Puzzle Answer: [Sum of All Invalid IDs — Part 2]*
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
  int n = s.size();

  for (int d = 1; d <= n / 2; d++) {
    if (n % d != 0)
      continue;

    std::string chunk = s.substr(0, d);
    bool ok = true;

    for (int i = d; i < n; i += d) {
      if (s.compare(i, d, chunk) != 0) {
        ok = false;
        break;
      }
    }

    if (ok)
      return true;
  }

  return false;
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
