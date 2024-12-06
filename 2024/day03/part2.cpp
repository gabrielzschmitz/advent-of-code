/**
 * @file part2.cpp
 * @brief Filtering Valid Multiplications Based on Conditional Instructions
 *
 * This program extends the analysis of corrupted memory data by incorporating
 * conditional instructions that control whether multiplication operations
 * (`mul(X,Y)`) are enabled or disabled. The goal is to extract and sum up the
 * results of only those `mul` instructions that are enabled based on the most
 * recent conditional directive.
 *
 * Conditional Instructions:
 * - `do()` enables future `mul` instructions.
 * - `don't()` disables future `mul` instructions.
 * - At the start of the program, `mul` instructions are enabled by default.
 *
 * A valid `mul` instruction:
 * - Must follow the same format as defined in Part 1 (`mul(X,Y)`).
 * - Is only processed if it occurs while enabled by the most recent `do()` or
 *   `don't()` instruction.
 *
 * Example Corrupted Memory:
 * xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))
 *
 * Execution Details:
 * - Initial state: `mul` instructions are enabled.
 * - `mul(2,4)` computes 2 * 4 = 8 (enabled).
 * - `mul(5,5)` is skipped (disabled by `don't()`).
 * - `mul(11,8)` is skipped (disabled by `don't()`).
 * - `mul(8,5)` computes 8 * 5 = 40 (re-enabled by `do()`).
 *
 * Sum of results: 8 + 40 = 48.
 *
 * The program scans the input, tracks the current state of `mul` instruction
 * enablement, extracts valid and enabled `mul` instructions, and computes the
 * total sum of their results.
 *
 * Puzzle Answer: [Sum of Enabled Multiplications]
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
  bool mul_enabled = true;
  std::regex mul_regex(R"(mul\((\d{1,3}),(\d{1,3})\))");
  std::regex do_regex(R"(do\(\))");
  std::regex dont_regex(R"(don't\(\))");
  std::smatch match;
  std::string buffer;
  char ch;
  while (input_file.get(ch)) {
    buffer += ch;

    while (std::regex_search(buffer, match, do_regex)) {
      mul_enabled = true;
      if (debug) std::cout << "Enabled mul" << std::endl;
      buffer = match.suffix();
    }
    while (std::regex_search(buffer, match, dont_regex)) {
      mul_enabled = false;
      if (debug) std::cout << "Disabled mul" << std::endl;
      buffer = match.suffix();
    }

    while (mul_enabled && std::regex_search(buffer, match, mul_regex)) {
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
