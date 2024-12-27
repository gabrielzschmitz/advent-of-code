/**
 * @file part2.cpp
 * @brief Counting Towel Arrangement Options for Onsen Designs
 *
 * This program extends the task of matching towel patterns to create requested
 * designs by counting all possible ways each design can be constructed. The
 * goal is to calculate the total number of unique arrangements for all
 * requested designs.
 *
 * **Challenge Description:**
 * - Each design consists of a sequence of colored stripes that must be matched 
 *   using one or more towels from the available patterns.
 * - Unlimited towels are available for each pattern, but the sequence of
 *   stripes must be constructed in order.
 * - The task is to determine all possible ways to arrange towels to form each 
 *   design and sum the total across all designs.
 *
 * **Task:**
 * 1. For each requested design, compute the total number of unique arrangements
 *    that match the design using the given towel patterns.
 * 2. Sum the counts for all designs to get the final result.
 *
 * **Example Input:**
 * ```
 * Available Patterns: r, wr, b, g, bwu, rb, gb, br
 * Requested Designs:
 *   brwrr
 *   bggr
 *   gbbr
 *   rrbgbr
 *   ubwu
 *   bwurrg
 *   brgr
 *   bbrgwb
 * ```
 *
 * **Example Output:**
 * ```
 * Total Arrangements: 16
 * ```
 *
 * **Steps to Solve:**
 * 1. Parse the input to separate towel patterns and requested designs.
 * 2. Use a recursive or dynamic programming approach to calculate the number
 *    of ways each design can be constructed.
 *    - Consider all patterns that match the prefix of the design.
 *    - Recursively compute the number of ways to construct the remaining part.
 * 3. Output the sum of all arrangement counts.
 *
 * **Constraints:**
 * - Designs that cannot be constructed from the patterns contribute 0 to the
 *   total.
 * - Only exact matches of the sequence (in order) are allowed.
 *
 * **Example Breakdown:**
 * - `brwrr` has 2 arrangements: [b, r, wr, r] or [br, wr, r].
 * - `bggr` has 1 arrangement: [b, g, g, r].
 * - Total = 2 + 1 + 4 + 6 + 0 + 1 + 2 + 0 = 16.
 *
 * **Puzzle Answer:** [Total Arrangements]
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

uint64_t countArrangements(const std::string& design,
                           const std::unordered_set<std::string>& patterns,
                           std::unordered_map<std::string, uint64_t>& memo) {
  if (design.empty()) return 1;
  if (memo.find(design) != memo.end()) return memo[design];

  uint64_t totalWays = 0;
  for (const auto& pattern : patterns)
    if (design.find(pattern) == 0)
      totalWays +=
        countArrangements(design.substr(pattern.size()), patterns, memo);

  memo[design] = totalWays;
  return totalWays;
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
  std::unordered_map<std::string, uint64_t> memo;
  for (const auto& d : designs)
    total += countArrangements(d, patterns, memo);

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
