/**
 * @file part2.cpp
 * @brief Easter Bunny HQ - First Revisited Location
 *
 * This program helps Santa locate the Easter Bunny Headquarters by following
 * a series of directional instructions. However, this time, Santa needs to find
 * the **first location he visits twice**.
 *
 * Santa starts at a given location facing North and follows a sequence of
 * instructions consisting of:
 * - 'L' followed by a number, indicating a 90-degree left turn and walking
 *   forward the given number of blocks.
 * - 'R' followed by a number, indicating a 90-degree right turn and walking
 *   forward the given number of blocks.
 *
 * As Santa walks, the program tracks all the coordinates he visits along the
 * way. The goal is to find the **first location that Santa visits twice**, and
 * compute the **Manhattan distance** from the starting point to that location.
 *
 * Example Input:
 * R8, R4, R4, R8  // First revisited location is 4 blocks away
 *
 * The program processes the full list of instructions and outputs the
 * Manhattan distance to the first revisited location.
 *
 * Puzzle Answer: [First Revisited Location Distance]
 *
 * @author [gabrielzschmitz]
 * @date [28/02/2025]
 */
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>
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

  std::set<std::pair<int, int>> visited;
  visited.insert({0, 0});

  for (const std::string& instr : instructions) {
    char turn = instr[0];
    int distance = std::stoi(instr.substr(1));

    if (turn == 'L') direction = (direction + 3) % 4;
    else if (turn == 'R') direction = (direction + 1) % 4;

    for (int step = 0; step < distance; ++step) {
      x += dx[direction];
      y += dy[direction];

      if (visited.count({x, y})) {
        int64_t manhattan_distance = std::abs(x) + std::abs(y);
        std::cout << "ANSWER: " << manhattan_distance << std::endl;
        return 0;
      }

      visited.insert({x, y});
    }

    if (debug)
      std::cout << "Moved to (" << x << ", " << y << ") facing direction "
                << direction << std::endl;
  }

  std::cerr << "NO LOCATION VISITED TWICE!" << std::endl;
  return 1;
}
