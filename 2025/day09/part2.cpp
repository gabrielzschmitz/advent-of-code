/**
 * @file part2.cpp
 * @brief Largest Valid Rectangle Using Red & Green Tiles
 *
 * After identifying the largest rectangle using only red corner tiles, the
 * Elves remember an important restriction: **they are only allowed to replace
 * tiles that are red or green**. This dramatically reduces the number of valid
 * rectangles.
 *
 * In the input, each red tile is part of a loop: it is connected to the
 * previous and next red tile in the list by a straight line of green tiles.
 * The list wraps, so the last red tile connects back to the first. Adjacent
 * red tiles are always aligned either horizontally or vertically.
 *
 * All tiles exactly along the connecting paths between red tiles are green.
 * Additionally, **all tiles inside the loop formed by these paths are also
 * green**. Every tile outside this closed loop is neither red nor green and
 * therefore cannot be part of a valid rectangle.
 *
 * The constraints for a valid rectangle now become:
 *   - Its two opposite corners must be red tiles.
 *   - **Every tile within the rectangle area must be red or green.**
 *   - No tile within the rectangle may be outside the red/green region.
 *
 * The program must therefore:
 *   1. Determine which tiles are green based on the loop defined by the red
 * tiles.
 *   2. Identify which rectangles formed by pairs of red tiles lie fully within
 *      the red/green region.
 *   3. Compute the area for each valid rectangle.
 *   4. Output the maximum area possible.
 *
 * Example:
 *   Using the same red points as in Part One, the largest rectangle that uses
 *   only red and green tiles has an area of **24**, using (9,5) and (2,3) as
 *   opposite corners.
 *
 * Puzzle Answer: [Largest Valid Red-Green Rectangle Area]
 *
 * @author [gabrielzschmitz]
 * @date [09/12/2025]
 */
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test")
    debug = true;
}

static inline std::pair<int64_t, int64_t> order_pair(int64_t a, int64_t b) {
  return (a < b) ? std::pair<int64_t, int64_t>{a, b}
                 : std::pair<int64_t, int64_t>{b, a};
}

struct RectEntry {
  int64_t area_neg;
  int rid;
};

struct RectEntryCmp {
  bool operator()(RectEntry const &a, RectEntry const &b) const {
    return a.area_neg > b.area_neg;
  }
};

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::pair<int64_t, int64_t>> tiles;
  std::string line;

  while (getline(input_file, line)) {
    if (line.empty())
      continue;
    std::stringstream ss(line);
    int64_t x, y;
    char comma;
    ss >> x >> comma >> y;
    tiles.emplace_back(x, y);
    if (debug)
      std::cerr << "[DEBUG] read: (" << x << "," << y << ")\n";
  }

  if (tiles.empty()) {
    std::cout << "ANSWER: 0\n";
    return 0;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  const int n = (int)tiles.size();

  std::unordered_map<int64_t, std::vector<std::pair<int64_t, int64_t>>>
      v_segments;
  std::unordered_map<int64_t, std::vector<std::pair<int64_t, int64_t>>>
      h_segments;
  v_segments.reserve(n);
  h_segments.reserve(n);

  for (int i = 0; i < n; ++i) {
    auto [x1, y1] = tiles[i];
    auto [x2, y2] = tiles[(i + 1) % n];

    if (x1 == x2)
      v_segments[x1].push_back(order_pair(y1, y2));
    else
      h_segments[y1].push_back(order_pair(x1, x2));
  }
  std::priority_queue<RectEntry, std::vector<RectEntry>, RectEntryCmp> pq;
  std::vector<std::pair<int, int>> rid_to_idx;
  rid_to_idx.reserve((size_t)n * (size_t)(n - 1) / 2);

  int rid = 0;
  for (int i = 0; i < n - 1; ++i) {
    for (int j = i + 1; j < n; ++j) {
      auto [x1, y1] = tiles[i];
      auto [x2, y2] = tiles[j];

      int64_t dx = std::abs(x1 - x2);
      int64_t dy = std::abs(y1 - y2);
      int64_t area = (dx + 1) * (dy + 1);

      pq.push(RectEntry{-area, rid});
      rid_to_idx.emplace_back(i, j);
      ++rid;
    }
  }

  while (!pq.empty()) {
    RectEntry top = pq.top();
    pq.pop();

    int id = top.rid;
    auto [i, j] = rid_to_idx[id];
    auto [x1, y1] = tiles[i];
    auto [x2, y2] = tiles[j];

    int64_t x_min = std::min(x1, x2);
    int64_t x_max = std::max(x1, x2);
    int64_t y_min = std::min(y1, y2);
    int64_t y_max = std::max(y1, y2);

    bool is_valid = true;

    for (auto &kv : v_segments) {
      int64_t x = kv.first;
      if (!(x_min < x && x < x_max))
        continue;

      for (auto &seg : kv.second) {
        int64_t ys = seg.first;
        int64_t ye = seg.second;
        if (ye > y_min && ys < y_max) {
          is_valid = false;
          break;
        }
      }
      if (!is_valid)
        break;
    }

    if (!is_valid)
      continue;

    for (auto &kv : h_segments) {
      int64_t y = kv.first;
      if (!(y_min < y && y < y_max))
        continue;

      for (auto &seg : kv.second) {
        int64_t xs = seg.first;
        int64_t xe = seg.second;
        if (xe > x_min && xs < x_max) {
          is_valid = false;
          break;
        }
      }
      if (!is_valid)
        break;
    }

    if (!is_valid)
      continue;

    int64_t total = -top.area_neg;
    std::cout << "ANSWER: " << total << std::endl;
    return 0;
  }

  std::cout << "ANSWER: 0" << std::endl;
  return 0;
}
