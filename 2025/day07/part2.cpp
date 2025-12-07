/**
 * @file part2.cpp
 * @brief Quantum Tachyon Manifold — Many-Worlds Timelines
 *
 * The classical tachyon manifold simulation counted every time a classical
 * beam was split by a splitter '^'. In the quantum variant, only a single
 * tachyon particle is injected into the manifold, but at each splitter the
 * particle does not choose a single child path — it takes both. Using the
 * many-worlds interpretation, each encounter with a splitter branches time
 * into two timelines: in one timeline the particle followed the left child,
 * in the other it followed the right child.
 *
 * Rules (restated for the quantum problem):
 *  - The input is a rectangular grid of characters containing a single 'S'
 *    (start), empty cells '.' and splitters '^'.
 *  - The particle begins just below 'S' and travels downward.
 *  - When the particle (in a given timeline) reaches a splitter cell '^',
 *    that timeline branches into two independent timelines:
 *      * in one timeline the particle continues from the position immediately
 *        to the left of the splitter (one column left, one row below the '^'),
 *      * in the other timeline it continues from the position immediately
 *        to the right of the splitter (one column right, one row below the
 * '^'). The incoming timeline does not continue past the splitter cell itself.
 *  - Timelines evolve independently thereafter; if a timeline reaches another
 *    splitter it branches again, doubling the number of descendant timelines
 *    coming from that branch.
 *  - Timelines terminate when the particle exits the bottom of the grid
 *    (or otherwise cannot proceed).
 *  - Multiple timelines may visit the same grid cell; they are still distinct
 *    timelines and counted separately.
 *
 * Task:
 *   Simulate the quantum propagation (many-worlds branching) and compute the
 *   total number of distinct timelines that exist after the single injected
 *   particle has completed all of its possible journeys through the manifold.
 *
 * Notes:
 *   - The number of timelines can grow exponentially with the number of
 *     encountered splitters; use big-integer arithmetic if necessary.
 *   - It is sufficient to track the count of active timelines at each
 *     propagation position rather than enumerate each timeline individually.
 *
 * Example:
 *   .......S.......
 *   ...............
 *   .......^.......
 *   ...............
 *   ......^.^......
 *   ...............
 *   .....^.^.^.....
 *   ...............
 *   ....^.^...^....
 *   ...............
 *   ...^.^...^.^...
 *   ...............
 *   ..^...^.....^..
 *   ...............
 *   .^.^.^.^.^...^.
 *   ...............
 *   For the example manifold given in the problem statement, a single quantum
 *   tachyon particle yields 40 distinct timelines in total.
 *
 * Output:
 *   Report the total number of timelines (an integer).
 *
 * Puzzle Answer: [Total number of timelines]
 *
 * @author [gabrielzschmitz]
 * @date [07/12/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test")
    debug = true;
}

struct P {
  int x, y;
  bool operator==(const P &o) const { return x == o.x && y == o.y; }
  bool operator<(const P &o) const {
    return (y < o.y) || (y == o.y && x < o.x);
  }
};

int H, W;
std::vector<std::string> grid;

bool inside(const P &p) { return p.y >= 0 && p.y < H && p.x >= 0 && p.x < W; }

char at(const P &p) {
  if (!inside(p))
    return '#';
  return grid[p.y][p.x];
}

P fall(P p) {
  while (inside(p) && at(p) != '^') {
    P down = {p.x, p.y + 1};
    if (!inside(down))
      break;
    p = down;
  }
  return p;
}

std::map<P, int> memo;
int timelines(P point) {
  if (memo.count(point))
    return memo[point];

  P left = fall({point.x - 1, point.y});
  P right = fall({point.x + 1, point.y});

  int left_tl = (at(left) != '^') ? 1 : timelines(left);
  int right_tl = (at(right) != '^') ? 1 : timelines(right);

  return memo[point] = left_tl + right_tl;
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string line;
  while (std::getline(input_file, line)) {
    if (!line.empty() && line.back() == '\r')
      line.pop_back();
    grid.push_back(line);
  }

  if (grid.empty()) {
    std::cout << "ANSWER: 0\n";
    return 0;
  }

  H = grid.size();
  W = grid[0].size();

  P start{-1, -1};
  for (int y = 0; y < H; y++)
    for (int x = 0; x < W; x++)
      if (grid[y][x] == 'S')
        start = {x, y};

  if (start.x == -1) {
    std::cout << "ANSWER: 0\n";
    return 0;
  }

  start = fall(start);

  int64_t total = timelines(start);

  std::cout << "ANSWER: " << total << "\n";

  return 0;
}
