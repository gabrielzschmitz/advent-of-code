/**
 * @file part1.cpp
 * @brief Navigating Corrupted Memory
 *
 * You and The Historians find yourselves trapped inside a computer at the North
 * Pole, with an algorithm rapidly corrupting memory by dropping bytes into a
 * two-dimensional grid representing the memory space.
 *
 * **Problem Description:**
 * - The memory space is a **70x70 grid** (example uses a **6x6 grid**).
 * - Bytes fall at specific **(X, Y)** coordinates, **corrupting** those
 *   locations.
 * - You must navigate from the **top-left corner (0,0)** to the **bottom-right
 *   (70,70)**.
 * - Movement is allowed **up, down, left, or right**, but corrupted locations
 *   **cannot** be entered.
 *
 * **Objective:**
 * 1. Simulate the first **1024** bytes falling and corrupting the memory space.
 * 2. Find the **shortest path** from the **start to the exit**, avoiding
 * corrupted locations.
 * 3. Output the **minimum steps** required to reach safety.
 *
 * **Challenges:**
 * - Efficiently tracking corrupted locations as bytes fall.
 * - Implementing an optimal pathfinding algorithm (e.g., **Breadth-First
 *   Search**).
 * - Ensuring the route is dynamically updated based on new corrupted positions.
 *
 * **Example Output:**
 * ```
 * Minimum Steps to Exit: [Step Count]
 * ```
 *
 * **Puzzle Answer:** [Minimum Steps]
 *
 * @author [gabrielzschmitz]
 * @date [03/02/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <utility>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

bool checkPathExists(const std::unordered_set<std::string>& corrupted,
                     int64_t max_size = 71) {
  std::pair<int64_t, int64_t> start = {0, 0};
  std::pair<int64_t, int64_t> end = {max_size - 1, max_size - 1};
  std::queue<std::pair<int64_t, int64_t>> queue;
  std::unordered_set<std::string> visited;

  queue.push(start);
  visited.insert(std::to_string(start.first) + "," +
                 std::to_string(start.second));

  while (!queue.empty()) {
    auto current = queue.front();
    queue.pop();

    if (current == end) { return true; }

    int64_t dx[] = {0, 1, 0, -1};
    int64_t dy[] = {1, 0, -1, 0};

    for (int64_t i = 0; i < 4; ++i) {
      int64_t nx = current.first + dx[i];
      int64_t ny = current.second + dy[i];

      if (nx >= 0 && nx < max_size && ny >= 0 && ny < max_size) {
        std::string new_pos = std::to_string(nx) + "," + std::to_string(ny);
        if (corrupted.find(new_pos) == corrupted.end() &&
            visited.find(new_pos) == visited.end()) {
          queue.push({nx, ny});
          visited.insert(new_pos);
        }
      }
    }
  }

  return false;
}

int64_t findShortestPath(const std::unordered_set<std::string>& corrupted,
                         int64_t max_size = 71) {
  std::pair<int64_t, int64_t> start = {0, 0};
  std::pair<int64_t, int64_t> end = {max_size - 1, max_size - 1};
  std::queue<std::pair<std::pair<int64_t, int64_t>, int64_t>> queue;
  std::unordered_set<std::string> visited;

  queue.push({start, 0});
  visited.insert(std::to_string(start.first) + "," +
                 std::to_string(start.second));

  while (!queue.empty()) {
    auto current = queue.front();
    queue.pop();

    if (current.first == end) { return current.second; }

    int64_t dx[] = {0, 1, 0, -1};
    int64_t dy[] = {1, 0, -1, 0};

    for (int64_t i = 0; i < 4; ++i) {
      int64_t nx = current.first.first + dx[i];
      int64_t ny = current.first.second + dy[i];

      if (nx >= 0 && nx < max_size && ny >= 0 && ny < max_size) {
        std::string new_pos = std::to_string(nx) + "," + std::to_string(ny);
        if (corrupted.find(new_pos) == corrupted.end() &&
            visited.find(new_pos) == visited.end()) {
          queue.push({{nx, ny}, current.second + 1});
          visited.insert(new_pos);
        }
      }
    }
  }

  return -1;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::pair<int64_t, int64_t>> coordinates;
  std::string line;

  while (std::getline(input_file, line)) {
    std::stringstream ss(line);
    int64_t x, y;
    char comma;
    ss >> x >> comma >> y;
    coordinates.emplace_back(x, y);
  }

  std::unordered_set<std::string> corrupted_1024;
  for (int i = 0; i < 1024 && i < coordinates.size(); ++i)
    corrupted_1024.insert(std::to_string(coordinates[i].first) + "," +
                          std::to_string(coordinates[i].second));

  std::int64_t total = findShortestPath(corrupted_1024);

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
