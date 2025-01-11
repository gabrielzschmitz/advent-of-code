/**
 * @file part1.cpp
 * @brief Garden Groups: Fencing Cost Calculation
 *
 * This program calculates the total cost of fencing for a set of garden plots. 
 * The garden plots are represented as a map where each plot is assigned a type 
 * of plant (indicated by a letter). Plots growing the same type of plant and 
 * touching (horizontally or vertically) form a region.
 *
 * Each region has:
 * - An **area**, which is the number of garden plots it contains.
 * - A **perimeter**, which is the number of sides of the plots in the region
 *   that do not touch another plot in the same region.
 *
 * The cost of fencing a region is calculated by multiplying its area by its 
 * perimeter. The total cost is the sum of the costs of fencing all regions on
 * the map.
 *
 * **Key Tasks:**
 * 1. Parse the input map to identify distinct regions.
 * 2. Calculate the area and perimeter for each region.
 * 3. Compute the total fencing cost by summing the cost of all regions.
 *
 * **Example Input:**
 * ```
 * AAAA
 * BBCD
 * BBCC
 * EEEC
 * ```
 * 
 * **Example Output:**
 * ```
 * Total fencing cost: 140
 * ```
 *
 * **Example Calculations:**
 * - Region A: Area = 4, Perimeter = 10, Cost = 4 * 10 = 40
 * - Region B: Area = 4, Perimeter = 8, Cost = 4 * 8 = 32
 * - Region C: Area = 4, Perimeter = 10, Cost = 4 * 10 = 40
 * - Region D: Area = 1, Perimeter = 4, Cost = 1 * 4 = 4
 * - Region E: Area = 3, Perimeter = 8, Cost = 3 * 8 = 24
 *
 * Total Cost = 40 + 32 + 40 + 4 + 24 = 140.
 *
 * **Challenges:**
 * - Efficiently identifying distinct regions in large maps.
 * - Accurately calculating the perimeter for irregularly shaped regions.
 *
 * **Puzzle Answer:** [Total Fencing Cost]
 *
 * @author [gabrielzschmitz]
 * @date [11/01/2024]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

// Directions for moving up, down, left, and right
const std::vector<std::pair<int, int>> directions = {{-1, 0},
                                                     {1, 0},
                                                     {0, -1},
                                                     {0, 1}};

bool isInBounds(int x, int y, int rows, int cols) {
  return x >= 0 && y >= 0 && x < rows && y < cols;
}

void floodFill(const std::vector<std::string>& map, int x, int y,
               std::vector<std::vector<bool>>& visited, char plantType,
               int& area, int& perimeter) {
  int rows = map.size();
  int cols = map[0].size();
  std::queue<std::pair<int, int>> q;
  q.push({x, y});
  visited[x][y] = true;

  while (!q.empty()) {
    auto [cx, cy] = q.front();
    q.pop();
    area++;

    for (const auto& [dx, dy] : directions) {
      int nx = cx + dx;
      int ny = cy + dy;

      if (isInBounds(nx, ny, rows, cols)) {
        if (map[nx][ny] == plantType && !visited[nx][ny]) {
          visited[nx][ny] = true;
          q.push({nx, ny});
        } else if (map[nx][ny] != plantType) perimeter++;
      } else perimeter++;
    }
  }
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::string> map;
  std::string line;
  while (std::getline(input_file, line))
    map.push_back(line);

  if (debug) {
    std::cout << "Input Map:" << std::endl;
    for (const auto& row : map)
      std::cout << row << std::endl;
    std::cout << "\n";
  }

  int rows = map.size();
  int cols = map[0].size();
  std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
  uint64_t total = 0;

  for (int x = 0; x < rows; ++x) {
    for (int y = 0; y < cols; ++y) {
      if (!visited[x][y]) {
        char plantType = map[x][y];
        int area = 0;
        int perimeter = 0;

        floodFill(map, x, y, visited, plantType, area, perimeter);

        uint64_t cost = static_cast<uint64_t>(area) * perimeter;
        total += cost;

        if (debug)
          std::cout << "Region " << plantType << ": Area = " << area
                    << ", Perimeter = " << perimeter << ", Cost = " << cost
                    << std::endl;
      }
    }
  }
  if (debug) std::cout << "\n";

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
