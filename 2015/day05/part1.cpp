/**
 * @file part1.cpp
 * @brief Naughty or Nice String Classifier
 *
 * This program helps Santa determine which strings in his text file are
 * "naughty" or "nice" based on a set of rules. A "nice" string must satisfy all
 * of the following conditions:
 * - It contains at least three vowels (aeiou).
 * - It has at least one letter that appears twice in a row.
 * - It does not contain the substrings "ab", "cd", "pq", or "xy".
 *
 * The program processes each string to check if it meets the criteria for being
 * classified as "nice" and counts how many strings in the input are "nice".
 *
 * Example Input:
 * - "ugknbfddgicrmopn" // Nice (three vowels, double letter "dd", no disallowed
 *   substrings)
 * - "aaa"              // Nice (three vowels, double letter "aa")
 * - "jchzalrnumimnmhp" // Naughty (no double letter)
 * - "haegwjzuvuyypxyu" // Naughty (contains disallowed substring "xy")
 * - "dvszwmarrgswjxmb" // Naughty (only one vowel)
 *
 * The program outputs the number of "nice" strings found in the input.
 *
 * Puzzle Answer: [Number of Nice Strings]
 *
 * @author [gabrielzschmitz]
 * @date [25/08/2024]
 */
#include <fstream>
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

bool checkThreeVowels(const std::string& str) {
  int vowel_count = 0;
  for (char ch : str)
    if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u')
      vowel_count++;
  return vowel_count >= 3;
}

bool checkDoubleLetter(const std::string& str) {
  for (size_t i = 0; i < str.size() - 1; ++i)
    if (str[i] == str[i + 1]) return true;
  return false;
}

bool checkBadStrings(const std::string& str) {
  std::vector<std::string> bad_strings = {"ab", "cd", "pq", "xy"};
  for (const auto& bad : bad_strings)
    if (str.find(bad) != std::string::npos) return false;
  return true;
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
    if (!(checkThreeVowels(line) && checkDoubleLetter(line) &&
          checkBadStrings(line)))
      ++bad;
    else ++nice;
  }

  if (debug)
    std::cout << "total nice: " << nice << ", total bad: " << bad << std::endl;

  std::cout << "ANSWER: " << nice << std::endl;

  return 0;
}
