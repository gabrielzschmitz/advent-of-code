/**
 * @file part2.cpp
 * @brief New Criteria for Determining Naughty or Nice Strings
 *
 * This program helps Santa evaluate which strings are classified as "naughty"
 * or "nice" under a new set of rules. A string is considered "nice" if it
 * satisfies both of the following conditions:
 * - It contains a pair of any two letters that appears at least twice in the
 *   string without overlapping, such as "xyxy" or "aabcdefgaa".
 * - It contains at least one letter that repeats with exactly one letter
 *   between them, such as "xyx" or "efe".
 *
 * The task is to assess each string according to these updated rules and count
 * how many strings are nice.
 *
 * Example Input:
 * - qjhvhtzxzqqjkmpb // Nice: contains a pair "qj" that appears twice and a
 *   repeating letter "zxz".
 * - xxyxx            // Nice: contains a pair "xx" that appears twice and a
 *   repeating letter "xyx".
 * - uurcxstgmygtbstg // Naughty: contains a pair "tg" but no repeating letter
 *   with one between them.
 * - ieodomkazucvgmuy // Naughty: contains a repeating letter "odo" but no pair
 *   that appears twice.
 *
 * The program processes each string and outputs the total count of nice strings
 * under the new rules.
 *
 * Puzzle Answer: [Number of Nice Strings]
 *
 * @author [gabrielzschmitz]
 * @date [25/08/2024]
 */
#include <fstream>
#include <iostream>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

bool checkDoubleSkip(const std::string& str) {
  for (size_t i = 0; i < str.size() - 2; ++i)
    if (str[i] == str[i + 2]) return true;
  return false;
}

bool checkPairTwice(const std::string& str) {
  for (size_t i = 0; i < str.size() - 1; ++i) {
    std::string pair = str.substr(i, 2);
    for (size_t j = i + 2; j < str.size() - 1; ++j)
      if (pair == str.substr(j, 2)) return true;
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

  int nice = 0;
  int bad = 0;
  std::string line;

  while (std::getline(input_file, line)) {
    if (!(checkDoubleSkip(line) && checkPairTwice(line))) ++bad;
    else ++nice;
  }

  if (debug)
    std::cout << "total nice: " << nice << ", total bad: " << bad << std::endl;

  std::cout << "ANSWER: " << nice << std::endl;

  return 0;
}
