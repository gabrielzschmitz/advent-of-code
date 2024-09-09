/**
 * @file part1.cpp
 * @brief Infinite Elves and Infinite Houses
 *
 * In this problem, Santa tasks the Elves with delivering presents to an
 * infinite number of houses, each numbered sequentially starting from 1. Each
 * Elf is assigned a number and delivers presents to houses based on that
 * number.
 *
 * - Elf 1 delivers presents to every house: 1, 2, 3, 4, 5, ... 
 * - Elf 2 delivers presents to every second house: 2, 4, 6, 8, 10, ... 
 * - Elf 3 delivers presents to every third house: 3, 6, 9, 12, 15, ... 
 *
 * The number of presents delivered by each Elf is equal to ten times the Elf's
 * number. For example, Elf 1 delivers 10 presents to every house it visits, Elf
 * 2 delivers 20 presents to every second house, and so on.
 *
 * Example of the first nine houses:
 * - House 1: 10 presents (visited by Elf 1)
 * - House 2: 30 presents (visited by Elves 1, 2)
 * - House 3: 40 presents (visited by Elves 1, 3)
 * - House 4: 70 presents (visited by Elves 1, 2, 4)
 * - House 5: 60 presents (visited by Elves 1, 5)
 * - House 6: 120 presents (visited by Elves 1, 2, 3, 6)
 * - House 7: 80 presents (visited by Elves 1, 7)
 * - House 8: 150 presents (visited by Elves 1, 2, 4, 8)
 * - House 9: 130 presents (visited by Elves 1, 3, 9)
 *
 * The goal is to find the lowest house number that receives at least as many 
 * presents as specified in the puzzle input.
 *
 * Puzzle Answer: [Lowest House Number]
 *
 * @author [gabrielzschmitz]
 * @date [09/09/2024]
 */
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

  int puzzle_input;
  std::string line;
  while (std::getline(input_file, line))
    puzzle_input = std::stoi(line);

  if (debug) std::cout << "input: " << puzzle_input << std::endl;

  int limit = puzzle_input / 10;
  std::vector<int> presents(limit, 0);

  for (int elf = 1; elf < limit; ++elf)
    for (int house = elf; house < limit; house += elf)
      presents[house] += elf * 10;

  int total = 0;
  for (int house = 1; house < limit; ++house) {
    if (presents[house] >= puzzle_input) {
      total = house;
      break;
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
