/**
 * @file part1.cpp
 * @brief Keypad Conundrum - Code Unlocking
 *
 * This program helps calculate the shortest sequence of button presses required
 * to unlock a door on Santa's Reindeer-class starship. The robot has to type a
 * series of numeric codes on a numeric keypad. However, the robot cannot
 * directly push buttons and instead controls a robotic arm using a directional
 * keypad. The program calculates the minimum number of button presses required
 * to generate the numeric codes based on a set of rules.
 *
 * **Challenge Description:**
 * The robot controls its arm using a directional keypad with directional arrows
 * and an activation button. The robot is initially pointing at the "A" key, and
 * it uses the directional inputs (up, down, left, right) to move to other keys
 * on the keypad, then presses the "A" button to type that key. The task is to
 * determine the shortest sequence of button presses to generate the five door
 * codes on the numeric keypad.
 * 
 * **Example Input:**
 * - 029A
 * - 980A
 * - 179A
 * - 456A
 * - 379A
 * 
 * **Example Process:**
 * The program must calculate the minimum number of button presses required for
 * each code to be typed, taking into account the structure of the directional
 * keypad and ensuring that the robotic arm does not move into areas where no
 * button is available.
 *
 * **Example Output:**
 * For each code, calculate the shortest button press sequence and the
 * complexity by multiplying the length of the sequence by the numeric part of
 * the code (ignoring leading zeros). The complexity for each code is then
 * summed to give the final result.
 *
 * **Puzzle Answer:** [Sum of Complexities]
 *
 * @author [gabrielzschmitz]
 * @date [23/12/2024]
 */
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

void add_next_goals(const std::deque<std::pair<int, int>>& goals,
                    std::deque<std::pair<int, int>>& next_goals,
                    std::pair<int, int>& curr, bool moving_numeral) {
  for (const auto& goal : goals) {
    const auto& [i, j] = curr;
    const auto& [ni, nj] = goal;
    const int di = ni - i, dj = nj - j;

    bool already_processed = false;

    if (moving_numeral) {
      if (ni == NUM_GAP_I && j == NUM_GAP_J) {
        for (int c = 0; c < dj; c++)
          next_goals.push_back(DIR_R_POS);
        for (int c = 0; c < di; c++)
          next_goals.push_back(DIR_D_POS);
        already_processed = true;
      }
      if (i == NUM_GAP_I && nj == NUM_GAP_J) {
        for (int c = 0; c < -di; c++)
          next_goals.push_back(DIR_U_POS);
        for (int c = 0; c < -dj; c++)
          next_goals.push_back(DIR_L_POS);
        already_processed = true;
      }
    } else {
      if (i == NUM_GAP_I && nj == NUM_GAP_J) {
        for (int c = 0; c < di; c++)
          next_goals.push_back(DIR_D_POS);
        for (int c = 0; c < -dj; c++)
          next_goals.push_back(DIR_L_POS);
        already_processed = true;
      }
      if (ni == NUM_GAP_I && j == NUM_GAP_J) {
        for (int c = 0; c < dj; c++)
          next_goals.push_back(DIR_R_POS);
        for (int c = 0; c < -di; c++)
          next_goals.push_back(DIR_U_POS);
        already_processed = true;
      }
    }

    if (!already_processed) {
      const bool going_right = dj > 0;
      const bool going_up = di < 0;

      if (!going_right) {
        for (int c = 0; c < -dj; c++)
          next_goals.push_back(DIR_L_POS);
      }
      if (!going_up) {
        for (int c = 0; c < di; c++)
          next_goals.push_back(DIR_D_POS);
      }
      if (going_right) {
        for (int c = 0; c < dj; c++)
          next_goals.push_back(DIR_R_POS);
      }
      if (going_up) {
        for (int c = 0; c < -di; c++)
          next_goals.push_back(DIR_U_POS);
      }
    }

    next_goals.push_back(DIR_A_POS);
    curr = goal;
  }
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
    std::deque<std::pair<int, int>> goals, next_goals;
    std::pair<int, int> curr = NUM_A_POS;

    for (char c : line)
      goals.push_back((c == 'A') ? NUM_A_POS : NUM_POSITIONS[c - '0']);

    add_next_goals(goals, next_goals, curr, true);

    for (int c = 0; c < 2; c++) {
      curr = DIR_A_POS;
      goals = next_goals;
      next_goals.clear();
      add_next_goals(goals, next_goals, curr, false);
    }

    total += next_goals.size() * std::stoull(line.substr(0, 3));
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
