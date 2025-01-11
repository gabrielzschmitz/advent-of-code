/**
 * @file part2.cpp
 * @brief Garden Groups: Bulk Discount Fencing Cost Calculation
 *
 * This program calculates the total cost of fencing for a set of garden plots 
 * under a bulk discount pricing model. Garden plots are represented as a map 
 * where each plot is assigned a type of plant (indicated by a letter). Plots 
 * growing the same type of plant and touching (horizontally or vertically) form 
 * a region.
 *
 * In the bulk discount model, the cost of fencing a region is calculated using:
 * - **Area**, which is the number of garden plots the region contains.
 * - **Number of sides**, where each straight section of the fence is counted 
 *   as a side, regardless of length.
 *
 * The cost for each region is found by multiplying its area by its number of 
 * sides. The total cost is the sum of the costs for all regions on the map.
 *
 * **Key Tasks:**
 * 1. Parse the input map to identify distinct regions.
 * 2. Calculate the area and number of sides for each region.
 * 3. Compute the total fencing cost using the bulk discount model.
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
 * Total fencing cost: 80
 * ```
 *
 * **Example Calculations:**
 * - Region A: Area = 4, Sides = 4, Cost = 4 * 4 = 16
 * - Region B: Area = 4, Sides = 4, Cost = 4 * 4 = 16
 * - Region C: Area = 4, Sides = 8, Cost = 4 * 8 = 32
 * - Region D: Area = 1, Sides = 4, Cost = 1 * 4 = 4
 * - Region E: Area = 3, Sides = 4, Cost = 3 * 4 = 12
 *
 * Total Cost = 16 + 16 + 32 + 4 + 12 = 80.
 *
 * **Challenges:**
 * - Accurately identifying complex regions with internal and external sides.
 * - Calculating sides for irregular shapes efficiently and avoiding overlaps.
 *
 * **Puzzle Answer:** [New Total Fencing Cost]
 *
 * @author [gabrielzschmitz]
 * @date [11/01/2024]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
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

// Directions for flood-fill (up, down, left, right)
const std::vector<std::pair<int, int>> directions = {{0, 1},
                                                     {1, 0},
                                                     {-1, 0},
                                                     {0, -1}};

// Directions for corners
const std::vector<std::pair<double, double>> cornerDirections = {{0.5, -0.5},
                                                                 {0.5, 0.5},
                                                                 {-0.5, 0.5},
                                                                 {-0.5, -0.5}};

bool isInBounds(int x, int y, int m, int n) {
  return x >= 0 && y >= 0 && x < m && y < n;
}

int sides(const std::set<std::pair<int, int>>& region) {
  int sidesCount = 0;
  std::set<std::pair<double, double>> corners;

  for (const auto& cell : region) {
    int cx, cy;
    std::tie(cx, cy) = cell;

    for (const auto& [dx, dy] : cornerDirections)
      corners.insert({cx + dx, cy + dy});
  }

  for (const auto& corner : corners) {
    double cx, cy;
    std::tie(cx, cy) = corner;

    std::vector<bool> adjCells(4, false);
    int n_adj = 0;

    for (int i = 0; i < 4; ++i) {
      double nx = cx + cornerDirections[i].first;
      double ny = cy + cornerDirections[i].second;
      adjCells[i] = region.find({static_cast<int>(nx), static_cast<int>(ny)}) !=
                    region.end();
      if (adjCells[i]) ++n_adj;
    }

    if (n_adj == 1 || n_adj == 3) sidesCount += 1;
    else if (adjCells == std::vector<bool>({true, false, true, false}) ||
             adjCells == std::vector<bool>({false, true, false, true}))
      sidesCount += 2;
  }

  return sidesCount;
}

void floodFill(int x, int y, const std::vector<std::vector<char>>& plants,
               std::vector<std::vector<bool>>& visited, char plantType,
               std::set<std::pair<int, int>>& region) {
  int m = plants.size();
  int n = plants[0].size();
  std::queue<std::pair<int, int>> q;
  q.push({x, y});
  visited[x][y] = true;

  while (!q.empty()) {
    auto [cx, cy] = q.front();
    q.pop();
    region.insert({cx, cy});

    if (debug)
      std::cout << "Visited: (" << cx << ", " << cy << ")" << std::endl;

    for (const auto& [dx, dy] : directions) {
      int nx = cx + dx;
      int ny = cy + dy;
      if (isInBounds(nx, ny, m, n) && !visited[nx][ny] &&
          plants[nx][ny] == plantType) {
        visited[nx][ny] = true;
        q.push({nx, ny});
        if (debug)
          std::cout << "Added to queue: (" << nx << ", " << ny << ")"
                    << std::endl;
      }
    }
  }
  if (debug)
    std::cout << "Region flood-filled. Total cells: " << region.size()
              << std::endl;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::vector<char>> plants;
  std::string line;
  while (std::getline(input_file, line))
    plants.push_back(std::vector<char>(line.begin(), line.end()));

  int m = plants.size();
  int n = plants[0].size();
  std::vector<std::vector<bool>> visited(m, std::vector<bool>(n, false));
  uint64_t total = 0;

  for (int x = 0; x < m; ++x) {
    for (int y = 0; y < n; ++y) {
      if (!visited[x][y]) {
        char plantType = plants[x][y];
        std::set<std::pair<int, int>> region;
        if (debug)
          std::cout << "Starting flood fill for plant: " << plantType << " at ("
                    << x << ", " << y << ")" << std::endl;
        floodFill(x, y, plants, visited, plantType, region);

        int area = region.size();
        int perimeter = 4 * area - region.size();
        int regionSides = sides(region);

        total += static_cast<uint64_t>(area) * regionSides;
        if (debug)
          std::cout << "Region (" << plantType << ") - Area: " << area
                    << ", Sides: " << regionSides
                    << ", Perimeter: " << perimeter << std::endl;
      }
    }
  }

  std::cout << "ANSWER: " << total << std::endl;

  return 0;
}
