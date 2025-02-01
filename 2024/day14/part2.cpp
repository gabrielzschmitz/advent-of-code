/**
 * @file part2.cpp
 * @brief Detecting the Christmas Tree Formation
 *
 * After analyzing the movement of robots outside the bathroom, a peculiar
 * pattern emerges. These robots resemble models used at the North Pole, which
 * contain a hard-coded **Easter egg**: at some moment, they should form a
 * **Christmas tree** pattern.
 *
 * **Problem Description:**
 * - Each robot moves according to its velocity **v=(dx,dy)**.
 * - The space is **101 tiles wide** and **103 tiles tall**, with robots
 *   wrapping around edges.
 * - We must determine the **earliest time** at which the robots naturally align
 *   into a recognizable **Christmas tree** shape.
 *
 * **Objective:**
 * 1. Simulate robot motion over time.
 * 2. Identify the first moment when the robots form a **Christmas tree**
 * pattern.
 * 3. Output the **earliest timestamp** where this occurs.
 *
 * **Challenges:**
 * - Efficiently simulating movement while handling periodic wrapping.
 * - Recognizing a meaningful **Christmas tree** structure among the robot
 *   positions.
 * - Ensuring an optimal search strategy to find the earliest occurrence.
 *
 * **Example Output:**
 * ```
 * Earliest Christmas Tree Formation: [Timestamp]
 * ```
 *
 * **Puzzle Answer:** [Earliest Timestamp]
 *
 * @author [gabrielzschmitz]
 * @date [31/01/2025]
 */
#include <cstdint>
#include <deque>
#include <fstream>
#include <iostream>
#include <set>
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

int mod(int value, int mod_val) {
  return (value % mod_val + mod_val) % mod_val;
}

void floodFill(const std::vector<std::vector<int>>& grid, int x, int y,
               std::set<std::pair<int, int>>& visited, int& cluster_size,
               int width, int height) {
  std::deque<std::pair<int, int>> queue;
  queue.push_back({x, y});
  while (!queue.empty()) {
    auto [cx, cy] = queue.front();
    queue.pop_front();
    if (visited.count({cx, cy})) continue;
    visited.insert({cx, cy});
    cluster_size++;
    for (auto [dx, dy] :
         std::vector<std::pair<int, int>>{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
      int nx = cx + dx, ny = cy + dy;
      if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
          grid[ny][nx] == 1 && !visited.count({nx, ny}))
        queue.push_back({nx, ny});
    }
  }
}

int getLargestCluster(const std::vector<std::pair<int, int>>& positions,
                      int width, int height) {
  std::vector<std::vector<int>> grid(height, std::vector<int>(width, 0));
  for (auto [x, y] : positions)
    grid[y][x] = 1;
  std::set<std::pair<int, int>> visited;
  int max_cluster = 0;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (grid[y][x] == 1 && !visited.count({x, y})) {
        int cluster_size = 0;
        floodFill(grid, x, y, visited, cluster_size, width, height);
        max_cluster = std::max(max_cluster, cluster_size);
      }
    }
  }
  return max_cluster;
}

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
  std::int64_t total = 0;
  std::string line;

  while (std::getline(input_file, line)) {
    int x, y, dx, dy;
    char discard;
    std::stringstream ss(line);
    ss >> discard >> discard >> x >> discard >> y >> discard >> discard >> dx >>
      discard >> dy;
    robots.push_back({x, y, dx, dy});
  }

  while (true) {
    std::vector<std::pair<int, int>> positions;
    for (const auto& r : robots) {
      int new_x = mod(r.x + total * r.dx, WIDTH);
      int new_y = mod(r.y + total * r.dy, HEIGHT);
      positions.emplace_back(new_x, new_y);
    }

    if (getLargestCluster(positions, WIDTH, HEIGHT) >
        15) { /* Found by trial and error */
      std::cout << "ANSWER: " << total << std::endl;
      return 0;
    }
    total++;
  }

  return 0;
}
