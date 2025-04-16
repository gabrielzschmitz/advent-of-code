/**
 * @file part1.cpp
 * @brief Bathroom Security - Keypad Code Finder
 *
 * This program helps Santa retrieve the bathroom access code at Easter Bunny
 * Headquarters by interpreting a sequence of movement instructions over a
 * keypad.
 *
 * The keypad is a standard 3x3 layout:
 * 1 2 3
 * 4 5 6
 * 7 8 9
 *
 * Santa starts on button 5. Each line of instructions consists of characters:
 * - 'U' to move up
 * - 'D' to move down
 * - 'L' to move left
 * - 'R' to move right
 *
 * For each line, Santa begins from the button reached at the end of the
 * previous line (starting from 5 for the first line), and follows the
 * instructions one character at a time. If a move would take Santa off the
 * keypad, it is ignored. The button where he lands after each line becomes part
 * of the bathroom code.
 *
 * Example Input:
 * ULL     // Results in button 1
 * RRDDD   // Results in button 9
 * LURDL   // Results in button 8
 * UUUUD   // Results in button 5
 *
 * The resulting code is 1985.
 *
 * The program reads the full set of instruction lines and outputs the final
 * bathroom code.
 *
 * Puzzle Answer: [Bathroom Code]
 *
 * @author [gabrielzschmitz]
 * @date [16/04/2025]
 */
#include <cctype>
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

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  // Keypad as 2D grid
  std::vector<std::vector<char>> keypad = {
    {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};

  // Starting position is '5' -> row 1, col 1
  int row = 1, col = 1;

  std::string line, code;
  while (std::getline(input_file, line)) {
    for (char move : line) {
      switch (move) {
        case 'U':
          if (row > 0) row--;
          break;
        case 'D':
          if (row < 2) row++;
          break;
        case 'L':
          if (col > 0) col--;
          break;
        case 'R':
          if (col < 2) col++;
          break;
      }
    }
    code += keypad[row][col];
    if (debug) std::cout << "Current code digit: " << keypad[row][col] << "\n";
  }

  std::cout << "ANSWER: " << code << std::endl;
  return 0;
}
