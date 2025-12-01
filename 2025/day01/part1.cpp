/**
 * @file part1.cpp
 * @brief Secret Entrance — Safe Dial Rotations
 *
 * The safe protecting the entrance uses a circular dial labeled 0–99. The
 * attached document (your puzzle input) is a sequence of rotations, one per
 * line. Each rotation starts with 'L' or 'R' indicating the direction to turn
 * the dial (left = toward smaller numbers, right = toward larger numbers),
 * followed by an integer distance giving how many clicks to move.
 *
 * The dial wraps around: left from 0 goes to 99, and right from 99 goes to 0.
 * The dial begins pointing at 50.
 *
 * Your task is to simulate the entire rotation sequence and count how many
 * times the dial ends up pointing at 0 **after** a rotation. That count is the
 * actual password (the safe itself is a decoy).
 *
 * Example:
 * Starting at 50, applying the sequence
 *   L68 L30 R48 L5 R60 L55 L1 L99 R14 L82
 * produces 0 three times, so the password would be 3.
 *
 * Input:
 * A newline-separated list of rotations, e.g.:
 *   L68
 *   L30
 *   R48
 *   ...
 *
 * Output:
 * An integer — the number of rotations after which the dial is pointing at 0.
 *
 * Puzzle Answer: [Password]
 *
 * @author [gabrielzschmitz]
 * @date [01/12/2025]
 */
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

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  int64_t total = 0;
  std::string line;

  int current_pointing = 50;
  char rotation;
  int distance;
  while (std::getline(input_file, line)) {
    if (line.empty())
      continue;
    rotation = line[0];
    distance = std::stoi(line.substr(1));

    if (rotation == 'L')
      current_pointing = (current_pointing - distance) % 100;
    else if (rotation == 'R')
      current_pointing = (current_pointing + distance) % 100;
    if (current_pointing < 0)
      current_pointing += 100;

    if (current_pointing == 0)
      total += 1;

    if (debug) {
      std::cout << "instruction: " << line
                << " current_pointing: " << current_pointing
                << " total: " << total << std::endl;
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
