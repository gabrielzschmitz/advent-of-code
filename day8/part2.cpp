/**
 * @file part2.cpp
 * @brief Haunted Wasteland Ghost Navigation
 *
 * As the sandstorm intensifies, your attempts to escape the haunted wasteland
 * using left/right instructions have proven futile. In a desperate turn, you
 * contemplate the idea that the map might be for ghosts, unaffected by the laws
 * of spacetime. To test this, you decide to simultaneously start at every node
 * ending with A and follow all paths until all nodes end with Z.
 *
 * A curious fact emerges from further examination: the number of nodes ending
 * in A is equal to the number ending in Z. This prompts the simultaneous
 * exploration of paths from nodes ending in A until all nodes reached end with
 * Z.
 *
 * Example Input:
 * LR
 * 11A = (11B, XXX)
 * 11B = (XXX, 11Z)
 * 11Z = (11B, XXX)
 * 22A = (22B, XXX)
 * 22B = (22C, 22C)
 * 22C = (22Z, 22Z)
 * 22Z = (22B, 22B)
 * XXX = (XXX, XXX)
 *
 * Starting simultaneously at every node ending with A, follow left/right
 * instructions to navigate away from all current nodes. Repeat this process
 * until all nodes reached end with Z. Nodes ending with Z exclusively indicate
 * success.
 *
 * Your task is to determine the number of steps it takes before you're only on
 * nodes that end with Z when starting from nodes ending with A.
 *
 * Example Output:
 * Total: 6
 *
 * @author [gabrielzschmitz]
 * @date [08/12/2023]
 */
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

static bool readFile(const string& fileName, vector<string>& lines) {
  ifstream in{fileName};
  if (!in) {
    cerr << "Cannot open file " << fileName << endl;
    return false;
  }
  string str;
  while (getline(in, str)) {
    lines.push_back(str);
  }
  in.close();
  return true;
}

using Map = map<string, pair<string, string>>;

// Custom gcd function using Euclidean algorithm
uint64_t gcd(uint64_t a, uint64_t b) {
  while (b != 0) {
    uint64_t temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";
  vector<string> lines{};
  if (!readFile(user_input, lines)) return -1;

  Map map;
  for (size_t i = 2; i < lines.size(); ++i) {
    auto node = lines[i].substr(0, 3);
    auto l = lines[i].substr(7, 3);
    auto r = lines[i].substr(12, 3);
    map[node] = make_pair(l, r);
  }

  vector<string> nodes;
  vector<string> endNodes;
  for (const auto& kv : map) {
    const auto& node = kv.first;
    if (node[2] == 'A') {
      nodes.push_back(node);
    }
    if (node[2] == 'Z') {
      endNodes.push_back(node);
    }
  }

  auto cycles = vector<uint64_t>(nodes.size(), 0);
  uint64_t total = 0;

  do {
    const auto dir = lines[0][total % lines[0].size()];
    for (size_t i = 0; i < nodes.size(); ++i) {
      if (cycles[i] > 0) {
        continue;
      }
      auto& node = nodes[i];
      if ('L' == dir) {
        node = map.at(node).first;
      } else {
        node = map.at(node).second;
      }
      if (find(endNodes.begin(), endNodes.end(), node) != endNodes.end()) {
        cycles[i] = total + 1;
      }
    }
    total++;
  } while (
      !all_of(cycles.begin(), cycles.end(), [](uint64_t c) { return c > 0; }));

  // Define a custom lcm function since C++11 doesn't have std::lcm
  auto lcm = [](uint64_t a, uint64_t b) -> uint64_t {
    return (a / gcd(a, b)) * b;
  };

  total = 1;
  for (size_t i = 0; i < cycles.size(); ++i) {
    total = lcm(total, cycles[i]);
  }

  cout << "Total: " << total << endl;

  return 0;
}
