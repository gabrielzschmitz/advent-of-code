/**
 * @file part2.cpp
 * @brief Enhanced Race Optimization with Extended Cheating Rules
 *
 * This program tackles an advanced version of the racetrack navigation problem.
 * The task involves finding the shortest path from a start ('S') to an end
 * ('E') point in a grid-based racetrack, with an updated "cheat" mechanic that
 * allows collision-free movement for up to 20 picoseconds.
 *
 * **Updated Challenge Description:**
 * - The racetrack map is represented by open paths (.) and walls (#). The race 
 *   begins at 'S' and ends at 'E'.
 * - A single cheat is permitted, during which collision detection is disabled 
 *   for up to 20 picoseconds. The cheat must start and end on open paths.
 * - The goal is to calculate the number of cheats that save at least 100 
 *   picoseconds under these new rules.
 *
 * **Example Input:**
 * ```
 * ###############
 * #...#...#.....#
 * #.#.#.#.#.###.#
 * #S#...#.#.#...#
 * #######.#.#.###
 * #######.#.#...#
 * #######.#.###.#
 * ###..E#...#...#
 * ###.#######.###
 * #...###...#...#
 * #.#####.#.###.#
 * #.#...#.#.#...#
 * #.#.#.#.#.#.###
 * #...#...#...###
 * ###############
 * ```
 *
 * **Example Process:**
 * - Simulate the shortest path without cheats to establish a baseline time.
 * - Explore all possible cheats by testing paths with collision disabled for
 *   varying durations (1 to 20 picoseconds), starting and ending on valid
 *   paths.
 * - Calculate the time saved by each cheat and determine the best cheats.
 * - Count and output the number of cheats that save at least 100 picoseconds.
 *
 * **Example Output:**
 * ```
 * Total Cheats Saving ≥100 Picoseconds: [Result]
 * ```
 *
 * **Steps to Solve:**
 * 1. Parse the input racetrack into a grid representation.
 * 2. Implement a shortest path algorithm (e.g., BFS) to determine the baseline 
 *    race time.
 * 3. Extend the algorithm to simulate cheats of varying durations and calculate 
 *    their impact on the total race time.
 * 4. Identify and count cheats that save at least 100 picoseconds under the new 
 *    rules.
 *
 * **Puzzle Answer:** [Number of Cheats Saving ≥100 Picoseconds]
 *
 * @author [gabrielzschmitz]
 * @date [24/12/2024]
 */
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

constexpr int MAX_STEPS = 20;

struct Point {
  int row;
  int col;
  bool operator==(const Point& p) const { return row == p.row && col == p.col; }
  bool operator<(const Point& p) const {
    return row < p.row || (row == p.row && col < p.col);
  }
};

constexpr std::array<Point, 4> MOVES = {Point{0, 1}, Point{1, 0}, Point{0, -1},
                                        Point{-1, 0}};

bool isInMap(const Point& p, const std::vector<std::string>& map) {
  return p.row >= 0 && p.row < map.size() && p.col >= 0 &&
         p.col < map[0].size();
}

std::pair<std::set<Point>, std::vector<std::vector<int>>> findPath(
  const std::vector<std::string>& map, const Point& start, const Point& end) {
  std::queue<std::pair<Point, int>> queue;
  std::set<Point> visited;
  std::vector<std::vector<int>> costs(map.size(),
                                      std::vector<int>(map[0].size(), 0));
  queue.push({end, 0});

  while (!queue.empty()) {
    auto [current, score] = queue.front();
    queue.pop();

    if (visited.find(current) != visited.end()) continue;
    visited.insert(current);
    costs[current.row][current.col] = score;

    if (current == start) return {visited, costs};

    for (const auto& move : MOVES) {
      Point next{current.row + move.row, current.col + move.col};
      if (isInMap(next, map) && map[next.row][next.col] != '#' &&
          visited.find(next) == visited.end())
        queue.push({next, score + 1});
    }
  }

  return {{}, {}};
}

std::map<int, int> evaluateCheats(const std::vector<std::string>& map,
                                  const std::vector<std::vector<int>>& costs,
                                  const std::set<Point>& path,
                                  const int steps) {
  std::map<int, int> cheatCounts;

  for (const auto& p : path) {
    int baseCost = costs[p.row][p.col];

    for (int dr = -steps; dr <= steps; ++dr) {
      for (int dc = -steps + std::abs(dr); dc <= steps - std::abs(dr); ++dc) {
        Point cheat{p.row + dr, p.col + dc};

        if (path.find(cheat) == path.end()) continue;

        int cheatCost = costs[cheat.row][cheat.col];
        int timeSaved = baseCost - cheatCost - std::abs(dr) - std::abs(dc);

        if (timeSaved > 0) cheatCounts[timeSaved]++;
      }
    }
  }

  return cheatCounts;
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
  std::vector<std::string> map;
  Point start, end;
  while (std::getline(input_file, line)) {
    map.push_back(line);
    for (size_t i = 0; i < line.size(); ++i) {
      if (line[i] == 'S')
        start = {static_cast<int>(map.size() - 1), static_cast<int>(i)};
      else if (line[i] == 'E')
        end = {static_cast<int>(map.size() - 1), static_cast<int>(i)};
    }
  }

  auto [path, costs] = findPath(map, start, end);
  auto cheatCounts = evaluateCheats(map, costs, path, MAX_STEPS);

  uint64_t total = 0;
  for (const auto& [timeSaved, count] : cheatCounts)
    if (timeSaved >= 100) total += count;

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
