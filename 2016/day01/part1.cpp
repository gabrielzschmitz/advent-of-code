/**
 * @file part1.cpp
 * @brief Easter Bunny HQ Navigation
 *
 * This program helps Santa find his way to Easter Bunny Headquarters by
 * following a sequence of directional instructions. Santa starts at a given
 * location facing North and must follow a set of turns and moves to reach his
 * final destination.
 *
 * The instructions consist of:
 * - 'L' followed by a number, indicating a 90-degree left turn and walking
 *   forward the given number of blocks.
 * - 'R' followed by a number, indicating a 90-degree right turn and walking
 *   forward the given number of blocks.
 *
 * The goal is to calculate the **shortest Manhattan distance** from Santa's
 * starting point to his final destination after following all the instructions.
 *
 * Example Input:
 * R2, L3    // Ends 5 blocks away
 * R2, R2, R2 // Ends 2 blocks away
 * R5, L5, R5, R3 // Ends 12 blocks away
 *
 * The program processes the full list of instructions and outputs the total
 * Manhattan distance between the starting point and final position.
 *
 * Puzzle Answer: [Manhattan Distance]
 *
 * @author [gabrielzschmitz]
 * @date [28/02/2025]
 */
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

/* Directions: 0 = North, 1 = East, 2 = South, 3 = West */
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

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

  std::vector<std::string> instructions;
  std::stringstream ss(line);
  std::string instruction;

  while (std::getline(ss, instruction, ',')) {
    if (instruction.front() == ' ') instruction = instruction.substr(1);
    instructions.push_back(instruction);
  }

  int x = 0, y = 0;
  int direction = 0;

  for (const std::string& instr : instructions) {
    char turn = instr[0];
    int distance = std::stoi(instr.substr(1));

    if (turn == 'L') direction = (direction + 3) % 4;
    else if (turn == 'R') direction = (direction + 1) % 4;

    x += dx[direction] * distance;
    y += dy[direction] * distance;

    if (debug)
      std::cout << "Moved to (" << x << ", " << y << ") facing direction "
                << direction << std::endl;
  }

  int64_t manhattan_distance = std::abs(x) + std::abs(y);
  std::cout << "ANSWER: " << manhattan_distance << std::endl;
  return 0;
}
