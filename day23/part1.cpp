/**
 * @file part1.cpp
 * @brief A Long Walk - Find the Longest Hike through Hiking Trails
 *
 * Given a map of hiking trails represented by paths (.), forest (#), and steep
 * slopes (^, >, v, <), find the longest hike from the top row (S) to the bottom
 * row. Ensure that the hike is scenic, never stepping onto the same tile twice,
 * and follow the rules for icy slopes (if you step onto a slope tile, your next
 * step must be downhill). Return the length of the longest hike.
 *
 * Example Input:
 * #.#####################
 * #.......#########...###
 * #######.#########.#.###
 * ###.....#.>.>.###.#.###
 * ###v#####.#v#.###.#.###
 * ###.>...#.#.#.....#...#
 * ###v###.#.#.#########.#
 * ###...#.#.#.......#...#
 * #####.#.#.#######.#.###
 * #.....#.#.#.......#...#
 * #.#####.#.#.#########v#
 * #.#...#...#...###...>.#
 * #.#.#v#######v###.###v#
 * #...#.>.#...>.>.#.###.#
 * #####v#.#.###v#.#.###.#
 * #.....#...#...#.#.#...#
 * #.#########.###.#.#.###
 * #...###...#...#...#.###
 * ###.###.#.###v#####v###
 * #...#...#.#.>.>.#.>.###
 * #.###.###.#.###.#.#v###
 * #.....###...###...#...#
 * #####################.#
 *
 * Example Output:
 * Length of the Longest Hike: 94
 *
 * Determine the longest scenic hike while following the rules for icy slopes
 * and avoiding revisiting the same tile.
 *
 * @author [gabrielzschmitz]
 * @date [23/12/2023]
 */
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <stack>
using namespace std;

namespace std {
template <typename T1, typename T2>
struct hash<std::pair<T1, T2>> {
  size_t operator()(const std::pair<T1, T2>& p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);

    // Combine the hash values using a simple hash function
    return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
  }
};

}  // namespace std

bool debug = false;

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2>& p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);

    // Combine the hash values using a simple hash function
    return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
  }
};

int recursiveDFS(
    const pair<int, int>& pt, const pair<int, int>& end,
    const unordered_map<pair<int, int>, unordered_map<pair<int, int>, int>,
                        pair_hash>& graph,
    unordered_set<pair<int, int>, pair_hash>& seenSet) {
  if (pt == end) {
    return 0;
  }

  int m = -numeric_limits<int>::infinity();

  seenSet.insert(pt);
  for (const auto& entry : graph.at(pt)) {
    const auto& nx = entry.first;
    const auto& w = entry.second;
    if (seenSet.find(nx) == seenSet.end()) {
      m = max(m, recursiveDFS(nx, end, graph, seenSet) + w);
    }
  }
  seenSet.erase(pt);

  return m;
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
  vector<string> map;

  string line;
  while (getline(input_file, line)) map.push_back(line);

  pair<int, int> start = {0, map[0].find('.')};
  pair<int, int> end = {map.size() - 1, map[map.size() - 1].find('.')};
  vector<pair<int, int>> points = {start, end};
  if (debug)
    cout << "(" << start.first << ", " << start.second << ") (" << end.first
         << ", " << end.second << ")"
         << "\n\n";

  for (int row = 0; row < map.size(); ++row) {
    for (int column = 0; column < map[row].size(); ++column) {
      if (map[row][column] == '#') continue;

      int neighbors = 0;
      for (const auto& neighbor : vector<pair<int, int>>{{row - 1, column},
                                                         {row + 1, column},
                                                         {row, column - 1},
                                                         {row, column + 1}}) {
        int nRow = neighbor.first;
        int nColumn = neighbor.second;
        if (0 <= nRow && nRow < map.size() && 0 <= nColumn &&
            nColumn < map[0].size() && map[nRow][nColumn] != '#')
          neighbors += 1;
      }

      if (neighbors >= 3) points.emplace_back(row, column);
    }
  }

  unordered_map<pair<int, int>, unordered_map<pair<int, int>, int>, pair_hash>
      graph;
  unordered_map<char, vector<pair<int, int>>> dirs = {
      {'^', {{-1, 0}}},
      {'v', {{1, 0}}},
      {'<', {{0, -1}}},
      {'>', {{0, 1}}},
      {'.', {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}},
  };

  for (size_t i = 0; i < points.size(); ++i) {
    const auto& startRow = points[i].first;
    const auto& startColumn = points[i].second;

    stack<tuple<int, int, int>> stack;
    unordered_set<pair<int, int>, pair_hash> seen;

    stack.push({0, startRow, startColumn});
    seen.insert({startRow, startColumn});

    while (!stack.empty()) {
      auto top = stack.top();
      stack.pop();
      auto n = get<0>(top);
      auto row = get<1>(top);
      auto column = get<2>(top);

      if (n != 0 && find(points.begin(), points.end(),
                         make_pair(row, column)) != points.end()) {
        graph[{startRow, startColumn}][{row, column}] = n;
        continue;
      }

      for (const auto& entry : dirs[map[row][column]]) {
        const auto& dRow = entry.first;
        const auto& dColumn = entry.second;

        int nRow = row + dRow;
        int nColumn = column + dColumn;
        if (0 <= nRow && nColumn < map.size() && 0 <= nColumn &&
            nColumn < map[0].size() && map[nRow][nColumn] != '#' &&
            seen.find({nRow, nColumn}) == seen.end()) {
          stack.push({n + 1, nRow, nColumn});
          seen.insert({nRow, nColumn});
        }
      }
    }
  }

  unordered_set<pair<int, int>, pair_hash> seenSet;

  total = recursiveDFS(start, end, graph, seenSet);
  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
