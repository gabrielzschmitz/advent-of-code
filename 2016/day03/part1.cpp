/**
 * @file part1.cpp
 * @brief Valid Triangle Counter
 *
 * This program helps Santa determine how many valid triangles are described
 * in a design document found in the labyrinth of Easter Bunny HQ. Each line
 * in the input specifies the lengths of three sides of a triangle.
 *
 * A triangle is considered valid if the sum of any two sides is greater than
 * the remaining side. This rule must hold true for all three combinations of
 * sides.
 *
 * The task is to count how many of the listed triangles in the input file
 * are valid based on this rule.
 *
 * Example Input:
 * 5 10 25     // Invalid: 5 + 10 <= 25
 * 3 4 5       // Valid
 * 7 10 12     // Valid
 *
 * The program processes each line, checks the triangle inequality condition,
 * and counts how many valid triangles are present.
 *
 * Puzzle Answer: [Number of Valid Triangles]
 *
 * @author [gabrielzschmitz]
 * @date [21/04/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

bool is_valid_triangle(int a, int b, int c) {
  return (a + b > c) && (a + c > b) && (b + c > a);
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::int64_t total = 0;
  std::string line;

  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    int a, b, c;
    if (!(iss >> a >> b >> c)) continue;

    if (is_valid_triangle(a, b, c)) {
      ++total;
      if (debug) std::cout << "Valid: " << a << " " << b << " " << c << "\n";
    } else {
      if (debug) std::cout << "Invalid: " << a << " " << b << " " << c << "\n";
    }
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
