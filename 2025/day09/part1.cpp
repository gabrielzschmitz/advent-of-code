/**
 * @file part1.cpp
 * @brief Largest Red-Corner Rectangle
 *
 * You arrive in the North Pole base movie theater, where the Elves are
 * redecorating a giant tiled floor. Some tiles in the grid are marked red,
 * and the Elves want to use these red tiles as **opposite corners** of a
 * rectangle.
 *
 * Your task is to determine the **largest possible rectangle area** that can be
 * formed by choosing **any two red tiles** as opposite corners. The rectangle
 * edges must be aligned with the grid axes, and its area is computed as:
 *
 *      area = |x₂ − x₁| × |y₂ − y₁|
 *
 * Only the two chosen corner tiles need to be red; the rectangle interior can
 * include any tiles.
 *
 * Given:
 *   - A list of coordinates (X,Y), each describing a red tile on the grid.
 *   - Coordinates are integer grid positions.
 *
 * Example:
 *   For the red tiles:
 *       7,1   11,1   11,7   9,7   9,5   2,5   2,3   7,3
 *
 *   The largest possible rectangle in this example has an area of **50**,
 *   using corners at (2,5) and (11,1).
 *
 * Program Goal:
 *   - Consider every pair of red tiles.
 *   - Compute the rectangle area formed by treating them as opposite corners.
 *   - Output the maximum rectangle area found.
 *
 * Puzzle Answer: [Largest Rectangle Area]
 *
 * @author [gabrielzschmitz]
 * @date [09/12/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test")
    debug = true;
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::pair<int64_t, int64_t>> pts;
  std::string line;

  while (getline(input_file, line)) {
    if (line.empty())
      continue;

    int64_t x, y;
    char comma;

    std::stringstream ss(line);
    ss >> x >> comma >> y;
    pts.emplace_back(x, y);
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int64_t total = 0;

  const int n = pts.size();
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      int64_t dx = std::abs(pts[j].first - pts[i].first);
      int64_t dy = std::abs(pts[j].second - pts[i].second);
      int64_t area = (dx + 1) * (dy + 1);

      if (area > total) {
        total = area;
        if (debug) {
          std::cout << "[DEBUG] New max area = " << total << " using ("
                    << pts[i].first << "," << pts[i].second << ") and ("
                    << pts[j].first << "," << pts[j].second << ")\n";
        }
      }
    }
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
