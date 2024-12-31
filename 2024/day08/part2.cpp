/**
 * @file part2.cpp
 * @brief Resonant Harmonics and Enhanced Antinode Calculation
 *
 * This program extends the previous calculation of antinodes by incorporating
 * the effects of resonant harmonics, which allow any grid position exactly in
 * line with at least two antennas of the same frequency to become an antinode.
 * This updated model includes additional antinodes occurring at the positions
 * of antennas themselves, provided they are aligned with at least one other
 * antenna of the same frequency.
 *
 * **Challenge Description:**
 * - Antennas are placed on a 2D grid, each emitting signals at a specific
 *   frequency (lowercase letters, uppercase letters, or digits).
 * - Antinodes occur at any grid position exactly in line with at least two
 *   antennas of the same frequency, irrespective of their distances.
 * - Each antenna position can also be an antinode if it aligns with another
 *   antenna of the same frequency.
 *
 * **Task:**
 * 1. Parse the grid to locate antennas and identify their frequencies.
 * 2. For every unique pair of antennas with the same frequency:
 *    - Calculate all possible antinode positions along their line of alignment.
 * 3. Add antenna positions as antinodes if they align with at least one other
 * antenna.
 * 4. Determine the total number of unique antinodes within the bounds of the
 * grid.
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
 * Total Unique Antinode Locations: 34
 * ```
 *
 * **Steps to Solve:**
 * 1. Parse the grid to record antenna positions and frequencies.
 * 2. Iterate through all pairs of antennas with the same frequency:
 *    - Calculate every grid position that lies along the line connecting them.
 *    - Include all calculated positions as potential antinodes.
 * 3. Include all antenna positions that align with at least one other antenna
 * of the same frequency as antinodes.
 * 4. Ensure all antinode positions are unique and within grid bounds.
 * 5. Count and output the total number of unique antinode positions.
 *
 * **Constraints:**
 * - Only grid positions within the bounds of the input map are considered.
 * - Each frequency is treated independently; no mixing of frequencies occurs.
 *
 * **Puzzle Answer:** [Total Unique Antinode Locations]
 *
 * @author [gabrielzschmitz]
 * @date [31/12/2024]
 */
#include <algorithm>
#include <cmath>
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

int gcd(int a, int b) {
  return b == 0 ? std::abs(a) : gcd(b, a % b);
}

struct Position {
  int x, y;

  Position operator+(const Position& other) const {
    return {x + other.x, y + other.y};
  }

  Position operator-(const Position& other) const {
    return {x - other.x, y - other.y};
  }

  bool isInBounds(int width, int height) const {
    return x >= 0 && y >= 0 && x < width && y < height;
  }

  Position normalize() const {
    int g = gcd(x, y);
    return {x / g, y / g};
  }

  bool operator<(const Position& other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
};

struct Antenna {
  Position pos;
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

  std::set<Position> unique_antinodes;

  for (size_t i = 0; i < antennas.size(); ++i) {
    for (size_t j = i + 1; j < antennas.size(); ++j) {
      if (antennas[i].frequency == antennas[j].frequency) {
        Position a1 = antennas[i].pos;
        Position a2 = antennas[j].pos;
        Position vec = a2 - a1;

        Position anti1 = a2 + vec;
        Position anti2 = a1 - vec;
        if (anti1.isInBounds(grid_width, grid_height))
          unique_antinodes.insert(anti1);
        if (anti2.isInBounds(grid_width, grid_height))
          unique_antinodes.insert(anti2);

        Position step = vec.normalize();
        Position temp = a1;
        while (temp.isInBounds(grid_width, grid_height)) {
          unique_antinodes.insert(temp);
          temp = temp + step;
        }
        temp = a1;
        while (temp.isInBounds(grid_width, grid_height)) {
          unique_antinodes.insert(temp);
          temp = temp - step;
        }
      }
    }
  }
  uint64_t total = unique_antinodes.size();

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
