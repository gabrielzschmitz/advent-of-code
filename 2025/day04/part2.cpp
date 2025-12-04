/**
 * @file part2.cpp
 * @brief Printing Department — Iterative Forklift Removal (Part Two)
 *
 * Building on Part One, the forklifts can remove accessible rolls of paper
 * (`@`) from the grid. A roll is accessible if it has fewer than four other
 * rolls in its eight neighboring cells (Moore neighbourhood). After a roll is
 * removed, that may make additional rolls become accessible. The forklifts
 * repeat this process — removing any roll that is currently accessible — until
 * no more rolls can be removed.
 *
 * Task:
 *   - Given the input grid of rolls (`@`) and empty cells (`.`), simulate
 *     repeatedly removing accessible rolls until no more removals are
 *     possible. Return the total number of rolls removed.
 *
 * Rules / details:
 *   - The grid is rectangular; lines are all the same length.
 *   - At each step, consider every cell that currently contains a roll (`@`).
 *     If that cell has strictly fewer than four adjacent rolls among the up to
 *     eight neighbours, it is accessible and can be removed.
 *   - Removals during a single iteration may be applied simultaneously (i.e.,
 *     identify all accessible rolls, remove them, then repeat), or you may
 *     remove them one-by-one in any order — the final result is the same so
 *     long as you remove any roll as soon as it becomes accessible and repeat
 *     until no more removals are possible.
 *   - The process stops when an iteration removes zero rolls.
 *
 * Example (the same sample from Part One):
 *   - Initially there are some rolls accessible; following the repeated
 *     removal process illustrated in the prompt yields a total of 43 rolls
 *     removed.
 *
 * Output:
 *   - The program should output a single integer: the total number of rolls
 *     removed after the process completes.
 *
 * Puzzle Answer: [Total Removed Rolls]
 *
 * @author [gabrielzschmitz]
 * @date [04/12/2025]
 */
#include <bits/stdc++.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
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

  while (true) {
    std::vector<std::pair<int, int>> accessible;
    accessible.reserve(1024);

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

        if (neighbours < 4)
          accessible.push_back({r, c});
      }
    }

    if (accessible.empty())
      break;

    if (debug)
      std::cout << "accessible this round: " << accessible.size() << std::endl;

    for (auto &p : accessible) {
      grid[p.first][p.second] = '.';
    }

    total += accessible.size();
  }

  if (debug)
    printGridWithMarks(grid, accessible);
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
