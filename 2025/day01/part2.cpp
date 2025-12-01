/**
 * @file part2.cpp
 * @brief Secret Entrance — Count every zero click (method 0x434C49434B)
 *
 * A newer security protocol changes how the password is computed. The safe's
 * dial is the same circular dial labeled 0–99 and begins pointing at 50.
 * The input is a newline-separated list of rotations, each beginning with 'L'
 * (left / toward smaller numbers) or 'R' (right / toward larger numbers)
 * followed by an integer distance (number of clicks).
 *
 * Under method 0x434C49434B, the password is the total number of times the
 * dial points at 0 **on any click**, including intermediate clicks while a
 * rotation is being performed, and including the final position after a
 * rotation. In other words, every individual click that lands the dial on 0
 * should be counted. Rotations wrap around: after 99 comes 0, and before 0
 * comes 99. Large rotation distances are allowed (e.g. R1000).
 *
 * Example:
 * Starting at 50, applying the sequence
 *   L68 L30 R48 L5 R60 L55 L1 L99 R14 L82
 *
 * - L68 moves left from 50 to 82; during that rotation it passes 0 once.
 * - R48 ends at 0 (counts).
 * - R60 passes 0 once during the rotation.
 * - L55 ends at 0 (counts).
 * - L99 ends at 0 (counts).
 * - L82 passes 0 once during the rotation.
 *
 * In this example, there are 3 occurrences at rotation endpoints plus 3 more
 * occurrences during rotations, for a total password of 6.
 *
 * Input:
 * A newline-separated list of rotations, e.g.:
 *   L68
 *   L30
 *   R48
 *   ...
 *
 * Output:
 * An integer — the total count of clicks that land the dial on 0 while
 * performing the full sequence (method 0x434C49434B).
 *
 * Implementation notes:
 * - Simulate each rotation as a sequence of individual clicks (or compute the
 *   number of times the dial crosses 0 analytically using modular arithmetic:
 *   for a rotation of distance d in direction dir starting at s, the number
 *   of times 0 is hit equals the number of integers k in 1..d for which
 *   (s + step*k) mod 100 == 0, where step = +1 for R and -1 for L).
 * - Include the final position after each rotation (it will be counted by the
 *   same method since the final click is part of the rotation's clicks).
 *
 * Puzzle Answer: [Password using method 0x434C49434B]
 *
 * @author [gabrielzschmitz]
 * @date [01/12/2025]
 */
#include <cmath>
#include <cstdint>
#include <cstdlib>
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

struct DivMod {
  long long div, mod;
};

DivMod divMod(long long a, long long b) {
  DivMod r;
  r.div = std::floor((long double)a / (long double)b);
  r.mod = a - r.div * b;
  return r;
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

    DivMod div_mod;
    if (rotation == 'L') {
      distance = -distance;
      div_mod = divMod(distance, -100);
      total += div_mod.div;
      if (current_pointing != 0 && current_pointing + div_mod.mod <= 0)
        total += 1;
    } else if (rotation == 'R') {
      div_mod = divMod(distance, 100);
      total += div_mod.div;
      if (current_pointing + div_mod.mod >= 100)
        total += 1;
    }

    current_pointing = (current_pointing + distance) % 100;
    if (current_pointing < 0)
      current_pointing += 100;

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
