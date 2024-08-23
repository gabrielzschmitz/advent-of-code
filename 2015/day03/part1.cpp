/**
 * @file part1.cpp
 * @brief Santa's Infinite Grid Delivery
 *
 * This program helps Santa deliver presents to an infinite two-dimensional grid
 * of houses. Santa starts by delivering a present to the house at his initial
 * location. After each move, he delivers another present to the house at his
 * new location, based on directions provided by an elf at the North Pole.
 *
 * The directions are given as a string of characters where:
 * - '^' indicates moving north.
 * - 'v' indicates moving south.
 * - '>' indicates moving east.
 * - '<' indicates moving west.
 *
 * The task is to determine how many unique houses receive at least one present, 
 * even if Santa visits some houses more than once due to faulty directions.
 *
 * Example Scenarios:
 * - '>' delivers presents to 2 houses: one at the starting location, and one to
 *   the east.
 * - '^>v<' delivers presents to 4 houses in a square, including a revisit to
 *   the 
 *   starting/ending location.
 * - '^v^v^v^v^v' delivers presents to only 2 houses, despite multiple moves.
 *
 * The program processes the direction string and outputs the total number of
 * unique houses that receive at least one present.
 *
 * Puzzle Answer: [Total Number of Houses Receiving at Least One Present]
 *
 * @author [gabrielzschmitz]
 * @date [23/08/2024]
 */
#include <fstream>
#include <iostream>
#include <set>
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

  std::set<std::pair<int, int>> visited_houses;
  int x = 0, y = 0;
  visited_houses.insert({x, y});

  char ch;
  while (input_file.get(ch)) {
    switch (ch) {
      case '^': y++; break;
      case 'v': y--; break;
      case '>': x++; break;
      case '<': x--; break;
      default: break;
    }
    visited_houses.insert({x, y});

    if (debug) std::cout << "Moved to: (" << x << ", " << y << ")" << std::endl;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = visited_houses.size();
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
