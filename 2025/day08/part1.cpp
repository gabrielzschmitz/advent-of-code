/**
 * @file part1.cpp
 * @brief Playground Junction Box Connections (Day 8: Playground)
 *
 * The playground contains many junction boxes suspended in 3D space. Each box
 * is specified by integer coordinates X,Y,Z (one box per input line).
 *
 * Two boxes can be connected by a string of lights; connecting two boxes
 * allows electricity to flow between them and therefore merges their
 * electrical circuits. To conserve cable, the Elves repeatedly connect the
 * two junction boxes that are closest together (by Euclidean straight-line
 * distance). If the chosen pair is already in the same circuit, that
 * connection does nothing.
 *
 * Task:
 *  - Read the list of junction box coordinates.
 *  - Consider every unordered pair of boxes and sort those pairs by
 *    Euclidean distance (shortest first).
 *  - Take the 1000 shortest pairs (ties may be broken arbitrarily) and for
 *    each pair union the two boxes' circuits (i.e. perform a union operation).
 *  - After performing these 1000 connections, compute the sizes (counts of
 *    boxes) of all connected circuits.
 *  - Find the three largest circuit sizes and return the product of those
 *    three sizes.
 *
 * Implementation notes / hints:
 *  - Use a union-find / disjoint-set data structure to efficiently merge
 *    circuits and track component sizes.
 *  - Generating all pairs for N boxes requires O(N^2) distance calculations.
 *    For large N, consider memory/time tradeoffs (e.g. partial selection via
 *    a min-heap, or storing pairs compactly). Sort the pair distances and
 *    process the first 1000 unique pairs.
 *  - Distances may be compared using squared Euclidean distance (to avoid
 *    unnecessary sqrt calls) because only ordering matters.
 *
 * Example:
 *    162,817,812
 *    57,618,57
 *    906,360,560
 *    592,479,940
 *    352,342,300
 *    466,668,158
 *    542,29,236
 *    431,825,988
 *    739,650,466
 *    52,470,668
 *    216,146,977
 *    819,987,18
 *    117,168,530
 *    805,96,715
 *    346,949,466
 *    970,615,88
 *    941,993,340
 *    862,61,35
 *    984,92,344
 *    425,690,689
 *  - For the 20-box sample in the prompt, after processing the ten shortest
 *    connections the circuit-size multiset is {5,4,2,2,1,1,1,1,1,1,1} and the
 *    product of the three largest (5 * 4 * 2) is 40.
 *
 * Output:
 *  - Print a single integer: the product of the sizes of the three largest
 *    circuits after performing the 1000 shortest connections.
 *
 * Puzzle Answer: [Product of sizes of three largest circuits]
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

struct DSU {
  int n;
  std::vector<int> parent, sz;

  DSU(int n) : n(n), parent(n), sz(n, 1) {
    iota(parent.begin(), parent.end(), 0);
  }

  int find(int a) {
    while (parent[a] != a) {
      parent[a] = parent[parent[a]];
      a = parent[a];
    }
    return a;
  }

  void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b)
      return;
    if (sz[a] < sz[b])
      std::swap(a, b);
    parent[b] = a;
    sz[a] += sz[b];
  }

  int size(int a) { return sz[find(a)]; }
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

  while (getline(input_file, line)) {
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
  if (N == 0) {
    std::cout << "ANSWER: 0\n";
    return 0;
  }

  DSU dsu(N);

  struct Pair {
    int i, j;
    int64_t d;
    bool operator<(const Pair &other) const { return d < other.d; }
  };

  int K = 1000;
  std::priority_queue<Pair> pq;

  for (int i = 0; i < N; i++) {
    auto [x1, y1, z1] = pts[i];
    for (int j = i + 1; j < N; j++) {
      auto [x2, y2, z2] = pts[j];
      int64_t dx = x1 - x2;
      int64_t dy = y1 - y2;
      int64_t dz = z1 - z2;
      int64_t dist = dx * dx + dy * dy + dz * dz;

      if ((int)pq.size() < K) {
        pq.push({i, j, dist});
      } else if (dist < pq.top().d) {
        pq.pop();
        pq.push({i, j, dist});
      }
    }
  }

  std::vector<Pair> edges;
  edges.reserve(pq.size());
  while (!pq.empty()) {
    edges.push_back(pq.top());
    pq.pop();
  }
  sort(edges.begin(), edges.end(),
       [](const Pair &a, const Pair &b) { return a.d < b.d; });

  for (auto &e : edges)
    dsu.unite(e.i, e.j);

  std::vector<int> comp;
  comp.reserve(N);

  std::unordered_map<int, int> mp;
  mp.reserve(N * 2);

  for (int i = 0; i < N; i++) {
    int r = dsu.find(i);
    mp[r] = dsu.sz[r];
  }

  for (auto &kv : mp)
    comp.push_back(kv.second);

  sort(comp.begin(), comp.end(), std::greater<int>());

  int64_t total = 1;
  for (int i = 0; i < std::min(3, (int)comp.size()); i++)
    total *= comp[i];

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
