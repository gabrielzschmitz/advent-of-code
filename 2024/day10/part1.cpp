/**
 * @file part1.cpp
 * @brief Hiking Trailhead Scoring
 *
 * This program identifies and evaluates trailheads on a topographic map to
 * determine the sum of their scores. A trailhead is any position with height 0
 * that starts one or more valid hiking trails. A hiking trail is defined as:
 * - Starting at height 0 and ending at height 9.
 * - Following an even, gradual uphill slope (each step increases by exactly 1).
 * - Moving only in cardinal directions (up, down, left, or right).
 *
 * **Challenge Description:**
 * - The map is represented as a 2D grid of heights (0-9).
 * - Positions marked with height 0 are potential trailheads.
 * - Each trailhead's score is determined by counting the number of height-9
 *   positions reachable via valid hiking trails.
 * - Impassable tiles do not appear in the actual input map but may be used in
 *   examples.
 *
 * **Task:**
 * 1. Parse the input topographic map.
 * 2. Identify all trailhead positions (height 0).
 * 3. For each trailhead, explore all valid hiking trails using breadth-first
 *    or depth-first search to count the reachable height-9 positions.
 * 4. Sum the scores of all trailheads and output the result.
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
 * Sum of Trailhead Scores: 36
 * ```
 *
 * **Steps to Solve:**
 * 1. Parse the 2D grid into a suitable data structure.
 * 2. Traverse the grid to identify trailheads (height 0).
 * 3. For each trailhead:
 *    - Perform a graph traversal (e.g., BFS/DFS) to find all reachable height-9
 *    positions via valid hiking trails.
 *    - Avoid revisiting positions and handle map boundaries carefully.
 * 4. Sum the scores for all trailheads and output the total.
 *
 * **Constraints:**
 * - The input map is a rectangular grid.
 * - The number of trailheads and valid hiking trails may vary based on the map.
 *
 * **Puzzle Answer:** [Sum of Trailhead Scores]
 *
 * @author [gabrielzschmitz]
 * @date [08/01/2024]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

int countReachableHeight9(const std::vector<std::string>& grid, int startX, int
    startY) {
    std::queue<std::pair<int, int>> q;
    q.push({startX, startY});

    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols,
          false));
    visited[startX][startY] = true;

    int reachableHeight9Count = 0;

    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0,
      1}};

    while (!q.empty()) {
      auto [x, y] = q.front();
      q.pop();

      for (const auto& [dx, dy] : directions) {
        int nx = x + dx;
        int ny = y + dy;

        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && !visited[nx][ny]) {
          int currentHeight = grid[x][y] - '0';
          int nextHeight = grid[nx][ny] - '0';

          if (nextHeight == 9 && currentHeight == 8)
            reachableHeight9Count++;

          if (nextHeight == currentHeight + 1) {
            q.push({nx, ny});
            visited[nx][ny] = true;
          }
        }
      }
    }

    return reachableHeight9Count;
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
        total += countReachableHeight9(grid, i, j);

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
