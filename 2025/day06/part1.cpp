/**
 * @file part1.cpp
 * @brief Cephalopod Vertical Math Parsing & Evaluation
 *
 * While trapped inside a trash compactor, you encounter a family of
 * cephalopods. To pass the time, they ask for help solving their math homework.
 * Their math worksheet (the puzzle input) is presented in a long horizontal
 * strip containing multiple problems stacked vertically.
 *
 * Each problem is formed by:
 *  - A vertical list of numbers (top to bottom).
 *  - A mathematical operator ('+' or '*') at the bottom.
 * Problems are separated by a full column of spaces.
 *
 * The left or right alignment of numbers inside a problem is irrelevant; each
 * column belongs to a single vertical problem until a completely blank column
 * is encountered.
 *
 * Example Worksheet:
 *     123 328  51 64
 *      45 64  387 23
 *       6 98  215 314
 *     *   +   *   +
 *
 * This worksheet defines four problems:
 *   • 123 * 45 * 6       = 33210
 *   • 328 + 64 + 98      = 490
 *   • 51 * 387 * 215     = 4243455
 *   • 64 + 23 + 314      = 401
 *
 * After individually evaluating each vertical problem, the students compute a
 * grand total by summing all problem answers:
 *     33210 + 490 + 4243455 + 401 = 4277556
 *
 * The task: Parse the full horizontal worksheet, reconstruct each problem,
 * compute its value using the indicated operation, and finally sum all problem
 * results.
 *
 * The program outputs:
 *   • The grand total of all evaluated problems.
 *
 * Puzzle Answer: [Grand Total]
 *
 * @author [gabrielzschmitz]
 * @date [06/12/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test")
    debug = true;
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::string> lines;
  std::string s;

  while (std::getline(input_file, s)) {
    if (!s.empty() && s.back() == '\r')
      s.pop_back();
    lines.push_back(s);
  }

  if (lines.empty()) {
    std::cout << "ANSWER: 0\n";
    return 0;
  }

  int H = lines.size();
  int W = 0;
  for (auto &L : lines)
    W = std::max(W, (int)L.size());

  for (auto &L : lines)
    if ((int)L.size() < W)
      L += std::string(W - L.size(), ' ');

  int64_t total = 0;

  int col = 0;
  while (col < W) {

    bool blank = true;
    while (col < W) {
      blank = true;
      for (int r = 0; r < H; r++)
        if (lines[r][col] != ' ')
          blank = false;

      if (!blank)
        break;
      col++;
    }
    if (col >= W)
      break;

    int start = col;

    while (col < W) {
      bool blankcol = true;
      for (int r = 0; r < H; r++)
        if (lines[r][col] != ' ')
          blankcol = false;
      if (blankcol)
        break;
      col++;
    }

    int end = col;

    std::vector<long long> nums;
    char op = 0;

    for (int r = 0; r < H; r++) {
      std::string cell = lines[r].substr(start, end - start);

      int a = 0;
      while (a < (int)cell.size() && cell[a] == ' ')
        a++;
      int b = cell.size() - 1;
      while (b >= 0 && cell[b] == ' ')
        b--;

      if (a > b)
        continue;
      std::string t = cell.substr(a, b - a + 1);

      if (t == "+" || t == "*")
        op = t[0];
      else
        nums.push_back(std::stoll(t));
    }

    // Evaluate
    long long value = (op == '+' ? 0LL : 1LL);
    for (long long x : nums) {
      if (op == '+')
        value += x;
      else
        value *= x;
    }

    total += value;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
