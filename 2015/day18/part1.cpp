/**
 * @file part1.cpp
 * @brief Animated Light Grid Simulation
 *
 * This program simulates an animated grid of lights arranged in a 100x100 grid.
 * Each light can be either "on" (#) or "off" (.), and its state evolves over 
 * time based on the states of its eight neighbors (including diagonals).
 *
 * The task is to calculate how many lights are "on" after simulating 100 steps 
 * of animation. Each light's state changes according to the following rules:
 * - A light that is "on" stays on if it has 2 or 3 neighbors that are also
 *   "on." Otherwise, it turns "off."
 * - A light that is "off" turns on if exactly 3 neighbors are "on." Otherwise, 
 *   it remains "off."
 *
 * The program processes the initial grid configuration provided as input, 
 * and then simulates the changes in state across 100 steps.
 *
 * Example Input:
 * Initial grid state:
 * .#.#.#
 * ...##.
 * #....#
 * ..#...
 * #.#..#
 * ####..
 *
 * Example Output after 4 steps:
 * Number of lights "on": 4
 *
 * The program computes the number of lights that remain "on" after 100 steps.
 *
 * Puzzle Answer: [Number of Lights On After 100 Steps]
 *
 * @author [gabrielzschmitz]
 * @date [07/09/2024]
 */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool debug = true;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

const int GRID_SIZE = debug ? 6 : 100;
const int STEPS = debug ? 5 : 100;

bool isValid(int x, int y) {
  return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

int countOnNeighbors(const std::vector<std::vector<char>>& grid, int x, int y) {
  int count = 0;
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) continue;
      int nx = x + dx, ny = y + dy;
      if (isValid(nx, ny) && grid[nx][ny] == '#') ++count;
    }
  }
  return count;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::vector<char>> grid(GRID_SIZE, std::vector<char>(GRID_SIZE));
  std::string line;
  int row = 0;
  while (std::getline(input_file, line)) {
    for (int col = 0; col < GRID_SIZE; ++col)
      grid[row][col] = line[col];
    ++row;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  for (int step = 0; step < STEPS; ++step) {
    std::vector<std::vector<char>> new_grid = grid;

    for (int i = 0; i < GRID_SIZE; ++i) {
      for (int j = 0; j < GRID_SIZE; ++j) {
        int on_neighbors = countOnNeighbors(grid, i, j);
        if (grid[i][j] == '#') {
          if (on_neighbors != 2 && on_neighbors != 3) new_grid[i][j] = '.';
        } else if (on_neighbors == 3) new_grid[i][j] = '#';
      }
    }

    grid = new_grid;
  }

  int total_on = 0;
  for (int i = 0; i < GRID_SIZE; ++i)
    for (int j = 0; j < GRID_SIZE; ++j)
      if (grid[i][j] == '#') ++total_on;

  int total = total_on;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
