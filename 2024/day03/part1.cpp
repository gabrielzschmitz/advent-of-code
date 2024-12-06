/**
 * @file part1.cpp
 * @brief Extracting and Summing Valid Multiplication Instructions
 *
 * This program analyzes corrupted memory data from the North Pole Toboggan
 * Rental Shop's computer system. The goal is to identify valid multiplication
 * instructions of the form `mul(X,Y)` and compute the sum of their results. Due
 * to memory corruption, invalid sequences that look like multiplication
 * instructions but are not properly formatted must be ignored.
 *
 * A valid multiplication instruction follows these rules:
 * - Starts with `mul(` and ends with `)`.
 * - Contains two numbers, X and Y, separated by a comma.
 * - X and Y are integers with 1 to 3 digits.
 *
 * Example of Valid Input:
 * - `mul(44,46)` computes 44 * 46 = 2024.
 * - `mul(123,4)` computes 123 * 4 = 492.
 *
 * Example Corrupted Memory:
 * xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))
 * 
 * Valid `mul` instructions from the example:
 * - `mul(2,4)` computes 2 * 4 = 8.
 * - `mul(5,5)` computes 5 * 5 = 25.
 * - `mul(11,8)` computes 11 * 8 = 88.
 * - `mul(8,5)` computes 8 * 5 = 40.
 * 
 * Sum of results: 8 + 25 + 88 + 40 = 161.
 *
 * The program scans the corrupted memory input, extracts valid instructions,
 * and calculates the total sum of their results.
 *
 * Puzzle Answer: [Sum of Valid Multiplications]
 *
 * @author [gabrielzschmitz]
 * @date [06/12/2024]
 */
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
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
  std::regex mul_regex(R"(mul\((\d{1,3}),(\d{1,3})\))");
  std::smatch match;
  std::string buffer;
  char ch;
  while (input_file.get(ch)) {
    buffer += ch;

    while (std::regex_search(buffer, match, mul_regex)) {
      int x = std::stoi(match[1]);
      int y = std::stoi(match[2]);
      int result = x * y;
      total += result;

      if (debug)
        std::cout << "Valid: " << match.str() << " -> " << result << std::endl;
      buffer = match.suffix();
    }

    if (buffer.size() > 50) buffer.erase(0, buffer.size() - 50);
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
