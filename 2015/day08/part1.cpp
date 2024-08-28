/**
 * @file part1.cpp
 * @brief Santa's Digital List Storage Calculation
 *
 * Santa needs to determine how much space his digital list will occupy on the
 * sleigh. The list consists of multiple double-quoted string literals, each
 * containing escaped characters. The task is to calculate the difference
 * between the number of characters in the code representation of the string
 * literals and the number of characters in the in-memory string data.
 *
 * The instructions are given as a series of string literals where:
 * - Each string is enclosed in double quotes ("").
 * - Escape sequences include:
 *   - \\ representing a single backslash.
 *   - \" representing a double-quote character.
 *   - \x followed by two hexadecimal characters representing a single ASCII
 *   character.
 *
 * The task is to compute:
 * 1. The total number of characters in the code representation of the string
 * literals. 2. The total number of characters in memory after processing escape
 * sequences. 3. The difference between these two totals for all strings in the
 * input file.
 *
 * Example Input:
 * ""          // 2 characters in code, 0 characters in memory
 * "abc"       // 5 characters in code, 3 characters in memory
 * "aaa\"aaa"  // 10 characters in code, 7 characters in memory
 * "\x27"      // 6 characters in code, 1 character in memory
 *
 * The program processes each string and outputs the total difference between
 * the number of characters in code and the number of characters in memory.
 *
 * Puzzle Answer: [Difference in Characters]
 *
 * @author [gabrielzschmitz]
 * @date [28/08/2024]
 */
#include <cmath>
#include <cstring>
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
  int total_code_characters = 0;
  int total_memory_characters = 0;

  while (std::getline(input_file, line)) {
    int code_characters = line.size();
    int memory_characters = 0;

    for (size_t i = 1; i < line.size() - 1; i++) {
      if (line[i] == '\\') {
        if (line[i + 1] == 'x' && i + 3 < line.size()) {
          memory_characters++;
          i += 3;
        } else if (line[i + 1] == '\\' || line[i + 1] == '\"') {
          memory_characters++;
          i++;
        }
      } else memory_characters++;
    }

    total_code_characters += code_characters;
    total_memory_characters += memory_characters;

    if (debug)
      std::cout << "input: " << line << " +" << code_characters << " -"
                << memory_characters << std::endl;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "total: " << total_code_characters << " - "
            << total_memory_characters << std::endl;

  int total = total_code_characters - total_memory_characters;
  std::cout << "ANSWER: " << total << std::endl;

  return 0;
}
