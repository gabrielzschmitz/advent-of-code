/**
 * @file part2.cpp
 * @brief Predicting Obstruction Positions to Trap the Guard
 *
 * This program builds upon the prediction of the guard's patrol path and
 * explores the possibility of adding a new obstruction to trap the guard
 * in a loop. The goal is to identify the positions where a new obstruction
 * can be added to make the guard repeatedly patrol the same set of positions
 * without leaving the mapped area. The Historians believe this would allow
 * them to safely search the lab without being caught.
 *
 * The map of the lab is represented as a grid where:
 * - `#` indicates obstacles (e.g., crates, desks, equipment).
 * - `.` indicates open space.
 * - `^`, `v`, `<`, `>` indicate the guard's current position and direction
 *   (up, down, left, right, respectively).
 * - `O` indicates a potential position for a new obstruction that could
 *   trap the guard in a loop.
 * - `|`, `-`, and `+` indicate positions where the guard moves up/down,
 *   left/right, or both, respectively.
 *
 * Task:
 * The Historians need to determine all the possible positions where a new
 * obstruction could be added to create a loop for the guard. The new obstruction
 * must not be placed at the guard's starting position, and it should cause
 * the guard to move in a loop, never leaving the mapped area. The number of
 * such positions must be calculated.
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
 * ....+---+#
 * ....|...|.
 * ..#.|...|.
 * ....|..#|.
 * ....|...|.
 * .#.O^---+.
 * ........#.
 * #.........
 * ......#...
 *
 * In this example, there are 6 different positions where a new obstruction
 * can be placed to trap the guard in a loop.
 *
 * Puzzle Answer: [Number of Possible Positions for Obstruction]
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

    if (is_within_bounds(next_x, next_y, grid) && grid[next_x][next_y] != '#') {
      x = next_x;
      y = next_y;

      if (!visited_states.insert({{x, y}, dir}).second) return 1;

      if (visited_positions.insert({x, y}).second) grid[x][y] = 'X';
    } else if (!is_within_bounds(next_x, next_y, grid)) break;
    else dir = turn_right[dir];
  }

  return 0;
}

int find_valid_obstruction_positions(std::vector<std::string>& grid,
                                     int start_x, int start_y, char start_dir) {
  int valid_positions = 0;

  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (grid[i][j] == '.' && !(i == start_x && j == start_y)) {
        std::vector<std::string> grid_tmp = grid;
        grid_tmp[i][j] = '#';

        int result = simulate_patrol(grid_tmp, start_x, start_y, start_dir);

        if (result == 1) valid_positions++;
      }
    }
  }

  return valid_positions;
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

  total = find_valid_obstruction_positions(grid, start_x, start_y, start_dir);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
