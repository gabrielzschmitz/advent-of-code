/**
 * @file part1.cpp
 * @brief Towel Pattern Matching for Onsen Designs
 *
 * This program helps arrange towels with specific patterns to create desired 
 * designs for an onsen. Each towel has a sequence of colored stripes, and 
 * the objective is to determine how many of the requested designs can be 
 * formed using the available towel patterns.
 *
 * **Challenge Description:**
 * - Towels are marked with a pattern of colored stripes, where each stripe 
 *   can be white (w), blue (u), black (b), red (r), or green (g).
 * - Each requested design consists of a sequence of stripes that must be 
 *   matched exactly using one or more towels from the available patterns.
 * - Towels cannot be flipped, and the sequence must be matched in order.
 *
 * **Task:**
 * For a given list of available towel patterns and requested designs:
 * 1. Determine if each design can be constructed using the patterns.
 * 2. Count the number of designs that are possible to create.
 *
 * **Example Input:**
 * ```
 * r, wr, b, g, bwu, rb, gb, br
 * brwrr
 * bggr
 * gbbr
 * rrbgbr
 * ubwu
 * bwurrg
 * brgr
 * bbrgwb
 * ```
 *
 * **Example Output:**
 * ```
 * Possible Designs: 6
 * Impossible Designs: 2
 * ```
 *
 * **Steps to Solve:**
 * 1. Parse the input to separate towel patterns and requested designs.
 * 2. Use a recursive or dynamic programming approach to test if each 
 *    design can be constructed from the available patterns.
 * 3. Output the total count of possible designs.
 *
 * **Constraints:**
 * - Unlimited towels are available for each pattern.
 * - Designs must be constructed exactly as specified.
 *
 * **Puzzle Answer:** [Number of Possible Designs]
 *
 * @author [gabrielzschmitz]
 * @date [27/12/2024]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

bool canFormDesign(const std::string& design,
                   const std::unordered_set<std::string>& patterns,
                   std::unordered_map<std::string, bool>& memo) {
  if (design.empty()) return true;
  if (memo.find(design) != memo.end()) return memo[design];

  for (const auto& pattern : patterns) {
    if (design.find(pattern) == 0) {
      if (canFormDesign(design.substr(pattern.size()), patterns, memo)) {
        memo[design] = true;
        return true;
      }
    }
  }
  memo[design] = false;
  return false;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string patterns_line;
  std::getline(input_file, patterns_line);

  std::unordered_set<std::string> patterns;
  size_t pos = 0;
  while ((pos = patterns_line.find(", ")) != std::string::npos) {
    patterns.insert(patterns_line.substr(0, pos));
    patterns_line.erase(0, pos + 2);
  }
  patterns.insert(patterns_line);
  std::string skip;
  std::getline(input_file, skip);

  std::vector<std::string> designs;
  std::string design;
  while (std::getline(input_file, design))
    if (!design.empty()) designs.push_back(design);

  uint64_t total = 0;
  std::unordered_map<std::string, bool> memo;
  for (const auto& d : designs)
    if (canFormDesign(d, patterns, memo)) total++;

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
