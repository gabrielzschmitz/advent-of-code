/**
 * @file part2.cpp
 * @brief Keypad Conundrum - Multi-Robot Chain Code Unlocking
 *
 * This program helps calculate the shortest sequence of button presses required
 * to unlock a door on Santa's Reindeer-class starship. This time, multiple
 * robots are involved in typing a series of numeric codes on a numeric keypad.
 * Each robot controls a directional keypad, and one robot’s actions lead to the
 * next robot typing on another directional keypad until the last robot types
 * the code on the numeric keypad. The challenge now involves a chain of 25
 * robots, where each robot controls another through a chain of remote control
 * keypads.
 *
 * **Challenge Description:**
 * The task is to calculate the minimum number of button presses required to
 * generate the numeric codes by navigating through 25 directional keypads. The
 * chain of keypads forms a sequence starting with the user's directional
 * keypad, leading through the robots' directional keypads, and finally ending
 * with the numeric keypad on the door. The goal is to determine the fewest
 * number of button presses needed for the robot at the last stage to type each
 * of the door codes.
 *
 * **Example Input:**
 * - 029A
 * - 980A
 * - 179A
 * - 456A
 * - 379A
 *
 * **Example Process:**
 * In this part, the complexity is increased as there are multiple keypads in
 * sequence. The user must now navigate through 25 directional keypads before
 * the robot finally types on the numeric keypad. Similar to Part 1, we
 * calculate the shortest sequences of button presses for each code.
 *
 * **Example Output:**
 * For each of the five door codes, we calculate the minimum button presses for
 * the chain of robots. Each sequence is multiplied by the numeric part of the
 * code to calculate the complexity. The complexities for all the codes are then
 * summed to give the final result.
 *
 * **Puzzle Answer:** [Sum of Complexities]
 *
 * @author [gabrielzschmitz]
 * @date [23/12/2024]
 */
#include <bits/stdc++.h>
#include <cstdint>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

const std::pair<int, int> NUM_POSITIONS[10] = {{3, 1}, {2, 0}, {2, 1}, {2, 2},
                                               {1, 0}, {1, 1}, {1, 2}, {0, 0},
                                               {0, 1}, {0, 2}};
const std::pair<int, int> NUM_A_POS = {3, 2};
const int NUM_GAP_I = 3;
const int NUM_GAP_J = 0;

const std::pair<int, int> DIR_U_POS = {0, 1};
const std::pair<int, int> DIR_L_POS = {1, 0};
const std::pair<int, int> DIR_D_POS = {1, 1};
const std::pair<int, int> DIR_R_POS = {1, 2};
const std::pair<int, int> DIR_A_POS = {0, 2};
const int DIR_GAP_I = 0;
const int DIR_GAP_J = 0;

std::deque<std::pair<int, int>> add_next_goals_numeral(
  std::deque<std::pair<int, int>> goals, std::pair<int, int> curr) {
  std::deque<std::pair<int, int>> next_goals;
  for (const auto& goal : goals) {
    const auto& [i, j] = curr;
    const auto& [ni, nj] = goal;
    const int di = ni - i, dj = nj - j;

    if (ni == NUM_GAP_I && j == NUM_GAP_J) {
      for (int c = 0; c < dj; c++)
        next_goals.push_back(DIR_R_POS);
      for (int c = 0; c < di; c++)
        next_goals.push_back(DIR_D_POS);
    } else if (i == NUM_GAP_I && nj == NUM_GAP_J) {
      for (int c = 0; c < -di; c++)
        next_goals.push_back(DIR_U_POS);
      for (int c = 0; c < -dj; c++)
        next_goals.push_back(DIR_L_POS);
    } else {
      for (int c = 0; c < -dj; c++)
        next_goals.push_back(DIR_L_POS);
      for (int c = 0; c < di; c++)
        next_goals.push_back(DIR_D_POS);

      for (int c = 0; c < -di; c++)
        next_goals.push_back(DIR_U_POS);
      for (int c = 0; c < dj; c++)
        next_goals.push_back(DIR_R_POS);
    }

    next_goals.push_back(DIR_A_POS);
    curr = goal;
  }

  return next_goals;
}

std::map<std::tuple<int, int, int, int, int>, uint64_t> memo;
uint64_t count_minimum_steps(std::pair<int, int> curr, std::pair<int, int> goal,
                             int keypads) {

  const auto& [i, j] = curr;
  const auto& [ni, nj] = goal;

  const int di = ni - i, dj = nj - j;
  if (keypads == 1) return abs(di) + abs(dj) + 1;

  const std::tuple<int, int, int, int, int>& key = {i, j, ni, nj, keypads};
  if (memo.find(key) != memo.end()) return memo[key];

  std::deque<std::pair<int, int>> next_goals;
  if (i == DIR_GAP_I && nj == DIR_GAP_J) {
    for (int c = 0; c < di; c++)
      next_goals.push_back(DIR_D_POS);
    for (int c = 0; c < -dj; c++)
      next_goals.push_back(DIR_L_POS);
  } else if (ni == DIR_GAP_I && j == DIR_GAP_J) {
    for (int c = 0; c < dj; c++)
      next_goals.push_back(DIR_R_POS);
    for (int c = 0; c < -di; c++)
      next_goals.push_back(DIR_U_POS);
  } else {
    for (int c = 0; c < -dj; c++)
      next_goals.push_back(DIR_L_POS);
    for (int c = 0; c < di; c++)
      next_goals.push_back(DIR_D_POS);

    for (int c = 0; c < -di; c++)
      next_goals.push_back(DIR_U_POS);
    for (int c = 0; c < dj; c++)
      next_goals.push_back(DIR_R_POS);
  }

  next_goals.push_back(DIR_A_POS);

  uint64_t steps = 0;
  std::pair<int, int> prev = DIR_A_POS;
  for (const auto& next_goal : next_goals) {
    steps += count_minimum_steps(prev, next_goal, keypads - 1);
    prev = next_goal;
  }

  memo[key] = steps;
  return steps;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  uint64_t total = 0;
  std::string line;

  while (std::getline(input_file, line)) {
    std::deque<std::pair<int, int>> goals;

    for (char c : line)
      goals.push_back((c == 'A') ? NUM_A_POS : NUM_POSITIONS[c - '0']);

    std::deque<std::pair<int, int>> next_goals =
      add_next_goals_numeral(goals, NUM_A_POS);

    uint64_t steps = 0;
    std::pair<int, int> prev = DIR_A_POS;
    for (const auto& goal : next_goals) {
      steps += count_minimum_steps(prev, goal, 25);
      prev = goal;
    }

    total += steps * stoi(line.substr(0, 3));
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
