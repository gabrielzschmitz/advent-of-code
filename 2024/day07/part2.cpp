/**
 * @file part2.cpp
 * @brief Bridge Repair Calibration Validation with Concatenation
 *
 * After identifying the initial set of valid calibration equations, the
 * engineers discover that there is a third operator hidden by some elephants:
 * the concatenation operator (||). This operator combines the digits from its
 * left and right inputs into a single number, such as 12 || 345 resulting in
 * 12345.
 *
 * The task is now to reevaluate the equations, considering this new operator
 * along with addition (+) and multiplication (*). All operators are still
 * evaluated left-to-right (no precedence rules), and numbers must remain in
 * their original order.
 *
 * Example Input:
 * 190: 10 19         // Valid: 10 * 19 = 190
 * 3267: 81 40 27     // Valid: 81 + 40 * 27 = 3267
 * 7290: 6 8 6 15     // Valid: 6 * 8 || 6 * 15 = 7290
 * 156: 15 6          // Valid: 15 || 6 = 156
 * 192: 17 8 14       // Valid: 17 || 8 + 14 = 192
 *
 * In the updated challenge, you must now consider all three types of operators 
 * (+, *, and ||) and find the total calibration result.
 *
 * Example Output:
 * Total Calibration Result: 11387
 *
 * Tasks:
 * 1. Parse each equation and determine if it can be made valid by inserting 
 *    any combination of the three operators (+, *, ||).
 * 2. Sum the test values of all valid equations to compute the new total
 *    calibration result.
 *
 * Puzzle Answer: [New Total Calibration Result]
 *
 * @author [gabrielzschmitz]
 * @date [20/12/2024]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

uint64_t concatenate(uint64_t left, uint64_t right) {
  return std::stoull(std::to_string(left) + std::to_string(right));
}

bool evaluateExpression(const std::vector<uint64_t>& numbers, uint64_t target) {
  uint64_t n = numbers.size();

  uint64_t max_mask =
    1 << (2 * (n - 1));  // There are 3^(n-1) combinations of operations

  for (uint64_t mask = 0; mask < max_mask; ++mask) {
    uint64_t result = numbers[0];
    size_t mask_copy = mask;

    for (uint64_t i = 0; i < n - 1; ++i) {
      char op;
      switch (mask_copy % 3) {
        case 0: op = '+'; break;
        case 1: op = '*'; break;
        case 2: op = '|'; break;
      }

      mask_copy /= 3;

      if (op == '+') result += numbers[i + 1];
      else if (op == '*') result *= numbers[i + 1];
      else if (op == '|') result = concatenate(result, numbers[i + 1]);
    }

    if (result == target) return true;
  }

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

  uint64_t total = 0;

  std::string line;
  while (std::getline(input_file, line)) {
    size_t colon_pos = line.find(":");
    if (colon_pos == std::string::npos) continue;

    uint64_t target = std::stoull(line.substr(0, colon_pos));
    std::vector<uint64_t> numbers;

    std::istringstream numbers_stream(line.substr(colon_pos + 1));
    uint64_t number;
    while (numbers_stream >> number)
      numbers.push_back(number);

    if (evaluateExpression(numbers, target)) {
      total += target;
      if (debug) std::cout << "Valid equation: " << line << std::endl;
    } else if (debug) std::cout << "Invalid equation: " << line << std::endl;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
