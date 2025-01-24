/**
 * @file part2.cpp
 * @brief Claw Machine Optimization with Corrected Prize Coordinates
 *
 * In this continuation of the claw machine optimization challenge, it was
 * discovered that the position of every prize was incorrectly measured. 
 * Each prize's actual position is **10 trillion (10^13)** units higher on both
 * the X and Y axes than initially specified. 
 * This program adjusts the prize coordinates accordingly and recalculates the
 * optimal strategy to win as many prizes as possible.
 *
 * **Changes to the Problem:**
 * - All prize coordinates are updated:
 *   - X' = X + 10^13
 *   - Y' = Y + 10^13
 * - Button configurations and costs remain unchanged.
 * - The claw machines may require significantly more button presses to win
 *   prizes.
 *
 * **Example Input (Corrected):**
 * ```
 * Button A: X+94, Y+34
 * Button B: X+22, Y+67
 * Prize: X=10000000008400, Y=10000000005400
 * ```
 *
 * **Example Calculations:**
 * - For the first claw machine:
 *   - Updated prize coordinates: (10000000008400, 10000000005400).
 *   - Determine the number of presses for buttons A and B to align the claw:
 *     - Solve: 94a + 22b = 10000000008400
 *     - Solve: 34a + 67b = 10000000005400
 *   - Calculate the total cost as:
 *     - Cost = 3a (A presses) + b (B presses).
 *   - For this example, it is not feasible to align the claw within a
 *   reasonable number of presses.
 *
 * **Objectives:**
 * 1. Update the prize coordinates by adding 10^13 to each X and Y position.
 * 2. Reevaluate the feasibility of aligning the claw for each machine.
 * 3. Maximize the number of prizes won while minimizing the total token
 * expenditure.
 *
 * **Example Output:**
 * ```
 * Maximum prizes: 2
 * Minimum tokens: [Calculated Value]
 * ```
 *
 * **Challenges:**
 * - Handling large numbers efficiently due to the corrected coordinates.
 * - Solving the system of linear equations for alignment with the updated
 *   constraints.
 * - Ensuring the solution remains computationally efficient given the larger
 *   search space.
 *
 * **Puzzle Answer:** [Fewest Tokens for Maximum Prizes with Corrected
 * Coordinates]
 *
 * @author [gabrielzschmitz]
 * @date [23/01/2024]
 */
#include <cstdint>
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

struct Button {
  int64_t x, y, cost;
};

struct Prize {
  int64_t x, y;
};

int64_t calculateMinimumTokens(int64_t ax, int64_t ay, int64_t bx, int64_t by,
                               int64_t prizex, int64_t prizey) {
  int64_t det = ax * by - ay * bx;
  int64_t a = (prizex * by - prizey * bx) / det;
  int64_t b = (ax * prizey - ay * prizex) / det;
  if (ax * a + bx * b == prizex && ay * a + by * b == prizey) return a * 3 + b;
  else return 0;
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
  std::int64_t total = 0;

  a.cost = 3;
  b.cost = 1;
  std::string line;

  while (std::getline(input_file, line)) {
    if (line.find("Button A:") != std::string::npos) {
      sscanf(line.c_str(), "Button A: X+%ld, Y+%ld", &a.x, &a.y);
    } else if (line.find("Button B:") != std::string::npos) {
      sscanf(line.c_str(), "Button B: X+%ld, Y+%ld", &b.x, &b.y);
    } else if (line.find("Prize:") != std::string::npos) {
      Prize prize;
      sscanf(line.c_str(), "Prize: X=%ld, Y=%ld", &prize.x, &prize.y);
      prize.x += 10000000000000LL;
      prize.y += 10000000000000LL;
      prizes.push_back(prize);
    } else if (line.empty() ||
               line.find_first_not_of(" \t\n\r") == std::string::npos) {
      for (const auto& prize : prizes) {
        int64_t cost =
          calculateMinimumTokens(a.x, a.y, b.x, b.y, prize.x, prize.y);
        total += cost;
      }
      prizes.clear();
    }
  }

  for (const auto& prize : prizes) {
    int64_t cost = calculateMinimumTokens(a.x, a.y, b.x, b.y, prize.x, prize.y);
    total += cost;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
