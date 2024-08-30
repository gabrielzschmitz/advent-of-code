/**
 * @file part2.cpp
 * @brief Look-and-Say Sequence Length Calculation (50 Iterations)
 *
 * This program calculates the length of the look-and-say sequence after a
 * specified number of iterations. The look-and-say sequence is a process where
 * each number is read aloud and replaced by a description of the count of each
 * digit followed by the digit itself. The process is iteratively applied to
 * generate the next sequence.
 *
 * Starting with an initial sequence, the program applies the look-and-say
 * transformation 50 times. Each iteration involves replacing each run of digits
 * with the number of digits followed by the digit itself.
 *
 * For example:
 * - Starting with "1", the sequence evolves as follows:
 *   1 → 11 (one 1)
 *   11 → 21 (two 1s)
 *   21 → 1211 (one 2, one 1)
 *   1211 → 111221 (one 1, one 2, two 1s)
 *   111221 → 312211 (three 1s, two 2s, one 1)
 *
 * The program processes the initial sequence through 50 iterations and outputs
 * the length of the final result.
 *
 * Puzzle Answer: [Length of Final Sequence]
 *
 * @author [gabrielzschmitz]
 * @date [30/08/2024]
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

std::string lookAndSay(std::string input, int n) {
  if (n == 0) return input;

  std::string newStr = "";
  int length = input.size();
  int i = 0;

  while (i < length) {
    int count = 1;
    while (i + 1 < length && input[i] == input[i + 1]) {
      i++;
      count++;
    }
    newStr += std::to_string(count) + input[i];
    if (debug)
      std::cout << "current: " << std::to_string(count) + input[i] << std::endl;
    i++;
  }

  return lookAndSay(newStr, n - 1);
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string line;
  std::getline(input_file, line);
  std::string input;
  int _;
  std::istringstream stream(line);
  stream >> input >> _;

  if (debug) std::cout << "input: " << input << " * " << _ << std::endl;
  std::string say = lookAndSay(input, 50);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = say.size();
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
