/**
 * @file part1.cpp
 * @brief Plutonian Pebbles Evolution
 *
 * This program simulates the evolution of a line of physics-defying stones,
 * each engraved with a number. The stones change simultaneously every time the
 * user "blinks" based on a set of rules:
 *
 * - If a stone is engraved with 0, it becomes a stone engraved with 1.
 * - If a stone's number has an even number of digits, it splits into two
 *   stones. The left stone gets the first half of the digits, and the right
 *   stone gets the second half. Leading zeros are removed in the split.
 * - For any other number, the stone is replaced with a new stone engraved with
 *   the original number multiplied 
 *   by 2024.
 *
 * The stones maintain their order after each transformation.
 *
 * **Task:** Calculate how many stones remain in the line after 25 iterations of
 * blinking.
 *
 * **Example Input:**
 * ```
 * Initial stones: 0 1 10 99 999
 * ```
 *
 * **Example Output After 1 Blink:**
 * ```
 * Stones: 1 2024 1 0 9 9 2021976
 * ```
 *
 * **Longer Example:**
 * - Initial stones: `125 17`
 * - After 1 blink: `253000 1 7`
 * - After 6 blinks: `2097446912 14168 4048 2 0 2 4 40 48 2024 40 48 80 96 2 8 6
 *   7 6 0 3 2`
 * - Number of stones after 25 blinks: `55312`
 *
 * **Steps to Solve:**
 * 1. Parse the input list of stones.
 * 2. Simulate the blinking process for 25 iterations:
 *    - Apply the rules to each stone in the line.
 *    - Track the resulting stones in a new list while preserving their order.
 * 3. Output the total number of stones after 25 iterations.
 *
 * **Constraints:**
 * - The input consists of integers representing the initial stones.
 * - Each blink operation transforms all stones simultaneously.
 *
 * **Puzzle Answer:** [Number of stones after 25 blinks]
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

std::vector<std::string> processBlink(const std::vector<std::string>& stones) {
  std::vector<std::string> new_stones;

  for (const auto& stone : stones) {
    if (stone == "0") {
      // Rule 1: 0 turns to 1
      new_stones.push_back("1");
    } else if (stone.length() % 2 == 0) {
      // Rule 2: Split into two stones
      auto parts = splitNumber(stone);
      new_stones.push_back(parts[0]);
      new_stones.push_back(parts[1]);
    } else {
      // Rule 3: Multiply by 2024
      uint64_t number = std::stoull(stone);
      new_stones.push_back(std::to_string(number * 2024));
    }
  }

  return new_stones;
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

  for (int iteration = 0; iteration < 25; ++iteration) {
    stones = processBlink(stones);

    if (debug)
      std::cout << "Iteration " << iteration + 1 << ": " << stones.size() <<
        " stones\n";
  }

  uint64_t total = stones.size();

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
