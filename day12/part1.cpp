/**
 * @file part1.cpp
 * @brief Hot Springs - Count Possible Arrangements
 *
 * Repair the damaged condition records of springs in a giant field.
 * For each row, count the different arrangements of operational and broken
 * springs based on the damaged condition records and size information.
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
 * Sum of Possible Arrangements: 21
 *
 * @author [gabrielzschmitz]
 * @date [12/12/2023]
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>
using namespace std;

namespace std {
template <typename T>
struct hash<std::tuple<T, T, T>> {
  size_t operator()(const std::tuple<T, T, T>& t) const {
    auto combined_hash = hash<T>{}(get<0>(t));
    combined_hash ^= hash<T>{}(get<1>(t)) + 0x9e3779b9 + (combined_hash << 6) +
                     (combined_hash >> 2);
    combined_hash ^= hash<T>{}(get<2>(t)) + 0x9e3779b9 + (combined_hash << 6) +
                     (combined_hash >> 2);
    return combined_hash;
  }
};
}  // namespace std

unordered_map<tuple<int, int, int>, int> DP;

int f(const string& dots, const vector<int>& blocks, int i, int bi,
      int current) {
  tuple<int, int, int> key = make_tuple(i, bi, current);
  if (DP.find(key) != DP.end()) {
    return DP[key];
  }
  if (i == dots.length()) {
    if (bi == blocks.size() && current == 0) {
      return 1;
    } else if (bi == blocks.size() - 1 && blocks[bi] == current) {
      return 1;
    } else {
      return 0;
    }
  }
  int ans = 0;
  for (char c : {'.', '#'}) {
    if (dots[i] == c || dots[i] == '?') {
      if (c == '.' && current == 0) {
        ans += f(dots, blocks, i + 1, bi, 0);
      } else if (c == '.' && current > 0 && bi < blocks.size() &&
                 blocks[bi] == current) {
        ans += f(dots, blocks, i + 1, bi + 1, 0);
      } else if (c == '#') {
        ans += f(dots, blocks, i + 1, bi, current + 1);
      }
    }
  }
  DP[key] = ans;
  return ans;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " input_file" << endl;
    return 1;
  }

  ifstream input_file(argv[1]);
  if (!input_file.is_open()) {
    cerr << "Error opening file: " << argv[1] << endl;
    return 1;
  }

  string line;
  int scores = 0;
  while (getline(input_file, line)) {
    vector<string> tokens;
    size_t pos = 0;
    while ((pos = line.find(' ')) != string::npos) {
      tokens.push_back(line.substr(0, pos));
      line.erase(0, pos + 1);
    }
    tokens.push_back(line);

    string dots = tokens[0];
    vector<int> blocks;
    for (size_t i = 0; i < tokens[1].length(); ++i) {
      if (tokens[1][i] == ',') {
        blocks.push_back(stoi(tokens[1].substr(0, i)));
        tokens[1].erase(0, i + 1);
        i = 0;
      }
    }
    blocks.push_back(stoi(tokens[1]));

    DP.clear();
    int score = f(dots, blocks, 0, 0, 0);
    scores = score + scores;
  }

  cout << scores << endl;
  input_file.close();

  return 0;
}
