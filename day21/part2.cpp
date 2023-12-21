/**
 * @file part2.cpp
 * @brief Step Counter - Counting Garden Plots Reached in Steps (Infinite Map)
 *
 * Given an infinite map with a starting position (S), garden plots (.), and
 * rocks (#) that repeats infinitely in every direction, determine the number of
 * garden plots the Elf can reach in exactly 26501365 steps. The Elf can move
 * north, south, east, or west, but only onto tiles that are garden plots.
 * Calculate the number of reachable garden plots after exactly 26501365 steps.
 *
 * Example Input:
 * Infinite map with garden plots (.) and rocks (#)
 * Starting position marked as (S)
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
 * Number of Garden Plots Reached in 26501365 Steps: [Result]
 *
 * Calculate the number of garden plots the Elf can reach in exactly 26501365
 * steps starting from the specified infinite map.
 *
 * @author [gabrielzschmitz]
 * @date [21/12/2023]
 */
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <cassert>
#include <vector>
#include <deque>
#include <tuple>
#include <cmath>
using namespace std;
namespace std {
template <>
struct hash<tuple<uint64_t, uint64_t>> {
  size_t operator()(const tuple<uint64_t, uint64_t>& p) const {
    auto h1 = hash<uint64_t>{}(get<0>(p));
    auto h2 = hash<uint64_t>{}(get<1>(p));
    return h1 ^ h2;
  }
};
}  // namespace std

bool debug = false;

uint64_t fillPlots(vector<string> gardenMap, uint64_t startRow,
                   uint64_t startColumn, uint64_t amountSteps) {
  unordered_set<tuple<uint64_t, uint64_t>> stepsCounter;
  unordered_set<tuple<uint64_t, uint64_t>> seen = {{startRow, startColumn}};
  deque<tuple<uint64_t, uint64_t, uint64_t>> queue = {
      {startRow, startColumn, amountSteps}};

  while (!queue.empty()) {
    auto front = queue.front();
    uint64_t row = get<0>(front);
    uint64_t column = get<1>(front);
    uint64_t steps = get<2>(front);
    queue.pop_front();

    if (steps % 2 == 0) {
      stepsCounter.insert({row, column});
    }
    if (steps == 0) {
      continue;
    }

    for (const auto& directionPair :
         vector<pair<uint64_t, uint64_t>>{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}) {
      uint64_t directionRow = directionPair.first;
      uint64_t directionColumn = directionPair.second;
      uint64_t nRow = row + directionRow;
      uint64_t nColumn = column + directionColumn;

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
    for (tuple<uint64_t, uint64_t> i : stepsCounter)
      cout << "(" << get<0>(i) << ", " << get<1>(i) << ") ";
    cout << "\n\n";
  }

  return stepsCounter.size();
}

pair<uint64_t, uint64_t> findStartingCoordinates(vector<string> gardenMap) {
  pair<uint64_t, uint64_t> start = {-1, -1};

  uint64_t row, column = -1;
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

  uint64_t total = 0;

  string line;
  vector<string> gardenMap;
  while (getline(input_file, line)) gardenMap.push_back(line);

  pair<uint64_t, uint64_t> startingCordinates =
      findStartingCoordinates(gardenMap);
  if (debug)
    cout << "Start: " << startingCordinates.first << ", "
         << startingCordinates.second << "\n\n";
  uint64_t mapSize = gardenMap.size();
  uint64_t steps = 26501365;

  assert(gardenMap.size() == gardenMap[0].size());

  assert(startingCordinates.first == startingCordinates.second &&
         startingCordinates.first == mapSize / 2);
  assert(steps % mapSize == mapSize / 2);

  uint64_t mapWidth = steps / mapSize - 1;

  uint64_t odd = (mapWidth / 2 * 2 + 1) * (mapWidth / 2 * 2 + 1);
  uint64_t even = ((mapWidth + 1) / 2 * 2) * ((mapWidth + 1) / 2 * 2);
  uint64_t odd_points = fillPlots(gardenMap, startingCordinates.first,
                                  startingCordinates.second, mapSize * 2 + 1);
  uint64_t even_points = fillPlots(gardenMap, startingCordinates.first,
                                   startingCordinates.second, mapSize * 2);

  uint64_t corner_t =
      fillPlots(gardenMap, mapSize - 1, startingCordinates.second, mapSize - 1);
  uint64_t corner_r =
      fillPlots(gardenMap, startingCordinates.first, 0, mapSize - 1);
  uint64_t corner_b =
      fillPlots(gardenMap, 0, startingCordinates.second, mapSize - 1);
  uint64_t corner_l =
      fillPlots(gardenMap, startingCordinates.first, mapSize - 1, mapSize - 1);

  uint64_t small_tr = fillPlots(gardenMap, mapSize - 1, 0, mapSize / 2 - 1);
  uint64_t small_tl =
      fillPlots(gardenMap, mapSize - 1, mapSize - 1, mapSize / 2 - 1);
  uint64_t small_br = fillPlots(gardenMap, 0, 0, mapSize / 2 - 1);
  uint64_t small_bl = fillPlots(gardenMap, 0, mapSize - 1, mapSize / 2 - 1);

  uint64_t large_tr = fillPlots(gardenMap, mapSize - 1, 0, mapSize * 3 / 2 - 1);
  uint64_t large_tl =
      fillPlots(gardenMap, mapSize - 1, mapSize - 1, mapSize * 3 / 2 - 1);
  uint64_t large_br = fillPlots(gardenMap, 0, 0, mapSize * 3 / 2 - 1);
  uint64_t large_bl = fillPlots(gardenMap, 0, mapSize - 1, mapSize * 3 / 2 - 1);

  total = odd * odd_points + even * even_points + corner_t + corner_r +
          corner_b + corner_l +
          (mapWidth + 1) * (small_tr + small_tl + small_br + small_bl) +
          mapWidth * (large_tr + large_tl + large_br + large_bl);
  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
