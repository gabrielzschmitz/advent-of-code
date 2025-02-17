/**
 * @file part1.cpp
 * @brief Warehouse Robot Simulation
 *
 * This program simulates the movement of an out-of-control robot in a warehouse
 * operated by lanternfish. The robot follows a sequence of movement commands
 * while attempting to push boxes within the warehouse. However, movement can be
 * blocked by walls or constrained by other boxes.
 *
 * **Problem Description:**
 * - The warehouse is represented as a grid where:
 *   - `#` represents walls that cannot be moved through.
 *   - `O` represents boxes that can be pushed if there is space.
 *   - `@` represents the robot, which follows movement instructions.
 * - The robot follows a sequence of commands (`^`, `v`, `<`, `>`) attempting to
 *   move in the respective direction.
 * - The movement may fail if the robot or a pushed box would collide with a
 *   wall or another obstacle.
 * - The goal is to determine the final locations of all boxes after executing
 *   all moves.
 *
 * **Objective:**
 * 1. Parse the warehouse grid and movement sequence.
 * 2. Simulate the movement of the robot and boxes based on the given rules.
 * 3. Compute the sum of all final box positions using a GPS coordinate system:
 *    - A box's coordinate is calculated as `100 * row + column`.
 * 4. Output the sum of all GPS coordinates after all moves are executed.
 *
 * **Example Input:**
 * ```
 * ########
 * #..O.O.#
 * ##@.O..#
 * #...O..#
 * #.#.O..#
 * #...O..#
 * #......#
 * ########
 *
 * <^^>>>vv<v>>v<<
 * ```
 *
 * **Example Output:**
 * ```
 * 2028
 * ```
 *
 * **Puzzle Answer:** [Sum of final GPS coordinates]
 *
 * @author [gabrielzschmitz]
 * @date 06/02/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

struct Warehouse {
  std::vector<std::string> grid;
  int robot_x, robot_y;
  int rows, cols;

  Warehouse() : robot_x(0), robot_y(0), rows(0), cols(0) {}

  bool is_within_bounds(int x, int y) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
  }

  bool push_box(int x, int y, int dx, int dy) {
    int new_x = x + dx, new_y = y + dy;

    while (is_within_bounds(new_x, new_y) && grid[new_x][new_y] == 'O') {
      new_x += dx;
      new_y += dy;
    }

    if (!is_within_bounds(new_x, new_y) || grid[new_x][new_y] == '#')
      return false;

    while (new_x != x || new_y != y) {
      grid[new_x][new_y] = 'O';
      new_x -= dx;
      new_y -= dy;
    }
    grid[x][y] = '.';

    return true;
  }

  void move_robot(char direction) {
    int dx = 0, dy = 0;
    if (direction == '^') dx = -1;
    if (direction == 'v') dx = 1;
    if (direction == '<') dy = -1;
    if (direction == '>') dy = 1;

    int new_x = robot_x + dx, new_y = robot_y + dy;

    if (!is_within_bounds(new_x, new_y) || grid[new_x][new_y] == '#') return;

    if (grid[new_x][new_y] == 'O' && !push_box(new_x, new_y, dx, dy)) return;

    grid[new_x][new_y] = '@';
    grid[robot_x][robot_y] = '.';
    robot_x = new_x;
    robot_y = new_y;
  }

  std::int64_t compute_score() {
    std::int64_t total = 0;
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
        if (grid[i][j] == 'O') total += 100 * i + j;
    return total;
  }
};

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  Warehouse warehouse;
  std::string line;

  while (std::getline(input_file, line) && !line.empty())
    warehouse.grid.push_back(line);

  warehouse.rows = warehouse.grid.size();
  warehouse.cols = warehouse.grid[0].size();

  for (int i = 0; i < warehouse.rows; i++) {
    for (int j = 0; j < warehouse.cols; j++) {
      if (warehouse.grid[i][j] == '@') {
        warehouse.robot_x = i;
        warehouse.robot_y = j;
      }
    }
  }

  std::vector<std::string> moves;
  while (std::getline(input_file, line))
    moves.push_back(line);

  for (std::string move : moves)
    for (char mv : move)
      warehouse.move_robot(mv);

  if (debug) {
    for (int i = 0; i < warehouse.grid.size(); i++) {
      for (int j = 0; j < warehouse.grid[i].size(); j++)
        std::cout << warehouse.grid[i][j];
      std::cout << "\n";
    }
  }

  std::cout << "ANSWER: " << warehouse.compute_score() << std::endl;
  return 0;
}
