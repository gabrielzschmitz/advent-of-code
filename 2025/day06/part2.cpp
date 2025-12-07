/**
 * @file part2.cpp
 * @brief Cephalopod Right-To-Left Columnar Math Evaluation
 *
 * After solving the cephalopod worksheet, the big cephalopods return and
 * explain a crucial detail: cephalopod math is read *right-to-left*, not
 * top-to-bottom like ordinary vertical arithmetic.
 *
 * The worksheet consists of many tightly packed math problems arranged
 * horizontally. Each problem occupies one or more columns, and problems
 * are separated by a full column of spaces.
 *
 * In cephalopod notation:
 *   • Each number occupies a single column.
 *   • The *top* of the column is the most significant digit.
 *   • The *bottom* of the column contains the operation symbol ('+' or '*'),
 *     which applies to all numbers in that problem.
 *   • Problems must be read from **rightmost to leftmost**, one column at a
 * time.
 *
 * Example Worksheet:
 *     123 328  51 64
 *      45 64  387 23
 *       6 98  215 314
 *     *   +   *   +
 *
 * When interpreted right-to-left, each column forms a complete number:
 *
 *   • Rightmost problem:
 *       Column digits → 4, 3, 1 → number 431
 *       Column digits → 6, 2, 3 → number 623
 *       Column digits → 1 digit → number 4
 *       Operator '+' → 4 + 431 + 623 = 1058
 *
 *   • Second-from-right:
 *       175 * 581 * 32 = 3253600
 *
 *   • Third-from-right:
 *       8 + 248 + 369 = 625
 *
 *   • Leftmost:
 *       356 * 24 * 1 = 8544
 *
 * The final answer is the sum of all evaluated problems:
 *     1058 + 3253600 + 625 + 8544 = 3263827
 *
 * Your task:
 *   Re-parse the worksheet using cephalopod right-to-left rules, reconstruct
 *   all numbers column-by-column, evaluate each problem using its operator,
 *   and compute the grand total across all problems.
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
    while (col < W) {
      bool blank = true;
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
      bool blank = true;
      for (int r = 0; r < H; r++)
        if (lines[r][col] != ' ')
          blank = false;
      if (blank)
        break;
      col++;
    }

    int end = col;

    std::vector<long long> nums;
    char op = 0;

    for (int c = end - 1; c >= start; c--) {
      std::string number_digits;
      for (int r = 0; r < H; r++) {
        char ch = lines[r][c];
        if (std::isdigit(ch))
          number_digits.push_back(ch);
      }

      char bottom = lines[H - 1][c];
      if (bottom == '+' || bottom == '*')
        op = bottom;

      if (!number_digits.empty()) {
        long long val = std::stoll(number_digits);
        nums.push_back(val);
      }
    }

    if (op == 0) {
      std::cerr << "ERROR: block has no operator!\n";
      return 1;
    }

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
