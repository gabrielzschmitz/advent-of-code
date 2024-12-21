/**
 * @file part1.cpp
 * @brief Bridge Repair Calibration Validation
 *
 * This program aids the engineers repairing a jungle rope bridge by determining 
 * which calibration equations can be made valid using a combination of addition 
 * (+) and multiplication (*) operators. The equations are crucial for final 
 * calibrations, allowing the bridge to function properly.
 *
 * Each equation is provided in the format:
 * - [Test Value]: [List of Numbers]
 *
 * The task is to check if the test value (the number before the colon) can be 
 * produced by inserting operators (+ or *) between the numbers in the given 
 * order. Operators are evaluated left-to-right, not by precedence. The numbers 
 * must remain in the original order, and all positions between numbers must be 
 * filled with an operator.
 *
 * Example Input:
 * 190: 10 19         // Valid: 10 * 19 = 190
 * 3267: 81 40 27     // Valid: 81 + 40 * 27 = 3267
 * 292: 11 6 16 20    // Valid: 11 + 6 * 16 + 20 = 292
 * 83: 17 5           // Invalid
 * 
 * Example Output:
 * Total Calibration Result: 3749
 *
 * Tasks:
 * 1. Parse each equation and determine if it can be made valid by inserting 
 *    appropriate operators.
 * 2. Sum the test values of all valid equations to compute the total
 *    calibration result.
 *
 * Puzzle Answer: [Total Calibration Result]
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

bool evaluateExpression(const std::vector<uint64_t>& numbers, uint64_t target) {
  uint64_t n = numbers.size();
  uint64_t combinations = 1 << (n - 1);  // 2^(n-1) combinations of operators

  for (uint64_t mask = 0; mask < combinations; ++mask) {
    uint64_t result = numbers[0];

    for (uint64_t i = 0; i < n - 1; ++i) {
      if (mask & (1 << i)) result *= numbers[i + 1];
      else result += numbers[i + 1];
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
