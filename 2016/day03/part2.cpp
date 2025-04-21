/**
 * @file part2.cpp
 * @brief Vertical Triangle Validator
 *
 * In this variation of triangle validation, the side lengths are no longer
 * grouped by rows, but rather by columns. Every three rows in the input form
 * a group, and the columns within that group define individual triangles.
 *
 * For each set of three consecutive rows, three triangles are formed:
 * - The first triangle is composed of the first number in each row.
 * - The second triangle uses the second number from each row.
 * - The third triangle uses the third number from each row.
 *
 * A triangle is valid if the sum of any two sides is greater than the third.
 * This rule must be satisfied for all side combinations.
 *
 * Example Input:
 * 101 301 501
 * 102 302 502
 * 103 303 503
 *
 * Valid triangles formed:
 * - (101, 102, 103)
 * - (301, 302, 303)
 * - (501, 502, 503)
 *
 * The program processes the input by column groups and counts how many valid
 * triangles exist using the triangle inequality rule.
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
#include <vector>

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
  std::vector<std::vector<int>> buffer;

  std::string line;
  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    int a, b, c;
    if (!(iss >> a >> b >> c)) continue;
    buffer.push_back({a, b, c});

    if (buffer.size() == 3) {
      // Process by columns
      for (int i = 0; i < 3; ++i) {
        int x = buffer[0][i];
        int y = buffer[1][i];
        int z = buffer[2][i];
        if (is_valid_triangle(x, y, z)) {
          ++total;
          if (debug)
            std::cout << "Valid: " << x << " " << y << " " << z << "\n";
        } else {
          if (debug)
            std::cout << "Invalid: " << x << " " << y << " " << z << "\n";
        }
      }
      buffer.clear();
    }
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
