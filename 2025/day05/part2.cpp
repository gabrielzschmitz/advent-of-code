/**
 * @file part2.cpp
 * @brief Cafeteria Fresh ID Range Expansion
 *
 * After determining which available ingredient IDs are fresh, the Elves now
 * want a complete list of *all* ingredient IDs that are considered fresh based
 * solely on the fresh ID ranges recorded in their new inventory system.
 *
 * In this second part of the puzzle, the section containing *available
 * ingredient IDs* is no longer relevant. Instead, the task focuses only on the
 * fresh ID ranges.
 *
 * Each fresh range is given in the form "A-B" and is inclusive. Ranges may
 * overlap or fully contain one another, and any ID that appears in *any* range
 * is considered fresh.
 *
 * To determine how many ingredient IDs are fresh in total:
 * 1. Read the list of fresh ID ranges.
 * 2. Merge or account for overlapping ranges.
 * 3. Count every unique ID covered by any range.
 *
 * Example:
 * Fresh Ranges:
 * 3-5
 * 10-14
 * 16-20
 * 12-18
 *
 * These ranges collectively include the following fresh ingredient IDs:
 * 3, 4, 5,
 * 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
 *
 * In total, 14 unique IDs are considered fresh by the set of ranges.
 *
 * The program processes the range definitions and outputs the total number of
 * distinct ingredient IDs that fall within any fresh ID range.
 *
 * Puzzle Answer: [Total Fresh IDs According to Ranges]
 *
 * @author [gabrielzschmitz]
 * @date [05/12/2025]
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

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::pair<int64_t, int64_t>> ranges;
  std::vector<int64_t> ids;

  std::string line;
  bool reading_ranges = true;

  while (std::getline(input_file, line)) {
    if (line.empty()) {
      reading_ranges = false;
      continue;
    }

    if (reading_ranges) {
      std::stringstream ss(line);
      int64_t a, b;
      char dash;
      ss >> a >> dash >> b;
      ranges.emplace_back(a, b);
    } else {
      ids.emplace_back(std::stoll(line));
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  if (ranges.empty()) {
    std::cout << "ANSWER: 0" << std::endl;
    return 0;
  }

  std::sort(ranges.begin(), ranges.end(),
            [](const auto &l, const auto &r) { return l.first < r.first; });

  std::vector<std::pair<int64_t, int64_t>> merged;
  merged.push_back(ranges[0]);

  for (size_t i = 1; i < ranges.size(); i++) {
    auto &last = merged.back();
    auto &cur = ranges[i];

    if (cur.first <= last.second + 1)
      last.second = std::max(last.second, cur.second);
    else
      merged.push_back(cur);
  }

  int64_t total = 0;
  for (auto &r : merged) {
    total += (r.second - r.first + 1);
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
