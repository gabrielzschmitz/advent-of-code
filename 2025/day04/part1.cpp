/**
 * @file part1.cpp
 * @brief Printing Department — Forklift-Accessible Rolls
 *
 * The printing department stores large rolls of paper on a grid. Each grid
 * cell is either a roll of paper ('@') or empty ('.'). Forklifts may only
 * access a roll of paper if **fewer than four** other rolls occupy the eight
 * adjacent cells (the Moore neighbourhood).
 *
 * Task:
 *   - Given the complete grid (puzzle input), count how many rolls of paper
 *     are accessible to a forklift under the rule above.
 *
 * Rules / details:
 *   - The grid is rectangular; lines are all the same length.
 *   - For each cell containing '@', examine its eight neighbours
 *     (up, down, left, right and the four diagonals). If strictly fewer than
 *     four neighbours are '@', that roll is accessible.
 *   - Cells on the edges have fewer than eight neighbours; only existing
 *     neighbours are considered.
 *
 * Example:
 *   Input (single-line-wrapped example shown):
 *     ..@@.@@@@.
 *     @@@.@.@.@@
 *     @@@@@.@.@@
 *     @.@@@@..@.
 *     @@.@@@@.@@
 *     .@@@@@@@.@
 *     .@.@.@.@@@
 *     @.@@@.@@@@
 *     .@@@@@@@@.
 *     @.@.@@@.@.
 *
 *   Accessible rolls (marked x) in the example produce a total of 13.
 *
 * Output:
 *   - The program should output a single integer: the count of accessible
 *     rolls of paper found in the input grid.
 *
 * Puzzle Answer: [Accessible Rolls Count]
 *
 * @author [gabrielzschmitz]
 * @date [04/12/2025]
 */
#include <bits/stdc++.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test")
    debug = true;
}

void printGridWithMarks(const std::vector<std::string> &grid,
                        const std::vector<std::pair<int, int>> &marks) {
  if (grid.empty())
    return;

  std::vector<std::string> out = grid;

  for (const auto &p : marks) {
    int r = p.first;
    int c = p.second;
    if (r >= 0 && r < (int)out.size() && c >= 0 && c < (int)out[0].size()) {
      out[r][c] = 'x';
    }
  }

  for (const auto &row : out)
    std::cout << row << "\n";
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::string> grid;
  std::vector<std::pair<int, int>> accessible;
  std::string line;
  while (std::getline(input_file, line))
    grid.push_back(line);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  const int H = grid.size();
  const int W = grid.empty() ? 0 : grid[0].size();

  int64_t total = 0;

  // Directions for 8 neighbors
  const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
  const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

  for (int r = 0; r < H; r++) {
    for (int c = 0; c < W; c++) {
      if (grid[r][c] != '@')
        continue;

      int neighbours = 0;

      for (int k = 0; k < 8; k++) {
        int nr = r + dx[k];
        int nc = c + dy[k];
        if (nr < 0 || nr >= H || nc < 0 || nc >= W)
          continue;
        if (grid[nr][nc] == '@')
          neighbours++;
      }

      if (neighbours < 4) {
        total++;
        accessible.push_back({r, c});
        if (debug)
          std::cout << "accessible roll at (" << r << ", " << c
                    << ") with neighbours=" << neighbours << std::endl;
      }
    }
  }

  if (debug)
    printGridWithMarks(grid, accessible);
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
