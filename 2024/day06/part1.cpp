/**
 * @file part1.cpp
 * @brief Predicting the Guard's Patrol Path
 *
 * This program predicts the patrol path of a guard in the North Pole prototype
 * suit manufacturing lab in the year 1518. The guard follows a strict patrol
 * protocol based on her position and surroundings, moving and turning based
 * on obstacles in the lab.
 *
 * The map of the lab is represented as a grid where:
 * - `#` indicates obstacles (e.g., crates, desks, equipment).
 * - `.` indicates open space.
 * - `^`, `v`, `<`, `>` indicate the guard's current position and direction 
 *   (up, down, left, right, respectively).
 *
 * Patrol Protocol:
 * 1. If there is an obstacle directly in front of the guard, she turns right
 *    90 degrees.
 * 2. Otherwise, she takes a step forward in the direction she is facing.
 * 
 * The guard continues following this protocol until she leaves the bounds of
 * the mapped area. The task is to predict her patrol path and count the number
 * of distinct positions visited, including her starting position.
 *
 * Example Input:
 * ....#.....
 * .........#
 * ..........
 * ..#.......
 * .......#..
 * ..........
 * .#..^.....
 * ........#.
 * #.........
 * ......#...
 *
 * Example Output:
 * ....#.....
 * ....XXXXX#
 * ....X...X.
 * ..#.X...X.
 * ..XXXXX#X.
 * ..X.X.X.X.
 * .#XXXXXXX.
 * .XXXXXXX#.
 * #XXXXXXX..
 * ......#X..
 *
 * Distinct positions visited: 41
 *
 * This program simulates the guard's patrol, tracks her path, and calculates
 * the number of unique positions visited.
 *
 * Puzzle Answer: [Distinct Positions Visited]
 *
 * @author [gabrielzschmitz]
 * @date [16/12/2024]
 */
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

using Position = std::pair<int, int>;
using State = std::pair<Position, char>;

// Directions mapped to their corresponding movements
std::unordered_map<char, Position> direction_map = {
  {'^', {-1, 0}},  // Up
  {'v', {1, 0}},   // Down
  {'<', {0, -1}},  // Left
  {'>', {0, 1}}    // Right
};

// Turns: right from current direction
std::unordered_map<char, char> turn_right = {{'^', '>'},
                                             {'>', 'v'},
                                             {'v', '<'},
                                             {'<', '^'}};

bool is_within_bounds(int x, int y, const std::vector<std::string>& grid) {
  return x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size();
}

int simulate_patrol(std::vector<std::string>& grid, int start_x, int start_y,
                    char start_dir) {
  int x = start_x, y = start_y;
  char dir = start_dir;

  std::set<State> visited_states;
  std::set<Position> visited_positions;

  visited_states.insert({{x, y}, dir});
  visited_positions.insert({x, y});

  grid[x][y] = 'X';

  while (true) {
    int next_x = x + direction_map[dir].first;
    int next_y = y + direction_map[dir].second;

    if (debug)
      std::cout << "Current Position: (" << x << ", " << y
                << ") Facing: " << dir << "\n";

    // Check if the next move is valid
    if (is_within_bounds(next_x, next_y, grid) && grid[next_x][next_y] != '#') {
      // Move forward
      x = next_x;
      y = next_y;

      // Check for cycles: stop if we've been here with the same direction
      if (!visited_states.insert({{x, y}, dir}).second) {
        if (debug)
          std::cout << "Cycle detected at (" << x << ", " << y
                    << ") Facing: " << dir << "\n";
        break;
      }

      // Mark the grid if it's a new position
      if (visited_positions.insert({x, y}).second) grid[x][y] = 'X';
    } else if (!is_within_bounds(next_x, next_y, grid)) break;
    else dir = turn_right[dir];
  }

  // Print the final grid
  if (debug)
    for (const auto& row : grid)
      std::cout << row << std::endl;

  return visited_positions.size();
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  int total = 0;

  std::vector<std::string> grid;
  std::string line;
  while (std::getline(input_file, line))
    if (!line.empty()) grid.push_back(line);

  int start_x = -1, start_y = -1;
  char start_dir = '^';
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (direction_map.count(grid[i][j])) {
        start_x = i;
        start_y = j;
        start_dir = grid[i][j];
        grid[i][j] = '.';
        break;
      }
    }
    if (start_x != -1) break;
  }

  if (start_x == -1 || start_y == -1) {
    std::cerr << "Invalid input: No starting position found!" << std::endl;
    return 1;
  }

  total = simulate_patrol(grid, start_x, start_y, start_dir);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
