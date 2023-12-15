/**
 * @file part2.cpp
 * @brief Point of Incidence - Fixing Smudges and Summarizing New Reflection
 * Lines
 *
 * Analyze patterns of ash (.) and rocks (#) in a valley of mirrors.
 * Locate and fix smudges in each pattern, causing different reflection lines.
 * Calculate a numerical summary based on the new reflection lines.
 * Sum up the calculated numbers from all patterns to obtain the final result.
 *
 * Example Input:
 * #.##..##.
 * ..#.##.#.
 * ##......#
 * ##......#
 * ..#.##.#.
 * ..##..##.
 * #.#.##.#.
 *
 * #...##..#
 * #....#..#
 * ..##..###
 * #####.##.
 * #####.##.
 * ..##..###
 * #....#..#
 *
 * Example Output:
 * Sum of New Reflection Numbers: 400
 *
 * In each pattern, fix the smudge and find the different line of reflection.
 * Summarize the new reflection lines as before to obtain the final result.
 *
 * @author [gabrielzschmitz]
 * @date [13/12/2023]
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>
#include <algorithm>
using namespace std;

bool debug = false;

bool isHorizontal(vector<string> pattern, int index) {
  int n = pattern.size();
  int m = pattern[0].size();

  for (int i = 0; i < m; i++) {
    for (int k1 = 0; k1 < n; k1++) {
      int k2 = index * 2 + 1 - k1;
      if (!(0 <= k2 && k2 < n)) continue;
      if (pattern[k1][i] != pattern[k2][i]) return false;
    }
  }

  return true;
}

vector<string> transpose(const vector<string>& input) {
  if (input.empty()) {
    cerr << "Input vector is empty. Cannot transpose." << endl;
    return {};
  }

  size_t rows = input.size();
  size_t cols = input[0].size();

  vector<string> result(cols, string(rows, ' '));

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      result[j][i] = input[i][j];
    }
  }

  return result;
}

pair<int, int> processPattern(vector<string> pattern,
                              pair<int, int> avoid = {-1, -1}) {
  if (debug) {
    for (string i : pattern) cout << i << endl;
    cout << endl;
  }
  int n = pattern.size();
  int m = pattern[0].size();

  int horizontal = -1;
  for (int index = 0; index < n - 1; index++)
    if (index != avoid.first && isHorizontal(pattern, index)) {
      horizontal = index;
      break;
    }

  int vertical = -1;
  vector<string> transposedPattern = transpose(pattern);
  for (int index = 0; index < m - 1; index++)
    if (index != avoid.second && isHorizontal(transposedPattern, index)) {
      vertical = index;
      break;
    }

  return {horizontal, vertical};
}

int summarizePattern(const vector<string>& pattern) {
  int n = pattern.size();
  int m = pattern[0].length();
  pair<int, int> summ_og = processPattern(pattern);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      vector<string> pattern_copy = pattern;
      if (pattern[i][j] == '#')
        pattern_copy[i][j] = '.';
      else
        pattern_copy[i][j] = '#';

      pair<int, int> summ_new = processPattern(pattern_copy, summ_og);

      if (summ_new != summ_og && summ_new != make_pair(-1, -1)) {
        int contrib = 0;
        if (summ_new.first != -1)
          return (summ_new.first + 1) * 100;
        else
          return (summ_new.second + 1);
      }
    }
  }
  return 0;
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  int total = 0;
  string line;
  vector<string> pattern;
  while (getline(input_file, line)) {
    if (line == "") {
      total += summarizePattern(pattern);
      pattern = {};
    } else {
      pattern.push_back(line);
    }
  }
  total += summarizePattern(pattern);

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
