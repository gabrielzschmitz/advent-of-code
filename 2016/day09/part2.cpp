/**
 * @file part2.cpp
 * @brief Calculating the Recursive Decompressed Length of an Experimental File
 *
 * This program determines the decompressed length of a file encoded using
 * version two of an experimental compression format. The input consists of a
 * sequence of characters containing optional repetition markers. Whitespace is
 * ignored.
 *
 * A repetition marker has the form (AxB), where:
 *   - A specifies the number of subsequent characters to process.
 *   - B specifies how many times the decompressed result of those characters
 *     should be repeated.
 *
 * Unlike the original format, any markers contained within the referenced
 * characters are recursively decompressed before their length is multiplied.
 * As a result, the decompressed output may be extremely large, making it
 * impractical to construct the full string in memory.
 *
 * Instead, the program recursively evaluates the contribution of each marker
 * and computes only the total decompressed length.
 *
 * Example Input:
 * (3x3)XYZ
 * X(8x2)(3x3)ABCY
 * (27x12)(20x12)(13x14)(7x10)(1x12)A
 * (25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN
 *
 * Example Output:
 * 9
 * 20
 * 241920
 * 445
 *
 * The program processes the compressed file using recursive marker expansion
 * and outputs the total decompressed length without generating the expanded
 * data.
 *
 * Puzzle Answer: [Recursive Decompressed Length]
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

std::uint64_t decompressedLength(const std::string& s, std::size_t begin,
                                 std::size_t end) {
  std::uint64_t length = 0;

  for (std::size_t i = begin; i < end;) {
    if (std::isspace(static_cast<unsigned char>(s[i]))) {
      ++i;
      continue;
    }

    if (s[i] != '(') {
      ++length;
      ++i;
      continue;
    }

    ++i;  // Skip '('

    std::size_t chars = 0;
    while (std::isdigit(static_cast<unsigned char>(s[i]))) {
      chars = chars * 10 + (s[i] - '0');
      ++i;
    }

    ++i;  // Skip 'x'

    std::size_t repeat = 0;
    while (std::isdigit(static_cast<unsigned char>(s[i]))) {
      repeat = repeat * 10 + (s[i] - '0');
      ++i;
    }

    ++i;  // Skip ')'

    std::size_t segmentBegin = i;
    std::size_t segmentEnd = i + chars;

    length += repeat * decompressedLength(s, segmentBegin, segmentEnd);

    i = segmentEnd;
  }

  return length;
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
  std::getline(input_file, line);

  std::uint64_t answer = 0;
  answer = decompressedLength(line, 0, line.size());

  std::cout << "ANSWER: " << answer << '\n';

  return 0;
}
