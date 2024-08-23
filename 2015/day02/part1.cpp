/**
 * @file part1.cpp
 * @brief Elves' Wrapping Paper Calculation
 *
 * This program assists the elves in determining the total amount of wrapping
 * paper required for all the presents. Each present is in the shape of a
 * perfect right rectangular prism, with given dimensions: length (l), width
 * (w), and height (h). The required wrapping paper for each gift includes:
 * - The surface area of the box: 2*l*w + 2*w*h + 2*h*l.
 * - An additional slack amount equal to the area of the smallest side.
 *
 * The task is to calculate the total square feet of wrapping paper needed for
 * all the presents in the list.
 *
 * Example Calculations:
 * - A present with dimensions 2x3x4 requires 52 square feet of wrapping paper
 *   plus 6 square feet of slack, for a total of 58 square feet.
 * - A present with dimensions 1x1x10 requires 42 square feet of wrapping paper
 *   plus 1 square foot of slack, for a total of 43 square feet.
 *
 * The program processes the dimensions of each present and outputs the total
 * amount of wrapping paper required.
 *
 * Puzzle Answer: [Total Square Feet of Wrapping Paper]
 *
 * @author [gabrielzschmitz]
 * @date [22/08/2024]
 */
#include <limits.h>
#include <algorithm>
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

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string line;
  int total = 0;
  while (std::getline(input_file, line)) {
    int l, w, h;
    char delimiter;
    std::istringstream stream(line);
    stream >> l >> delimiter >> w >> delimiter >> h;

    int local_total = (2 * l * w) + (2 * w * h) + (2 * h * l);

    int lw = l * w;
    int wh = w * h;
    int hl = h * l;
    int smallest_side_area = std::min({lw, wh, hl});

    total += local_total + smallest_side_area;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
