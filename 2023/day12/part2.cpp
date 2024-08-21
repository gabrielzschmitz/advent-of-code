/**
 * @file part2.cpp
 * @brief Hot Springs - Unfolded Condition Records and Count Possible
 * Arrangements
 *
 * Unfold the damaged condition records of springs in a giant field and count
 * the different arrangements of operational and broken springs based on the
 * unfolded condition records and size information.
 *
 * Example Input:
 * ???.### 1,1,3
 * .??..??...?##. 1,1,3
 * ?#?#?#?#?#?#?#? 1,3,1,6
 * ????.#...#... 4,1,1
 * ????.######..#####. 1,6,5
 * ?###???????? 3,2,1
 *
 * Example Output:
 * Sum of Possible Arrangements: 525152
 *
 * Unfold the condition records and calculate the new sum of possible
 * arrangements.
 *
 * @author [gabrielzschmitz]
 * @date [12/12/2023]
 */
#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

using Group = vector<uint32_t>;
using Puzzle = pair<string, Group>;
using Puzzles = vector<Puzzle>;

static bool readFile(const string& fileName, vector<string>& lines) {
  ifstream in(fileName.c_str());
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

template <size_t FOLD = 1, bool WITH_CACHE = true>
static size_t count(
  const string& p, const Group& g, size_t pos = 0, size_t k = 0,
  shared_ptr<map<pair<size_t, size_t>, size_t>> cache = nullptr) {
  if (WITH_CACHE) {
    assert(nullptr != cache);
    auto it = cache->find({pos, k});
    if (it != cache->end()) { return it->second; }
  }
  if (pos == p.size() && k == FOLD * g.size()) { return 1; }
  if (pos >= p.size()) { return 0; }
  if ('#' == p[pos]) {
    if (k >= FOLD * g.size() || pos + g[k % g.size()] >= p.size()) { return 0; }
    for (size_t c = 1; c < g[k % g.size()]; ++c) {
      if ('.' == p[pos + c]) { return 0; }
    }
    pos += g[k % g.size()];
    k++;
    if ('#' == p[pos]) { return 0; }
    const auto s = count<FOLD, WITH_CACHE>(p, g, pos + 1, k, cache);
    if (WITH_CACHE) { cache->insert({{pos + 1, k}, s}); }
    return s;
  }
  if ('.' == p[pos]) {
    return count<FOLD, WITH_CACHE>(p, g, pos + 1, k, cache);
  }
  const string p1 =
    p.substr(0, pos) + '#' + p.substr(pos + 1, p.size() - pos - 1);
  const string p2 =
    p.substr(0, pos) + '.' + p.substr(pos + 1, p.size() - pos - 1);
  return count<FOLD, WITH_CACHE>(p1, g, pos, k, cache) +
         count<FOLD, WITH_CACHE>(p2, g, pos, k, cache);
}

static inline size_t count1(const string& p, const Group& g) {
  return count<1, false>(p, g);
}

static inline size_t count5(const string& p, const Group& g) {
  return count<5>(p, g, 0, 0, make_shared<map<pair<size_t, size_t>, size_t>>());
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";
  vector<string> lines{};
  if (!readFile(user_input, lines)) return -1;

  Puzzles puzzles;
  for (const auto& line : lines) {
    uint32_t x;
    string p;
    istringstream iss(line);
    iss >> p;
    p = p + '?' + p + '?' + p + '?' + p + '?' + p + '.';
    Group g;
    while (iss >> x) {
      g.push_back(x);
      iss.ignore(1);
    }
    puzzles.push_back({p, g});
  }

  size_t total{};
  for (const auto& puzzle : puzzles) {
    total += count5(puzzle.first, puzzle.second);
  }
  cout << "NUMBER:" << total << "\n";

  return 0;
}
