/**
 * @file part1.cpp
 * @brief Cafeteria Ingredient Freshness Checker
 *
 * In the newly rediscovered cafeteria, the Elves are overwhelmed after
 * switching to a new inventory management system right before Christmas. This
 * program helps them determine which ingredient IDs are still fresh.
 *
 * The database (puzzle input) is structured into two sections:
 * - A list of *fresh ingredient ID ranges*, each in the form "A-B" (inclusive).
 * - A blank line.
 * - A list of *available ingredient IDs* to evaluate.
 *
 * An ingredient ID is considered **fresh** if it falls within *any* of the
 * given fresh ID ranges. Ranges may overlap, and any overlapping or redundant
 * ranges should still be treated independently.
 *
 * Example:
 * Fresh Ranges:
 * 3-5
 * 10-14
 * 16-20
 * 12-18
 *
 * Available IDs:
 * 1   // spoiled
 * 5   // fresh (within 3-5)
 * 8   // spoiled
 * 11  // fresh (within 10-14)
 * 17  // fresh (within 16-20 and 12-18)
 * 32  // spoiled
 *
 * The task is to process all available ingredient IDs and count how many of
 * them are fresh by checking whether each ID falls inside at least one of the
 * ranges.
 *
 * The program outputs the total number of fresh ingredient IDs.
 *
 * Puzzle Answer: [Number of Fresh Ingredient IDs]
 *
 * @author [gabrielzschmitz]
 * @date [05/12/2025]
 */
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

  int64_t total = 0;

  for (int64_t id : ids) {
    bool fresh = false;

    for (auto &r : ranges) {
      if (id >= r.first && id <= r.second) {
        fresh = true;
        break;
      }
    }

    if (fresh)
      total++;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
