/**
 * @file part1.cpp
 * @brief Calculating the Decompressed Length of an Experimental File
 *
 * This program determines the decompressed length of a file encoded using an
 * experimental compression format. The input consists of a sequence of
 * characters containing optional repetition markers. Whitespace is ignored.
 *
 * A repetition marker has the form (AxB), where:
 *   - A specifies the number of subsequent characters to repeat.
 *   - B specifies how many times those characters should be repeated.
 *
 * When a marker is encountered, the next A characters are copied B times into
 * the decompressed output. Any parentheses or marker-like text appearing
 * within those A characters are treated as ordinary data and are not processed
 * as additional markers.
 *
 * The program scans the compressed input from left to right, applies each
 * marker exactly once, and computes the total length of the decompressed data
 * without explicitly constructing the decompressed string.
 *
 * Example Input:
 * ADVENT
 * A(1x5)BC
 * (3x3)XYZ
 * A(2x2)BCD(2x2)EFG
 * (6x1)(1x3)A
 * X(8x2)(3x3)ABCY
 *
 * Example Output:
 * 6
 * 7
 * 9
 * 11
 * 6
 * 18
 *
 * The program processes the compressed file and outputs its decompressed
 * length, excluding all whitespace.
 *
 * Puzzle Answer: [Decompressed Length]
 *
 * @author [gabrielzschmitz]
 * @date [18/07/2026]
 */
#include <cctype>
#include <cstdint>
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

  std::string line;
  std::uint64_t answer = 0;

  std::getline(input_file, line);
  for (std::size_t i = 0; i < line.size();) {
    if (std::isspace(static_cast<unsigned char>(line[i]))) {
      ++i;
      continue;
    }

    if (line[i] != '(') {
      ++answer;
      ++i;
      continue;
    }

    // Parse marker (AxB)
    ++i;  // Skip '('

    std::size_t chars = 0;
    while (std::isdigit(static_cast<unsigned char>(line[i]))) {
      chars = chars * 10 + (line[i] - '0');
      ++i;
    }

    ++i;  // Skip 'x'

    std::size_t repeat = 0;
    while (std::isdigit(static_cast<unsigned char>(line[i]))) {
      repeat = repeat * 10 + (line[i] - '0');
      ++i;
    }

    ++i;  // Skip ')'

    answer += chars * repeat;

    if (debug) {
      std::cout << "Marker: (" << chars << "x" << repeat << ") -> +"
                << chars * repeat << '\n';
    }

    // Skip the consumed characters.
    i += chars;
  }

  std::cout << "ANSWER: " << answer << '\n';

  return 0;
}
