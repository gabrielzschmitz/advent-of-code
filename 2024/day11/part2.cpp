/**
 * @file part2.cpp
 * @brief Plutonian Pebbles Extended Evolution
 *
 * This program builds upon the stone evolution process described in Part 1,
 * simulating the changes in a line of stones over a longer duration. The same
 * transformation rules apply:
 *
 * - If a stone is engraved with 0, it becomes a stone engraved with 1.
 * - If a stone's number has an even number of digits, it splits into two
 *   stones. The left stone gets the first half of the digits, and the right
 *   stone gets the second half. Leading zeros are removed in the split.
 * - For any other number, the stone is replaced with a new stone engraved with
 *   the original number multiplied by 2024.
 *
 * The stones maintain their order after each transformation.
 *
 * **Task:** Calculate how many stones remain in the line after 75 iterations of
 * blinking.
 *
 * **Example Input:**
 * ```
 * Initial stones: 125 17
 * ```
 *
 * **Example Output After 6 Blinks:**
 * ```
 * Stones: 2097446912 14168 4048 2 0 2 4 40 48 2024 40 48 80 96 2 8 6 7 6 0 3 2
 * ```
 *
 * **Key Steps to Solve:**
 * 1. Parse the input list of stones.
 * 2. Extend the blinking process to 75 iterations:
 *    - Apply the transformation rules simultaneously to all stones in the line.
 *    - Use efficient data structures to handle the large growth in the number
 *    of stones.
 * 3. Output the total number of stones after 75 iterations.
 *
 * **Challenges:**
 * - Exponential growth in the number of stones requires optimized storage and
 *   processing.
 * - Memory and computational efficiency are critical to simulate 75 iterations
 *   successfully.
 *
 * **Puzzle Answer:** [Number of stones after 75 blinks]
 *
 * @author [gabrielzschmitz]
 * @date [10/01/2024]
 */
#include <cmath>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

std::vector<std::string> splitNumber(const std::string& number) {
  size_t len = number.length();
  size_t mid = len / 2;
  std::string left = number.substr(0, mid);
  std::string right = number.substr(mid);

  if (!left.empty() && left[0] == '0')
    left.erase(0, left.find_first_not_of('0'));
  if (!right.empty() && right[0] == '0') 
    right.erase(0, right.find_first_not_of('0'));

  if (left.empty()) left = "0";
  if (right.empty()) right = "0";

  return {left, right};
}

// Memoization table: map<stone, map<remaining_iterations, resulting_count>>
std::unordered_map<std::string, std::unordered_map<int, uint64_t>> memo;

uint64_t countStones(const std::string& stone, int iterations) {
  if (iterations == 0) return 1;

  if (memo[stone].count(iterations)) return memo[stone][iterations];

  uint64_t result = 0;

  if (stone == "0") {
    // Rule 1: 0 turns to 1
    result = countStones("1", iterations - 1);
  } else if (stone.length() % 2 == 0) {
    // Rule 2: Split into two stones
    auto parts = splitNumber(stone);
    result = countStones(parts[0], iterations - 1) + countStones(parts[1],
        iterations - 1);
  } else {
    // Rule 3: Multiply by 2024
    uint64_t number = std::stoull(stone);
    result = countStones(std::to_string(number * 2024), iterations - 1);
  }

  memo[stone][iterations] = result;
  return result;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::string> stones;
  std::string line;
  while (std::getline(input_file, line)) {
    std::stringstream ss(line);
    std::string stone;
    while (ss >> stone)
      stones.push_back(stone);
  }

  const int total_iterations = 75;
  uint64_t total = 0;

  for (const auto& stone : stones)
    total += countStones(stone, total_iterations);

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
