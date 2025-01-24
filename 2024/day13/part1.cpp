/**
 * @file part1.cpp
 * @brief Claw Machine Optimization
 *
 * This program calculates the minimum number of tokens required to win as many
 * prizes as possible from a set of claw machines. 
 * Each claw machine has two buttons, A and B, that move the claw a specific
 * amount along the X and Y axes:
 * - Button A costs 3 tokens per press and moves the claw by specific amounts
 *   along X and Y.
 * - Button B costs 1 token per press and moves the claw by different amounts
 *   along X and Y.
 *
 * Each machine has a single prize located at specific coordinates. To win the
 * prize, the claw must align exactly with the prize's X and Y coordinates.
 *
 * The task involves:
 * 1. Determining how many times to press buttons A and B for each machine to
 * align the claw with the prize.
 * 2. Calculating the total cost of achieving alignment for each machine.
 * 3. Determining the maximum number of prizes that can be won for the minimum
 * token expenditure.
 *
 * **Example Input:**
 * ```
 * Button A: X+94, Y+34
 * Button B: X+22, Y+67
 * Prize: X=8400, Y=5400
 * ```
 *
 * **Example Calculations:**
 * - To align the claw for the first machine:
 *   - Press A 80 times and B 40 times to satisfy both X and Y axis conditions:
 *     - X-axis: 80 * 94 + 40 * 22 = 8400
 *     - Y-axis: 80 * 34 + 40 * 67 = 5400
 *   - Cost = 80 * 3 (A presses) + 40 * 1 (B presses) = 280 tokens.
 *
 * **Example Output:**
 * ```
 * Maximum prizes: 2
 * Minimum tokens: 480
 * ```
 *
 * **Challenges:**
 * - Solving the system of linear equations for X and Y movement constraints.
 * - Efficiently determining feasible solutions within the constraints (e.g.,
 *   each button pressed at most 100 times).
 * - Calculating the optimal strategy to minimize tokens while maximizing
 *   prizes.
 *
 * **Puzzle Answer:** [Fewest Tokens for Maximum Prizes]
 *
 * @author [gabrielzschmitz]
 * @date [23/01/2024]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

struct Button {
  int32_t x, y, cost;
};

struct Prize {
  int32_t x, y;
};

int32_t calculateMinimumTokens(const Button& a, const Button& b,
                               const Prize& prize) {
  int32_t min_cost = std::numeric_limits<int32_t>::max();

  for (int a_presses = 0; a_presses <= 100; ++a_presses) {
    for (int b_presses = 0; b_presses <= 100; ++b_presses) {
      int32_t x = a_presses * a.x + b_presses * b.x;
      int32_t y = a_presses * a.y + b_presses * b.y;

      if (x == prize.x && y == prize.y) {
        int cost = a_presses * a.cost + b_presses * b.cost;
        if (cost < min_cost) min_cost = cost;
      }
    }
  }

  return min_cost == std::numeric_limits<std::int32_t>::max() ? -1 : min_cost;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  Button a, b;
  std::vector<Prize> prizes;
  std::int32_t total = 0;

  a.cost = 3;
  b.cost = 1;
  std::string line;
  bool parsing_prize = false;

  while (std::getline(input_file, line)) {
    if (line.find("Button A:") != std::string::npos) {
      sscanf(line.c_str(), "Button A: X+%d, Y+%d", &a.x, &a.y);
    } else if (line.find("Button B:") != std::string::npos) {
      sscanf(line.c_str(), "Button B: X+%d, Y+%d", &b.x, &b.y);
    } else if (line.find("Prize:") != std::string::npos) {
      Prize prize;
      sscanf(line.c_str(), "Prize: X=%d, Y=%d", &prize.x, &prize.y);
      prizes.push_back(prize);
    } else if (line.empty() ||
               line.find_first_not_of(" \t\n\r") == std::string::npos) {
      for (const auto& prize : prizes) {
        int32_t cost = calculateMinimumTokens(a, b, prize);
        if (cost != -1) total += cost;
      }
      prizes.clear();
    }
  }

  for (const auto& prize : prizes) {
    int32_t cost = calculateMinimumTokens(a, b, prize);
    if (cost != -1) total += cost;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
