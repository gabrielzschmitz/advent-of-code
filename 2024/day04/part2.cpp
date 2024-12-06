/**
 * @file part2.cpp
 * @brief Word Search Puzzle: Finding "X-MAS" Patterns
 *
 * This program extends the functionality of the word search puzzle solver 
 * by finding patterns of two "MAS" sequences forming an "X". Each "MAS" 
 * can appear forwards or backwards, and the pattern is structured as follows:
 *
 * M.S
 * .A.
 * M.S
 *
 * The task is to scan the grid and count all instances of the "X-MAS" pattern 
 * regardless of overlapping characters or orientation.
 *
 * Example Word Search:
 * MMMSXXMASM
 * MSAMXMSMSA
 * AMXSXMAAMM
 * MSAMASMSMX
 * XMASAMXAMM
 * XXAMMXXAMA
 * SMSMSASXSS
 * SAXAMASAAA
 * MAMMMXMMMM
 * MXMXAXMASX
 *
 * Example Output:
 * In this word search, the "X-MAS" pattern occurs a total of 9 times.
 *
 * The program processes the grid by checking each possible starting position 
 * and validates whether the "X-MAS" pattern is present. The total count of 
 * valid patterns is then output.
 *
 * Puzzle Answer: [Total Instances of "X-MAS"]
 *
 * @author [gabrielzschmitz]
 * @date [06/12/2024]
 */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

bool checkXMASPattern(const std::vector<std::string>& grid, int x, int y) {
  int rows = grid.size();
  int cols = grid[0].size();

  // Ensure bounds for the "X-MAS" pattern
  if (x < 1 || x >= rows - 1 || y < 1 || y >= cols - 1) return false;

  // Validate the "X-MAS" structure:
  // Top row:     M . S
  // Center:      . A .
  // Bottom row:  M . S
  if (grid[x - 1][y - 1] == 'M' && grid[x - 1][y + 1] == 'S' &&
      grid[x][y] == 'A' && grid[x + 1][y - 1] == 'M' &&
      grid[x + 1][y + 1] == 'S')
    return true;

  // Inverted "X-MAS" structure:
  // Top row:     S . M
  // Center:      . A .
  // Bottom row:  S . M
  if (grid[x - 1][y - 1] == 'S' && grid[x - 1][y + 1] == 'M' &&
      grid[x][y] == 'A' && grid[x + 1][y - 1] == 'S' &&
      grid[x + 1][y + 1] == 'M')
    return true;

  // Both "MAS" inverted:
  // Top row:     S . S
  // Center:      . A .
  // Bottom row:  M . M
  if (grid[x - 1][y - 1] == 'S' && grid[x - 1][y + 1] == 'S' &&
      grid[x][y] == 'A' && grid[x + 1][y - 1] == 'M' &&
      grid[x + 1][y + 1] == 'M')
    return true;

  // New case for both "MAS" equal:
  // Top row:     M . M
  // Center:      . A .
  // Bottom row:  S . S
  if (grid[x - 1][y - 1] == 'M' && grid[x - 1][y + 1] == 'M' &&
      grid[x][y] == 'A' && grid[x + 1][y - 1] == 'S' &&
      grid[x + 1][y + 1] == 'S')
    return true;

  return false;
}

int countXMASPatterns(const std::vector<std::string>& grid) {
  int rows = grid.size();
  int cols = grid[0].size();
  int total = 0;

  for (int x = 0; x < rows; ++x) {
    for (int y = 0; y < cols; ++y) {
      if (checkXMASPattern(grid, x, y)) {
        ++total;
        if (debug)
          std::cout << "Found X-MAS pattern at (" << x << ", " << y << ")"
                    << std::endl;
      }
    }
  }
  return total;
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
    grid.push_back(line);

  if (grid.empty()) {
    std::cerr << "Grid is empty!" << std::endl;
    return 1;
  }

  total = countXMASPatterns(grid);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
