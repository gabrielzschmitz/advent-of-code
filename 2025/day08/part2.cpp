/**
 * @file part2.cpp
 * @brief Playground Junction Box Final Merge (Day 8: Playground — Part Two)
 *
 * After making the first 1000 closest connections (Part One), the Elves realize
 * they still don't have enough extension cables. You must continue connecting
 * pairs of junction boxes — always selecting the currently closest pair of
 * boxes that are *not yet in the same circuit* — until **all** junction boxes
 * belong to a single connected circuit.
 *
 * When processing pairs:
 *   - Each pair consists of two junction boxes with integer coordinates
 * (X,Y,Z).
 *   - Pairs are sorted in increasing order of Euclidean distance. You may use
 *     squared distances to avoid computing square roots, since only ordering
 *     matters.
 *   - When the next closest pair is examined:
 *       • If the two boxes are in different circuits, perform a union and
 * record that this pair was used as an actual connection. • If they are already
 * connected, skip it.
 *
 * Task:
 *   - Continue processing pairs in order until the union–find structure shows
 *     that all junction boxes have merged into one connected component.
 *   - The *final* connection — the pair that merges the *last* two previously
 *     separate circuits — is important.
 *   - Let the X-coordinates of that last pair be X₁ and X₂.
 *   - Output the value X₁ × X₂.
 *
 * Example (from prompt):
 *   - In the 20-box example, the final connection needed to unify all circuits
 *     is between:
 *         (216,146,977) and (117,168,530)
 *   - Their X-coordinates multiply to 216 × 117 = 25272.
 *
 * Output:
 *   - A single integer: the product of the X-coordinates of the last pair of
 *     junction boxes whose connection completes the single unified circuit.
 *
 * Notes:
 *   - The union–find (disjoint-set) data structure is required for efficiency.
 *   - All unordered pairs must be considered and sorted; constraints will
 *     determine whether you should store all distances, use a streamed k-way
 *     merge, or maintain a min-heap.
 *   - The answer depends solely on identifying the final edge that collapses
 *     the last two components.
 *
 * Puzzle Answer: [Product of X-coordinates of the final merged pair]
 *
 * @author [gabrielzschmitz]
 * @date [08/12/2025]
 */
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
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

struct DSU {
  int n;
  std::vector<int> parent, sz;

  DSU(int n) : n(n), parent(n), sz(n, 1) {
    std::iota(parent.begin(), parent.end(), 0);
  }

  int find(int a) {
    while (parent[a] != a) {
      parent[a] = parent[parent[a]];
      a = parent[a];
    }
    return a;
  }

  bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b)
      return false;
    if (sz[a] < sz[b])
      std::swap(a, b);
    parent[b] = a;
    sz[a] += sz[b];
    return true;
  }

  int count_roots() {
    int c = 0;
    for (int i = 0; i < n; i++)
      if (find(i) == i)
        c++;
    return c;
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

  std::vector<std::array<int64_t, 3>> pts;
  std::string line;

  while (std::getline(input_file, line)) {
    if (line.empty())
      continue;
    int64_t x, y, z;
    char c1, c2;
    std::stringstream ss(line);
    ss >> x >> c1 >> y >> c2 >> z;
    pts.push_back({x, y, z});
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int N = pts.size();
  if (N <= 1) {
    std::cout << "ANSWER: 0\n";
    return 0;
  }

  DSU dsu(N);

  struct Edge {
    int i, j;
    int64_t d;
  };
  std::vector<Edge> edges;
  edges.reserve((size_t)N * (N - 1) / 2);

  for (int i = 0; i < N; i++) {
    auto [x1, y1, z1] = pts[i];
    for (int j = i + 1; j < N; j++) {
      auto [x2, y2, z2] = pts[j];
      int64_t dx = x1 - x2;
      int64_t dy = y1 - y2;
      int64_t dz = z1 - z2;
      int64_t dist = dx * dx + dy * dy + dz * dz;
      edges.push_back({i, j, dist});
    }
  }

  std::sort(edges.begin(), edges.end(),
            [](auto &a, auto &b) { return a.d < b.d; });

  int components = N;
  int last_i = -1, last_j = -1;

  for (auto &e : edges) {
    if (dsu.unite(e.i, e.j)) {
      components--;
      last_i = e.i;
      last_j = e.j;
      if (components == 1)
        break;
    }
  }

  int64_t X1 = pts[last_i][0];
  int64_t X2 = pts[last_j][0];
  int64_t total = X1 * X2;

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
