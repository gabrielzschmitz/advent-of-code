/**
 * @file part1.cpp
 * @brief Optimizing Race Conditions with Strategic Cheats
 *
 * This program navigates a racetrack represented as a grid, finding the fastest
 * route from a start ('S') to an end ('E') point. Each move (up, down, left,
 * right) takes 1 picosecond. The challenge introduces a "cheat" mechanic to
 * improve the time by passing through walls for up to 2 picoseconds once during
 * the race.
 *
 * **Challenge Description:**
 * - The map consists of open paths (.) and walls (#). The race begins at 'S' and 
 *   ends at 'E'.
 * - A program may cheat exactly once during a race, disabling collision for up 
 *   to 2 picoseconds to save time. The cheat must start and end on open paths.
 * - The goal is to calculate the number of cheats that can save at least 100 
 *   picoseconds.
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
 * - Simulate the shortest path without cheats.
 * - Explore all possible cheats (start and end positions) to determine their 
 *   impact on time saved.
 * - Count the number of cheats that save at least 100 picoseconds.
 *
 * **Example Output:**
 * ```
 * Total Cheats Saving ≥100 Picoseconds: [Result]
 * ```
 *
 * **Steps to Solve:**
 * 1. Parse the input to create a grid representation of the racetrack.
 * 2. Implement a shortest path algorithm (e.g., BFS) to determine the baseline 
 *    time from 'S' to 'E'.
 * 3. Simulate all possible cheats by modifying the grid temporarily, tracking 
 *    the time saved by each cheat.
 * 4. Count and output the cheats that save at least 100 picoseconds.
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

constexpr int n_steps = 2;

struct Point {
  int row;
  int col;
  bool operator==(const Point& p) const { return row == p.row && col == p.col; }

  bool operator<(const Point& p) const {
    return row < p.row || (row == p.row && col < p.col);
  }
};

constexpr std::array<Point, 4> moves{Point{0, 1}, Point{1, 0}, Point{0, -1},
                                     Point{-1, 0}};

bool in_map(const Point& p, const std::vector<std::string>& map) {
  return p.row >= 0 && p.row < map.size() && p.col >= 0 &&
         p.col < map[0].size();
}

std::pair<std::set<Point>, std::vector<std::vector<int>>> find_path(
  const std::vector<std::string>& map, const Point& start, const Point& end) {
  std::queue<std::pair<Point, int>> q;
  std::set<Point> visited;
  std::vector<std::vector<int>> cost_on_path(
    map.size(), std::vector<int>(map[0].size(), 0));

  q.push({end, 0});
  while (!q.empty()) {
    const auto [current, score] = q.front();
    q.pop();
    if (visited.find(current) != visited.end()) continue;
    visited.insert(current);
    cost_on_path[current.row][current.col] = score;

    if (start == current) return {visited, cost_on_path};

    for (const auto& move : moves) {
      Point new_point{current.row + move.row, current.col + move.col};
      if (in_map(new_point, map) && map[new_point.row][new_point.col] != '#')
        q.push({new_point, score + 1});
    }
  }

  return {{}, {}};
}

std::map<int, int> find_possible_cheats(
  const std::vector<std::string>& map,
  const std::vector<std::vector<int>>& costs_on_path,
  const std::set<Point>& points_on_path, const Point& start, const Point& end,
  const int steps) {
  std::map<int, int> cheats_count;

  for (const auto& p : points_on_path) {
    const int cost_at_p = costs_on_path[p.row][p.col];
    for (int d_row = -steps; d_row <= steps; d_row++) {
      for (int d_col = -steps + std::abs(d_row);
           d_col <= steps - std::abs(d_row); d_col++) {
        Point ep{p.row + d_row, p.col + d_col};
        if (points_on_path.find(ep) == points_on_path.end()) continue;

        const int cost_at_ep = costs_on_path[ep.row][ep.col];
        const int time_saved =
          cost_at_p - cost_at_ep - std::abs(d_row) - std::abs(d_col);
        if (time_saved > 0) cheats_count[time_saved]++;
      }
    }
  }

  return cheats_count;
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
    for (size_t idx = 0; idx < line.size(); ++idx) {
      if (line[idx] == 'S')
        start = {static_cast<int>(map.size() - 1), static_cast<int>(idx)};
      else if (line[idx] == 'E')
        end = {static_cast<int>(map.size() - 1), static_cast<int>(idx)};
    }
  }

  const auto [points_on_path, costs_on_path] = find_path(map, start, end);
  const auto cheats_map = find_possible_cheats(
    map, costs_on_path, points_on_path, start, end, n_steps);

  uint64_t total = 0;
  for (const auto& [val, n] : cheats_map)
    if (val >= 100) total += n;

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
