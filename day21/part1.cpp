/**
 * @file part1.cpp
 * @brief Step Counter - Counting Garden Plots Reached in Steps
 *
 * Given a map with a starting position (S), garden plots (.), and rocks (#),
 * determine the number of garden plots the Elf can reach in exactly 64 steps.
 * The Elf can move north, south, east, or west, but only onto tiles that are
 * garden plots. Calculate the number of reachable garden plots after exactly 64
 * steps.
 *
 * Example Input:
 * ...........
 * .....###.#.
 * .###.##..#.
 * ..#.#...#..
 * ....#.#....
 * .##..S####.
 * .##..#...#.
 * .......##..
 * .##.#.####.
 * .##..##.##.
 * ...........
 *
 * Example Output:
 * Number of Garden Plots Reached in 64 Steps: 16
 *
 * Calculate the number of garden plots the Elf can reach in exactly 64 steps
 * starting from the specified map.
 *
 * @author [gabrielzschmitz]
 * @date [21/12/2023]
 */
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <tuple>
using namespace std;
namespace std {
template <>
struct hash<tuple<int, int>> {
  size_t operator()(const tuple<int, int>& p) const {
    auto h1 = hash<int>{}(get<0>(p));
    auto h2 = hash<int>{}(get<1>(p));
    return h1 ^ h2;
  }
};
}  // namespace std

bool debug = false;

int fillPlots(vector<string> gardenMap, int startRow, int startColumn,
              int amountSteps) {
  unordered_set<tuple<int, int>> stepsCounter;
  unordered_set<tuple<int, int>> seen = {{startRow, startColumn}};
  deque<tuple<int, int, int>> queue = {{startRow, startColumn, amountSteps}};

  while (!queue.empty()) {
    auto front = queue.front();
    int row = get<0>(front);
    int column = get<1>(front);
    int steps = get<2>(front);
    queue.pop_front();

    if (steps % 2 == 0) {
      stepsCounter.insert({row, column});
    }
    if (steps == 0) {
      continue;
    }

    for (const auto& directionPair :
         vector<pair<int, int>>{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}) {
      int directionRow = directionPair.first;
      int directionColumn = directionPair.second;
      int nRow = row + directionRow;
      int nColumn = column + directionColumn;

      if (nRow < 0 || nRow >= gardenMap.size() || nColumn < 0 ||
          nColumn >= gardenMap[0].size() || gardenMap[nRow][nColumn] == '#' ||
          seen.count({nRow, nColumn})) {
        continue;
      }

      seen.insert({nRow, nColumn});
      queue.push_back({nRow, nColumn, steps - 1});
    }
  }

  if (debug) {
    for (tuple<int, int> i : stepsCounter)
      cout << "(" << get<0>(i) << ", " << get<1>(i) << ") ";
    cout << "\n\n";
  }

  return stepsCounter.size();
}

pair<int, int> findStartingCoordinates(vector<string> gardenMap) {
  pair<int, int> start = {-1, -1};

  int row, column = -1;
  for (string i : gardenMap) {
    column++;
    row = -1;
    for (char j : i) {
      row++;
      if (j == 'S') start = {row, column};
    }
  }

  return start;
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";
  if (debug) user_input = "test";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  int total = 0;

  string line;
  vector<string> gardenMap;
  while (getline(input_file, line)) gardenMap.push_back(line);

  pair<int, int> startingCordinates = findStartingCoordinates(gardenMap);
  if (debug)
    cout << "Start: " << startingCordinates.first << ", "
         << startingCordinates.second << "\n\n";

  total = fillPlots(gardenMap, startingCordinates.first,
                    startingCordinates.second, 64);
  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
