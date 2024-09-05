/**
 * @file part1.cpp
 * @brief Identifying Aunt Sue
 *
 * This program helps you determine which of your 500 Aunt Sues sent you a gift. 
 * Each Aunt Sue is identified by a set of characteristics, but only one Aunt 
 * Sue matches the properties detected by your "My First Crime Scene Analysis 
 * Machine" (MFCSAM) which analyzed the gift wrapping. 
 *
 * The MFCSAM detects the following characteristics in the gift:
 * - children: 3
 * - cats: 7
 * - samoyeds: 2
 * - pomeranians: 3
 * - akitas: 0
 * - vizslas: 0
 * - goldfish: 5
 * - trees: 3
 * - cars: 2
 * - perfumes: 1
 *
 * You have a list of the traits for each of the 500 Aunt Sues. Your task is to 
 * compare the traits of each Aunt Sue with the MFCSAM results and determine 
 * which one sent you the gift.
 *
 * Example Input:
 * - Sue 1: children: 1, cars: 8, vizslas: 7
 * - Sue 2: akitas: 0, perfumes: 5, cars: 2
 * - Sue 3: trees: 3, goldfish: 5, akitas: 0
 *
 * The program processes the list of Aunt Sues and outputs the number of the 
 * Aunt Sue who matches the MFCSAM analysis.
 *
 * Puzzle Answer: [Aunt Sue Number]
 *
 * @author [gabrielzschmitz]
 * @date [05/09/2024]
 */
#include <fstream>
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

std::map<std::string, int> SIGNATURE = {
  {"children", 3}, {"cats", 7},    {"samoyeds", 2}, {"pomeranians", 3},
  {"akitas", 0},   {"vizslas", 0}, {"goldfish", 5}, {"trees", 3},
  {"cars", 2},     {"perfumes", 1}};

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

      if (SIGNATURE[attr1] == value1 && SIGNATURE[attr2] == value2 &&
          SIGNATURE[attr3] == value3) {
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
