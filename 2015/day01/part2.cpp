/**
 * @file part2.cpp
 * @brief Santa's Basement Entry Detection
 *
 * This program extends the task from Part One by adding the requirement to
 * determine the exact position of the first character in the instructions that
 * causes Santa to enter the basement (floor -1). Santa starts on the ground
 * floor (floor 0), and follows a series of instructions where:
 * - An opening parenthesis '(' indicates moving up one floor.
 * - A closing parenthesis ')' indicates moving down one floor.
 *
 * The task is to find the position (1-based index) of the first character that,
 * when processed, causes Santa to enter the basement. The position is counted
 * from the start of the instruction string, with the first character being at
 * position 1.
 *
 * Example Input:
 * )          // Causes Santa to enter the basement at position 1.
 * ()())      // Causes Santa to enter the basement at position 5.
 *
 * The program processes the instructions and outputs the position of the first
 * character that results in Santa entering the basement.
 *
 * Puzzle Answer: [Position]
 *
 * @author [gabrielzschmitz]
 * @date [21/08/2024]
 */
#include <fstream>
#include <iostream>
#include <string>

bool debug = false;

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  if (user_input == "test") debug = true;

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  char ch;
  int position = 0;
  int open_parentheses_count = 0;
  int close_parentheses_count = 0;
  int total = 0;
  while (input_file.get(ch)) {
    if (ch == '(') ++open_parentheses_count;
    else if (ch == ')') ++close_parentheses_count;
    ++position;
    total = open_parentheses_count - close_parentheses_count;
    if (total == -1) break;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << position << std::endl;
  return 0;
}
