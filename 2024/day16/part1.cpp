/**
 * @file part1.cpp
 * @brief Reindeer Maze Navigation
 *
 * This program solves the Reindeer Maze challenge where a Reindeer starts at
 * the Start Tile ('S') facing East and must navigate through a maze to reach
 * the End Tile ('E'). The Reindeer can move forward one tile at a time,
 * increasing the score by 1 point, or rotate 90 degrees clockwise or
 * counterclockwise, increasing the score by 1000 points. The goal is to find
 * the lowest possible score to reach the End Tile.
 *
 * The maze is represented as a grid where:
 * - '.' indicates a walkable path.
 * - '#' indicates a wall.
 * - 'S' marks the starting position.
 * - 'E' marks the end position.
 * The Reindeer can take multiple paths, but only one path results in the lowest 
 * score.
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
 * 7036  // The lowest score achieved after following the optimal path.
 *
 * The program analyzes the maze and calculates the minimum score for the
 * Reindeer to reach the end while considering both movement and rotation.
 *
 * Puzzle Answer: [Lowest Score]
 *
 * @author [gabrielzschmitz]
 * @date 16/02/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

struct State {
  int x, y;
  int direction; /* 0: East, 1: South, 2: West, 3: North */
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

  std::int64_t total = 0;

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
  pq.push({startX, startY, 0, 0});

  std::vector<std::vector<std::vector<int>>> dist(
    maze.size(),
    std::vector<std::vector<int>>(
      maze[0].size(), std::vector<int>(4, std::numeric_limits<int>::max())));

  dist[startX][startY][0] = 0;

  const int forward = 1;
  const int turn = 1000;

  while (!pq.empty()) {
    State curr = pq.top();
    pq.pop();

    if (curr.x == endX && curr.y == endY) {
      std::cout << "ANSWER: " << curr.score << std::endl;
      return 0;
    }

    /* Move forward */
    int newX = curr.x + dx[curr.direction];
    int newY = curr.y + dy[curr.direction];
    if (newX >= 0 && newX < maze.size() && newY >= 0 && newY < maze[0].size() &&
        maze[newX][newY] != '#') {
      if (curr.score + forward < dist[newX][newY][curr.direction]) {
        dist[newX][newY][curr.direction] = curr.score + 1;
        pq.push({newX, newY, curr.direction, curr.score + 1});
      }
    }

    /* Rotate clockwise */
    int newDirection = (curr.direction + 1) % 4;
    if (curr.score + turn < dist[curr.x][curr.y][newDirection]) {
      dist[curr.x][curr.y][newDirection] = curr.score + turn;
      pq.push({curr.x, curr.y, newDirection, curr.score + turn});
    }

    /* Rotate counterclockwise */
    newDirection = (curr.direction + 3) % 4;
    if (curr.score + turn < dist[curr.x][curr.y][newDirection]) {
      dist[curr.x][curr.y][newDirection] = curr.score + turn;
      pq.push({curr.x, curr.y, newDirection, curr.score + turn});
    }
  }

  return 0;
}
