/**
 * @file part1.cpp
 * @brief Weather Machine Code Generation
 *
 * In this program, you help Santa boot up his weather machine by calculating a
 * specific code needed to bypass its copy protection. The codes are generated
 * following a specific algorithm and are organized on an infinite sheet of
 * paper, filled diagonally by rows and columns. The first code is 20151125, and
 * each subsequent code is generated using the previous code multiplied by
 * 252533, with the result taken modulo 33554393.
 *
 * The task is to compute the code for a specific location on the sheet, given
 * the puzzle input, which consists of the row and column numbers. The location
 * is identified based on how the diagonal filling process progresses.
 *
 * Example Code Sequence:
 * Starting Code: 20151125
 * Second Code: (20151125 * 252533) % 33554393 = 31916031
 * Third Code: (31916031 * 252533) % 33554393 = 18749137
 * 
 * Example Grid Progression:
 *   | 1   2   3   4   5   6  
 * ---+---+---+---+---+---+---+
 *  1 |  1   3   6  10  15  21
 *  2 |  2   5   9  14  20
 *  3 |  4   8  13  19
 *  4 |  7  12  18
 *  5 | 11  17
 *  6 | 16
 *
 * The program processes the row and column input, computes the sequence number
 * in the diagonal order, and then calculates the corresponding code.
 *
 * Puzzle Answer: [Code at Given Location]
 *
 * @author [gabrielzschmitz]
 * @date [14/09/2024]
 */
#include <algorithm>
#include <cstdint>
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

const uint64_t START_CODE = 20151125;
const uint64_t MULTIPLIER = 252533;
const uint64_t MODULO = 33554393;

uint64_t calculateSequenceNumber(int row, int col) {
  uint64_t diagonal = row + col - 1;

  uint64_t sequence_number = (diagonal * (diagonal - 1)) / 2 + col;

  return sequence_number;
}

uint64_t generateCode(uint64_t sequence_number) {
  uint64_t code = START_CODE;

  for (uint64_t i = 1; i < sequence_number; ++i)
    code = (code * MULTIPLIER) % MODULO;

  return code;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string line;
  int row = 0, col = 0;
  while (std::getline(input_file, line)) {
    if (line.find("row:") != std::string::npos)
      row = std::stoi(line.substr(line.find(":") + 1));
    else if (line.find("column:") != std::string::npos)
      col = std::stoi(line.substr(line.find(":") + 1));
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  uint64_t sequence_number = calculateSequenceNumber(row, col);
  uint64_t code = generateCode(sequence_number);

  uint64_t total = code;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
