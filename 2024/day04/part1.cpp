/**
 * @file part1.cpp
 * @brief Word Search Puzzle: Finding Instances of "XMAS"
 *
 * This program assists in solving a word search puzzle by finding all
 * occurrences of the word "XMAS" in a grid of letters. The word can appear in
 * various orientations:
 * - Horizontal (left-to-right or right-to-left)
 * - Vertical (top-to-bottom or bottom-to-top)
 * - Diagonal (in all four possible directions)
 * - Overlapping with other words is allowed.
 *
 * The task is to scan the entire grid and count how many times "XMAS" appears, 
 * regardless of orientation or overlapping characters.
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
 * In this word search, "XMAS" occurs a total of 18 times.
 *
 * The program processes the grid by checking each possible starting position
 * and orientation, counting valid occurrences of "XMAS" and outputting the
 * total count.
 *
 * Puzzle Answer: [Total Instances of "XMAS"]
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

const int directions[8][2] = {
  {0, 1},   // Right
  {0, -1},  // Left
  {1, 0},   // Down
  {-1, 0},  // Up
  {1, 1},   // Down-Right
  {1, -1},  // Down-Left
  {-1, 1},  // Up-Right
  {-1, -1}  // Up-Left
};

bool checkWord(const std::vector<std::string>& grid, int x, int y, int dx,
               int dy) {
  const std::string word = "XMAS";
  int rows = grid.size();
  int cols = grid[0].size();

  for (int i = 0; i < word.size(); ++i) {
    int nx = x + i * dx;
    int ny = y + i * dy;

    if (nx < 0 || nx >= rows || ny < 0 || ny >= cols || grid[nx][ny] != word[i])
      return false;
  }
  return true;
}

int countXMAS(const std::vector<std::string>& grid) {
  int rows = grid.size();
  int cols = grid[0].size();
  int total = 0;

  for (int x = 0; x < rows; ++x) {
    for (int y = 0; y < cols; ++y) {
      for (const auto& dir : directions) {
        if (checkWord(grid, x, y, dir[0], dir[1])) {
          ++total;
          if (debug) {
            std::cout << "Found XMAS at (" << x << ", " << y << ") direction ("
                      << dir[0] << ", " << dir[1] << ")" << std::endl;
          }
        }
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

  std::vector<std::string> grid;
  std::string line;
  while (std::getline(input_file, line)) {
    grid.push_back(line);
  }

  if (grid.empty()) {
    std::cerr << "Grid is empty!" << std::endl;
    return 1;
  }

  int total = countXMAS(grid);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
