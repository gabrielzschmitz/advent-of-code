/**
 * @file part2.cpp
 * @brief Bathroom Security - Advanced Keypad Code Finder
 *
 * This program helps Santa retrieve the bathroom access code at Easter Bunny
 * Headquarters using a more complex keypad layout, and interpreting a sequence
 * of movement instructions over this new keypad.
 *
 * The keypad layout is as follows:
 * 
 *     1
 *   2 3 4
 * 5 6 7 8 9
 *   A B C
 *     D
 * 
 * Santa starts at button 5. The instructions consist of characters:
 * - 'U' to move up
 * - 'D' to move down
 * - 'L' to move left
 * - 'R' to move right
 *
 * Similar to Part 1, Santa follows the instructions starting from button 5. If
 * a move would take Santa off the keypad, it is ignored. After processing all
 * the instructions, the sequence of buttons Santa presses forms the bathroom
 * code.
 *
 * Example Input:
 * ULL     // Results in button 5
 * RRDDD   // Results in button D
 * LURDL   // Results in button B
 * UUUUD   // Results in button 3
 *
 * The resulting code is 5DB3.
 *
 * The program processes the instructions and outputs the final bathroom code.
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

  std::vector<std::vector<char>> keypad = {{' ', ' ', '1', ' ', ' '},
                                           {' ', '2', '3', '4', ' '},
                                           {'5', '6', '7', '8', '9'},
                                           {' ', 'A', 'B', 'C', ' '},
                                           {' ', ' ', 'D', ' ', ' '}};

  int row = 2, col = 0;

  std::string line, code;
  while (std::getline(input_file, line)) {
    for (char move : line) {
      int new_row = row, new_col = col;

      switch (move) {
        case 'U': new_row--; break;
        case 'D': new_row++; break;
        case 'L': new_col--; break;
        case 'R': new_col++; break;
      }

      if (new_row >= 0 && new_row < 5 && new_col >= 0 && new_col < 5 &&
          keypad[new_row][new_col] != ' ') {
        row = new_row;
        col = new_col;
      }
    }
    code += keypad[row][col];
    if (debug) std::cout << "Current code digit: " << keypad[row][col] << "\n";
  }

  std::cout << "ANSWER: " << code << std::endl;
  return 0;
}
