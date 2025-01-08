/**
 * @file part2.cpp
 * @brief Trailhead Rating Calculation
 *
 * This program evaluates trailheads on a topographic map to calculate their
 * ratings. A trailhead is a position with height 0 that can start one or more
 * hiking trails. A hiking trail is defined as:
 * - Starting at height 0 and ending at height 9.
 * - Following an even, gradual uphill slope (each step increases by exactly 1).
 * - Moving only in cardinal directions (up, down, left, or right).
 *
 * **Part 2 Extension:**
 * The rating of a trailhead is the total number of distinct hiking trails that
 * begin at that trailhead and lead to any height-9 position. Each trail is
 * counted separately even if they reach the same height-9 position.
 *
 * **Task:**
 * 1. Parse the input topographic map.
 * 2. Identify all trailhead positions (height 0).
 * 3. For each trailhead:
 *    - Perform a traversal to count all distinct hiking trails to height-9
 *      positions.
 * 4. Sum the ratings for all trailheads and output the result.
 *
 * **Example Input:**
 * ```
 * 89010123
 * 78121874
 * 87430965
 * 96549874
 * 45678903
 * 32019012
 * 01329801
 * 10456732
 * ```
 *
 * **Example Output:**
 * ```
 * Sum of Trailhead Ratings: 81
 * ```
 *
 * **Steps to Solve:**
 * 1. Parse the 2D grid into a suitable data structure.
 * 2. Traverse the grid to identify trailheads (height 0).
 * 3. For each trailhead:
 *    - Use a graph traversal (e.g., BFS/DFS) to explore all valid hiking
 *    trails.
 *    - Maintain a set of visited trails to ensure distinct trails are counted.
 *    - Count and sum the number of distinct trails reaching height-9 positions.
 * 4. Sum the ratings for all trailheads and output the total.
 *
 * **Constraints:**
 * - The input map is a rectangular grid.
 * - Hiking trails cannot revisit the same position on a single trail.
 *
 * **Puzzle Answer:** [Sum of Trailhead Ratings]
 *
 * @author [gabrielzschmitz]
 * @date [08/01/2024]
 */
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <queue>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

int countDistinctTrailsToHeight9(const std::vector<std::string>& grid, int
    startX, int startY) {
    std::queue<std::vector<std::pair<int, int>>> q;
    q.push({{startX, startY}});

    int rows = grid.size();
    int cols = grid[0].size();
    std::set<std::vector<std::pair<int, int>>> uniqueTrails;

    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0,
      1}};

    while (!q.empty()) {
      auto trail = q.front();
      q.pop();

      auto [x, y] = trail.back();

      for (const auto& [dx, dy] : directions) {
        int nx = x + dx;
        int ny = y + dy;

        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
          int currentHeight = grid[x][y] - '0';
          int nextHeight = grid[nx][ny] - '0';

          if (nextHeight == 9 && currentHeight == 8) {
            auto newTrail = trail;
            newTrail.push_back({nx, ny});
            uniqueTrails.insert(newTrail);
          } else if (nextHeight == currentHeight + 1 && std::find(trail.begin(),
                trail.end(), std::make_pair(nx, ny)) == trail.end()) {
            auto newTrail = trail;
            newTrail.push_back({nx, ny});
            q.push(newTrail);
          }
        }
      }
    }

    return uniqueTrails.size();
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::string> grid;
  std::string line;
  while (std::getline(input_file, line))
    grid.push_back(line);

  uint64_t total = 0;

  for (int i = 0; i < grid.size(); ++i)
    for (int j = 0; j < grid[i].size(); ++j)
      if (grid[i][j] == '0')
        total += countDistinctTrailsToHeight9(grid, i, j);

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
