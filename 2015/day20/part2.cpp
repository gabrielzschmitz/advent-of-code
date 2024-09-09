/**
 * @file part2.cpp
 * @brief Modified Elf House Delivery with Limited Visits
 *
 * In this extension of the problem, the Elves have altered their strategy for
 * delivering presents to houses. Instead of visiting an infinite number of
 * houses, each Elf now visits exactly 50 houses. Additionally, to compensate
 * for visiting fewer houses, the number of presents they deliver to each house
 * is increased to 11 times their number.
 *
 * - Each Elf will visit 50 houses and deliver presents equal to 11 times their
 *   number to each house.
 * 
 * For example:
 * - Elf 1 delivers 11 presents to houses 1 through 50.
 * - Elf 2 delivers 22 presents to houses 2 through 52.
 * - Elf 3 delivers 33 presents to houses 3 through 53, and so on.
 *
 * The objective is to determine the lowest house number that receives at least
 * as many presents as specified in the puzzle input, given this new set of
 * rules.
 *
 * Example of houses with this new rule:
 * - House 1: receives presents from Elf 1
 * - House 2: receives presents from Elves 1, 2
 * - House 3: receives presents from Elves 1, 3
 * - House 4: receives presents from Elves 1, 2, 4
 * - ...
 *
 * The goal is to find the lowest house number that receives at least the
 * required number of presents as specified in the puzzle input.
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
    for (int house = elf, count = 0; house < limit && count < 50;
         house += elf, ++count)
      presents[house] += elf * 11;

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
