/**
 * @file part2.cpp
 * @brief Reindeer Maze: Best Path Tiles
 *
 * In this part of the challenge, the goal is to determine how many tiles in the
 * maze are part of the best paths that lead to the End Tile ('E'). The Reindeer
 * start at the Start Tile ('S') and follow the optimal paths, which were
 * calculated in Part 1 with the lowest score. This program finds all the tiles
 * that are part of at least one of these best paths, including the Start and
 * End tiles.
 *
 * A tile is considered part of the best path if it is visited by the Reindeer
 * during any optimal traversal through the maze, either as a movement or a
 * turn. The tiles are marked as 'O' to indicate they belong to the best
 * path(s).
 *
 * The maze is represented as a grid where:
 * - '.' indicates a walkable path.
 * - '#' indicates a wall.
 * - 'S' marks the starting position.
 * - 'E' marks the end position.
 * The optimal paths are determined based on the calculations from Part 1.
 *
 * Example Input:
 * ###############
 * #.......#....E#
 * #.#.###.#.###.#
 * #.....#.#...#.#
 * #.###.#####.#.#
 * #.#.#.......#.#
 * #.#.#####.###.#
 * #...........#.#
 * ###.#.#####.#.#
 * #...#.....#.#.#
 * #.#.#.###.#.#.#
 * #.....#...#.#.#
 * #.###.#.#.#.#.#
 * #S..#.....#...#
 * ###############
 *
 * Example Output:
 * 45  // The number of tiles part of at least one of the best paths.
 *
 * The program computes the number of tiles marked 'O' that are part of the best 
 * paths found in Part 1, including both the start and end tiles.
 *
 * Puzzle Answer: [Number of Tiles on Best Paths]
 *
 * @author [gabrielzschmitz]
 * @date 16/02/2025]
 */
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/* Custom hash function for std::tuple<int, int, int, int> */
struct TupleHash {
  size_t operator()(const std::tuple<int, int, int, int>& t) const {
    auto hash1 = std::hash<int>{}(std::get<0>(t));
    auto hash2 = std::hash<int>{}(std::get<1>(t));
    auto hash3 = std::hash<int>{}(std::get<2>(t));
    auto hash4 = std::hash<int>{}(std::get<3>(t));
    return hash1 ^ (hash2 << 1) ^ (hash3 << 2) ^ (hash4 << 3);
  }
};

/* Custom hash function for std::pair<int, int> */
struct PairHash {
  size_t operator()(const std::pair<int, int>& p) const {
    auto hash1 = std::hash<int>{}(p.first);
    auto hash2 = std::hash<int>{}(p.second);
    return hash1 ^ (hash2 << 1);
  }
};

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

struct State {
  int x, y;
  int dx, dy;
  int score;

  bool operator>(const State& other) const { return score > other.score; }
};

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::string> maze;
  int startX = -1, startY = -1, endX = -1, endY = -1;

  std::string line;
  while (std::getline(input_file, line)) {
    maze.push_back(line);
    for (int j = 0; j < line.size(); ++j) {
      if (line[j] == 'S') {
        startX = maze.size() - 1;
        startY = j;
      }
      if (line[j] == 'E') {
        endX = maze.size() - 1;
        endY = j;
      }
    }
  }

  int dx[] = {0, 1, 0, -1};
  int dy[] = {1, 0, -1, 0};

  std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
  pq.push({startX, startY, 0, 1, 0});

  std::unordered_map<std::tuple<int, int, int, int>, int, TupleHash> lowestCost;
  std::unordered_map<
    std::tuple<int, int, int, int>,
    std::unordered_set<std::tuple<int, int, int, int>, TupleHash>, TupleHash>
    backtrack;
  std::unordered_set<std::tuple<int, int, int, int>, TupleHash> endStates;
  int bestCost = std::numeric_limits<int>::max();

  lowestCost[{startX, startY, 0, 1}] = 0;

  const int forward = 1;
  const int turn = 1000;

  while (!pq.empty()) {
    State curr = pq.top();
    pq.pop();

    if (curr.score > lowestCost[{curr.x, curr.y, curr.dx, curr.dy}]) continue;

    if (curr.x == endX && curr.y == endY) {
      if (curr.score > bestCost) break;
      bestCost = curr.score;
      endStates.insert({curr.x, curr.y, curr.dx, curr.dy});
    }

    std::vector<std::tuple<int, int, int, int, int>> directions = {
      {curr.score + forward, curr.x + curr.dx, curr.y + curr.dy, curr.dx,
       curr.dy},
      {curr.score + turn, curr.x, curr.y, curr.dy, -curr.dx},
      {curr.score + turn, curr.x, curr.y, -curr.dy, curr.dx}};

    for (auto& [newCost, newX, newY, newDx, newDy] : directions) {
      if (newX < 0 || newX >= maze.size() || newY < 0 ||
          newY >= maze[0].size() || maze[newX][newY] == '#')
        continue;

      auto newKey = std::make_tuple(newX, newY, newDx, newDy);
      int lowest = lowestCost.count(newKey) ? lowestCost[newKey]
                                            : std::numeric_limits<int>::max();

      if (newCost > lowest) continue;

      if (newCost < lowest) {
        backtrack[newKey] =
          std::unordered_set<std::tuple<int, int, int, int>, TupleHash>();
        lowestCost[newKey] = newCost;
      }

      backtrack[newKey].insert({curr.x, curr.y, curr.dx, curr.dy});
      pq.push({newX, newY, newDx, newDy, newCost});
    }
  }

  std::queue<std::tuple<int, int, int, int>> states;
  std::unordered_set<std::tuple<int, int, int, int>, TupleHash> seen;

  for (auto& state : endStates) {
    states.push(state);
    seen.insert(state);
  }

  while (!states.empty()) {
    auto key = states.front();
    states.pop();

    if (!backtrack.count(key)) continue;

    for (auto& last : backtrack[key]) {
      if (!seen.count(last)) {
        seen.insert(last);
        states.push(last);
      }
    }
  }

  std::unordered_set<std::pair<int, int>, PairHash> uniquePositions;
  for (auto& [x, y, dx, dy] : seen)
    uniquePositions.insert({x, y});

  uniquePositions.insert({startX, startY});
  uniquePositions.insert({endX, endY});

  for (auto& [x, y] : uniquePositions)
    if (maze[x][y] != 'S' && maze[x][y] != 'E') maze[x][y] = 'O';

  if (debug) {
    std::cout << "BEST COST: " << bestCost << std::endl;
    for (const auto& row : maze)
      std::cout << row << std::endl;
  }

  std::cout << "ANSWER: " << uniquePositions.size() << std::endl;

  return 0;
}
