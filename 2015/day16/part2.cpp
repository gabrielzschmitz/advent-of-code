/**
 * @file part2.cpp
 * @brief Identifying the Real Aunt Sue with Updated Analysis
 *
 * After reviewing the MFCSAM's instructions more carefully, you discover that 
 * its output for certain traits is not exact and indicates ranges instead. 
 * Specifically:
 * - Cats: The number is greater than the value reported by the MFCSAM.
 * - Trees: The number is greater than the value reported by the MFCSAM.
 * - Pomeranians: The number is less than the value reported by the MFCSAM.
 * - Goldfish: The number is less than the value reported by the MFCSAM.
 *
 * With this new understanding, the task is to reanalyze the list of Aunt Sues 
 * and determine which one matches the MFCSAM results, considering these ranges.
 *
 * The original MFCSAM output is:
 * - children: 3
 * - cats: 7 (greater than)
 * - samoyeds: 2
 * - pomeranians: 3 (less than)
 * - akitas: 0
 * - vizslas: 0
 * - goldfish: 5 (less than)
 * - trees: 3 (greater than)
 * - cars: 2
 * - perfumes: 1
 *
 * Example Input:
 * - Sue 1: children: 1, cars: 8, vizslas: 7
 * - Sue 2: akitas: 0, perfumes: 5, cars: 2
 * - Sue 3: trees: 3, goldfish: 5, akitas: 0
 *
 * The program now processes the list of Aunt Sues with the new range-based 
 * criteria and outputs the number of the real Aunt Sue who matches the updated 
 * MFCSAM analysis.
 *
 * Puzzle Answer: [Real Aunt Sue Number]
 *
 * @author [gabrielzschmitz]
 * @date [05/09/2024]
 */
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

std::map<std::string, std::function<bool(int)>> SIGNATURE = {
  {"children",
   [](int value) {
     return value == 3;
   }},
  {"cats",
   [](int value) {
     return value > 7;
   }},
  {"samoyeds",
   [](int value) {
     return value == 2;
   }},
  {"pomeranians",
   [](int value) {
     return value < 3;
   }},
  {"akitas",
   [](int value) {
     return value == 0;
   }},
  {"vizslas",
   [](int value) {
     return value == 0;
   }},
  {"goldfish",
   [](int value) {
     return value < 5;
   }},
  {"trees",
   [](int value) {
     return value > 3;
   }},
  {"cars",
   [](int value) {
     return value == 2;
   }},
  {"perfumes", [](int value) {
     return value == 1;
   }}};

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string line;
  std::regex auntRegex(
    R"(Sue (\d+): (\w+): (\d+), (\w+): (\d+), (\w+): (\d+))");
  std::smatch match;

  int total = 0;
  while (std::getline(input_file, line)) {
    if (std::regex_search(line, match, auntRegex)) {
      int auntNumber = std::stoi(match[1]);
      std::string attr1 = match[2], attr2 = match[4], attr3 = match[6];
      int value1 = std::stoi(match[3]), value2 = std::stoi(match[5]),
          value3 = std::stoi(match[7]);

      if (SIGNATURE[attr1](value1) && SIGNATURE[attr2](value2) &&
          SIGNATURE[attr3](value3)) {
        total = auntNumber;
        break;
      }
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
