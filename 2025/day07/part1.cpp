/**
 * @file part1.cpp
 * @brief Tachyon Manifold Beam Splitting
 *
 * A tachyon beam enters a rectangular manifold at the cell marked 'S' and
 * always travels downward through empty space ('.'). When a downward-moving
 * beam encounters a splitter ('^'), that beam stops at the splitter and the
 * splitter emits two new downward beams: one starting from the cell immediately
 * to the splitter's left, and one from the cell immediately to the splitter's
 * right. Those new beams then continue downward in the same fashion (they may
 * hit further splitters, producing yet more beams). Beams that run off the
 * bottom edge of the diagram exit the manifold and play no further role.
 *
 * Beams may overlap: multiple beams can occupy the same cell (and passing
 * through the same splitter multiple incoming beams still causes only the
 * usual split behavior for each incoming beam).
 *
 * Your input is a rectangular grid of characters consisting of:
 *   - 'S' : single starting position (exactly one in the input). The first
 *           beam occupies the cell below S and proceeds downward.
 *   - '.' : empty space that beams pass through.
 *   - '^' : splitter that halts any beam that reaches it and emits two new
 *           beams starting at left and right neighbors (one cell above the
 *           splitter's row, i.e. the beams begin immediately *below* the
 *           splitter's left/right positions and continue downward).
 *
 * Task:
 *   Simulate the tachyon propagation and count the total number of times a
 *   beam is split (i.e. the total number of times any beam encounters a
 *   '^' and causes emission of left/right descendant beams). Return that
 *   total split count.
 *
 * Notes and clarifications:
 *   - The initial beam starts just below the 'S' cell, travelling straight
 *     down. It does not split until (and unless) it reaches a '^'.
 *   - When a beam reaches a splitter, that beam stops at the splitter cell
 *     and contributes +1 to the global split counter. Two new beams are
 *     created: they originate at the cells immediately left and right of the
 *     splitter, one row below the splitter (i.e. they continue downward from
 *     that column). Those two new beams then propagate independently.
 *   - Multiple beams may pass through the same cell; each beam is tracked
 *     independently for the purpose of encountering splitters and producing
 *     further splits.
 *   - The manifold is finite; beams that move past the bottom row simply exit.
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
 *   Given the sample manifold in the problem statement, the propagation yields
 *   a total of 21 splits.
 *
 * Your job:
 *   Parse the provided manifold diagram (puzzle input), simulate beam
 *   propagation according to the rules above, and compute the total number
 *   of split events.
 *
 * Puzzle Answer: [Total number of splits]
 *
 * @author [gabrielzschmitz]
 * @date [07/12/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
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

int bfs(P start) {
  std::queue<P> q;
  std::set<P> visited;
  q.push(start);
  visited.insert(start);

  int splitter = 0;

  while (!q.empty()) {
    P p = q.front();
    q.pop();

    if (at(p) == '^') {
      splitter += 1;
      std::vector<P> dirs = {{p.x - 1, p.y}, {p.x + 1, p.y}};
      for (auto d : dirs) {
        if (inside(d) && !visited.count(d)) {
          visited.insert(d);
          q.push(d);
        }
      }
    } else {
      P d = {p.x, p.y + 1};
      if (inside(d) && !visited.count(d)) {
        visited.insert(d);
        q.push(d);
      }
    }
  }
  return splitter;
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

  int64_t total = bfs(start);

  std::cout << "ANSWER: " << total << "\n";

  return 0;
}
