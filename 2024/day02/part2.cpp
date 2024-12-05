/**
 * @file part2.cpp
 * @brief Analyzing Reactor Safety Reports with the Problem Dampener
 *
 * This program builds upon the previous analysis of reactor safety reports from
 * the Red-Nosed reactor, but now includes the Problem Dampener. The Problem
 * Dampener allows the reactor safety system to tolerate one bad level in an
 * otherwise safe report. If removing a single level from an unsafe report would
 * make it safe, the report is now considered safe.
 * 
 * The input consists of multiple reports, each represented as a line of
 * space-separated numbers. The program evaluates each report against the safety
 * criteria:
 * 
 * 1. The levels must be either all increasing or all decreasing.
 * 2. Any two adjacent levels must differ by at least 1 and at most 3.
 * 
 * If a report doesn't meet the criteria, the Problem Dampener can be applied to
 * remove one level, making it safe.
 *
 * Example Input:
 * 7 6 4 2 1     // Safe: All levels decreasing by 1 or 2
 * 1 2 7 8 9     // Unsafe: Increase of 5 between 2 and 7
 * 9 7 6 2 1     // Unsafe: Decrease of 4 between 6 and 2
 * 1 3 2 4 5     // Safe by removing the second level, 3
 * 8 6 4 4 1     // Safe by removing the third level, 4
 * 1 3 6 7 9     // Safe: All levels increasing by 1, 2, or 3
 *
 * With the Problem Dampener, 4 reports are classified as safe.
 *
 * The program processes all reports and outputs the total count of safe
 * reports, including those that became safe by removing one level.
 *
 * Puzzle Answer: [Number of Safe Reports After Problem Dampener]
 *
 * @author [gabrielzschmitz]
 * @date [04/12/2024]
 */
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

bool isIncreasing(const std::vector<int>& numbers) {
  for (size_t i = 1; i < numbers.size(); ++i)
    if (numbers[i] <= numbers[i - 1] ||
        std::abs(numbers[i] - numbers[i - 1]) < 1 ||
        std::abs(numbers[i] - numbers[i - 1]) > 3)
      return false;
  return true;
}

bool isDecreasing(const std::vector<int>& numbers) {
  for (size_t i = 1; i < numbers.size(); ++i)
    if (numbers[i] >= numbers[i - 1] ||
        std::abs(numbers[i] - numbers[i - 1]) < 1 ||
        std::abs(numbers[i] - numbers[i - 1]) > 3)
      return false;
  return true;
}

bool canBeMadeSafeByRemovingOne(std::vector<int>& numbers) {
  for (size_t i = 0; i < numbers.size(); ++i) {
    std::vector<int> modified_numbers = numbers;
    modified_numbers.erase(modified_numbers.begin() + i);

    if (isIncreasing(modified_numbers) || isDecreasing(modified_numbers))
      return true;
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

  int total = 0;
  std::string line;
  while (std::getline(input_file, line)) {
    std::istringstream line_stream(line);
    std::vector<int> report;
    int num;

    while (line_stream >> num)
      report.push_back(num);

    if (isIncreasing(report) || isDecreasing(report) ||
        canBeMadeSafeByRemovingOne(report))
      total += 1;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
