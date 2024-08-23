/**
 * @file part2.cpp
 * @brief Elves' Ribbon Calculation
 *
 * This program helps the elves determine the total amount of ribbon required to
 * wrap all the presents. Each present is a perfect right rectangular prism with
 * given dimensions: length (l), width (w), and height (h). The required ribbon
 * for each gift includes:
 * - The shortest distance around its sides, which is the smallest perimeter of
 *   any one face.
 * - Additional ribbon for a bow, which is equal to the cubic volume of the
 *   present.
 *
 * The task is to calculate the total feet of ribbon needed for all the presents
 * in the list.
 *
 * Example Calculations:
 * - A present with dimensions 2x3x4 requires 10 feet of ribbon to wrap the
 *   present plus 24 feet of ribbon for the bow, for a total of 34 feet.
 * - A present with dimensions 1x1x10 requires 4 feet of ribbon to wrap the
 *   present plus 10 feet of ribbon for the bow, for a total of 14 feet.
 *
 * The program processes the dimensions of each present and outputs the total
 * amount of ribbon required.
 *
 * Puzzle Answer: [Total Feet of Ribbon]
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

    int side1 = std::min(l, std::min(w, h));
    int side2 = (l + w + h) - side1 - std::max(l, std::max(w, h));

    if (debug) std::cout << "sides: " << side1 << " " << side2 << std::endl;

    int smallest_perimeter = 2 * (side1 + side2);
    int cubic_volume = (l * w * h);
    if (debug)
      std::cout << "smallest_perimeter: " << smallest_perimeter
                << " cubic: " << cubic_volume << std::endl;

    int local_total = smallest_perimeter + cubic_volume;

    total += local_total;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
