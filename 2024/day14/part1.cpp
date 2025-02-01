/**
 * @file part1.cpp
 * @brief Predicting Robot Positions for Safe Navigation
 *
 * In this challenge, robots outside the bathroom at Easter Bunny Headquarters
 * move in predictable straight-line paths. 
 * To safely guide The Historian to the bathroom, we must determine the
 * distribution of robots after **100 seconds**.
 *
 * **Problem Description:**
 * - Each robot has an initial position **p=(x,y)** and a velocity
 *   **v=(dx,dy)**.
 * - Robots move according to their velocities every second.
 * - The space is **101 tiles wide** and **103 tiles tall**.
 * - Robots wrap around when they reach the boundaries, teleporting to the
 *   opposite side.
 * - Robots do not interact with each other and can share the same tile.
 *
 * **Example Input:**
 * ```
 * p=0,4 v=3,-3
 * p=6,3 v=-1,-3
 * p=10,3 v=-1,2
 * ```
 *
 * **Motion Example:**
 * A robot at **p=2,4** with velocity **v=2,-3** moves as follows:
 * ```
 * t=0  -> (2,4)
 * t=1  -> (4,1)
 * t=2  -> (6,5)
 * t=3  -> (8,2)
 * t=4  -> (10,6)
 * ```
 *
 * **Objective:**
 * 1. Simulate robot movement for **100 seconds** within the bounded space.
 * 2. Count the number of robots in each of the **four quadrants**:
 *    - Quadrants are divided by the center of the grid.
 *    - Robots exactly on the centerlines do not belong to any quadrant.
 * 3. Compute the **safety factor** as the product of the robot counts in all
 * four quadrants.
 *
 * **Example Output:**
 * ```
 * Safety Factor: 12
 * ```
 *
 * **Challenges:**
 * - Efficiently simulating motion with modular arithmetic to handle wrapping.
 * - Handling large numbers of robots while maintaining performance.
 * - Ensuring correct quadrant classification after movement.
 *
 * **Puzzle Answer:** [Computed Safety Factor]
 *
 * @author [gabrielzschmitz]
 * @date [31/01/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

struct Robot {
  int x, y, dx, dy;
};

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  int WIDTH, HEIGHT, TIME;
  if (debug) {
    WIDTH = 11;
    HEIGHT = 7;
    TIME = 100;
  } else {
    WIDTH = 101;
    HEIGHT = 103;
    TIME = 100;
  }
  const int CENTER_X = WIDTH / 2, CENTER_Y = HEIGHT / 2;

  std::vector<Robot> robots;
  std::int64_t total;
  std::string line;

  while (std::getline(input_file, line)) {
    int x, y, dx, dy;
    char discard;
    std::stringstream ss(line);
    ss >> discard >> discard >> x >> discard >> y >> discard >> discard >> dx >>
      discard >> dy;
    robots.push_back({x, y, dx, dy});
  }

  int q1 = 0, q2 = 0, q3 = 0, q4 = 0;

  for (const auto& r : robots) {
    int new_x = (r.x + TIME * r.dx) % WIDTH;
    if (new_x < 0) new_x += WIDTH;

    int new_y = (r.y + TIME * r.dy) % HEIGHT;
    if (new_y < 0) new_y += HEIGHT;

    if (new_x == CENTER_X || new_y == CENTER_Y) continue;
    if (new_x > CENTER_X && new_y > CENTER_Y) q1++;
    if (new_x < CENTER_X && new_y > CENTER_Y) q2++;
    if (new_x < CENTER_X && new_y < CENTER_Y) q3++;
    if (new_x > CENTER_X && new_y < CENTER_Y) q4++;
  }

  total = (std::int64_t)q1 * q2 * q3 * q4;

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
