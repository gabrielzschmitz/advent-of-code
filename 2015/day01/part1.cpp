/**
 * @file part1.cpp
 * @brief Santa's Floor Navigation
 *
 * This program helps Santa navigate through a large apartment building by
 * interpreting a series of directions. Santa starts on the ground floor
 * (floor 0) and needs to follow a series of instructions to determine his final
 * floor position.
 * The instructions are given as a string of characters where:
 * - An opening parenthesis '(' indicates moving up one floor.
 * - A closing parenthesis ')' indicates moving down one floor.
 *
 * The task is to calculate the final floor Santa will end up on after following
 * all the instructions provided in the input. This is the result of adjusting
 * Santa's floor position based on the instructions from his initial position of
 * floor 0.
 *
 * Example Input:
 * (())      // Results in floor 0
 * (((       // Results in floor 3
 * ))(((((   // Results in floor 3
 * ())       // Results in floor -1
 * )))       // Results in floor -3
 *
 * The program processes the instructions and outputs the final floor Santa
 * reaches.
 *
 * Puzzle Answer: [Final Floor]
 *
 * @author [gabrielzschmitz]
 * @date [21/08/2024]
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

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  char ch;
  int open_parentheses_count = 0;
  int close_parentheses_count = 0;
  while (input_file.get(ch)) {
    if (ch == '(') ++open_parentheses_count;
    else if (ch == ')') ++close_parentheses_count;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = open_parentheses_count - close_parentheses_count;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
