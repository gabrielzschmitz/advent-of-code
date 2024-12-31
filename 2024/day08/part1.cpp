/**
 * @file part1.cpp
 * @brief Antenna Resonance and Antinode Calculation
 *
 * This program analyzes a map of antennas to calculate the impact of their
 * emitted signals by identifying unique locations that contain antinodes. An
 * antenna emits a signal at a specific frequency, and antinodes occur when
 * certain conditions of collinearity and distance are met.
 *
 * **Challenge Description:**
 * - Antennas are placed on a 2D grid and emit signals at specific frequencies
 *   (represented as lowercase letters, uppercase letters, or digits).
 * - Antinodes are created at positions perfectly in line with two antennas of
 *   the same frequency, with one antenna being twice as far away from the
 *   position as the other.
 * - Each pair of antennas with the same frequency contributes two antinodes, 
 *   one on either side of the line connecting them.
 * - Antinodes can overlap antenna positions and are frequency-specific.
 * - Antennas with different frequencies do not create shared antinodes.
 *
 * **Task:**
 * 1. Parse the map to identify antenna positions and their frequencies.
 * 2. For each pair of antennas with the same frequency, calculate potential 
 *    antinode positions based on the resonance rule.
 * 3. Determine the unique set of positions within the map bounds that contain
 * antinodes.
 *
 * **Example Input:**
 * ```
 * ............
 * ........0...
 * .....0......
 * .......0....
 * ....0.......
 * ......A.....
 * ............
 * ............
 * ........A...
 * .........A..
 * ............
 * ............
 * ```
 *
 * **Example Output:**
 * ```
 * Total Unique Antinode Locations: 14
 * ```
 *
 * **Steps to Solve:**
 * 1. Parse the grid and record the coordinates and frequencies of all antennas.
 * 2. Iterate through each pair of antennas with the same frequency.
 *    - Calculate potential antinode positions based on collinearity and
 *    distance rules.
 * 3. Collect all antinode positions within the map bounds, ensuring uniqueness.
 * 4. Count and output the total number of unique antinode positions.
 *
 * **Constraints:**
 * - Antinodes must be within the bounds of the input grid.
 * - Only exact matches of frequency contribute to antinode creation.
 *
 * **Puzzle Answer:** [Total Unique Antinode Locations]
 *
 * @author [gabrielzschmitz]
 * @date [31/12/2024]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

struct Antenna {
  int x, y;
  char frequency;
};

bool isWithinBounds(int x, int y, int width, int height) {
  return x >= 0 && y >= 0 && x < width && y < height;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<Antenna> antennas;
  std::string line;
  int row = 0, grid_width = 0, grid_height = 0;

  while (std::getline(input_file, line)) {
    grid_width = line.size();
    for (int col = 0; col < line.size(); ++col) {
      char c = line[col];
      if (std::isalnum(c) && c != '.') antennas.push_back({col, row, c});
    }
    ++row;
  }
  grid_height = row;

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::set<std::pair<int, int>> unique_antinodes;

  for (size_t i = 0; i < antennas.size(); ++i) {
    for (size_t j = i + 1; j < antennas.size(); ++j) {
      if (antennas[i].frequency == antennas[j].frequency) {
        int dx = antennas[j].x - antennas[i].x;
        int dy = antennas[j].y - antennas[i].y;

        int antinode1_x = antennas[i].x - dx;
        int antinode1_y = antennas[i].y - dy;
        int antinode2_x = antennas[j].x + dx;
        int antinode2_y = antennas[j].y + dy;

        if (isWithinBounds(antinode1_x, antinode1_y, grid_width, grid_height))
          unique_antinodes.emplace(antinode1_x, antinode1_y);
        if (isWithinBounds(antinode2_x, antinode2_y, grid_width, grid_height))
          unique_antinodes.emplace(antinode2_x, antinode2_y);
      }
    }
  }
  uint64_t total = unique_antinodes.size();

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
