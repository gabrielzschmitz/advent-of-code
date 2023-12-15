/**
 * @file part1.cpp
 * @brief Point of Incidence - Mirror Reflection Summation
 *
 * Analyze patterns of ash (.) and rocks (#) in a valley of mirrors.
 * Determine the lines of reflection (vertical or horizontal) for each pattern,
 * and calculate a numerical summary based on the reflection lines.
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
 * Sum of Reflection Numbers: 405
 *
 * To summarize your pattern notes, add up the number of columns to the left
 * of each vertical line of reflection; to that, also add 100 multiplied by
 * the number of rows above each horizontal line of reflection. In the above
 * example, the first pattern's vertical line has 5 columns to its left and
 * the second pattern's horizontal line has 4 rows above it, resulting in a
 * total of 405.
 *
 * @author [gabrielzschmitz]
 * @date [13/12/2023]
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
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

void processPattern(vector<string> pattern, int& total) {
  if (debug) {
    for (string i : pattern) cout << i << endl;
    cout << endl;
  }
  int n = pattern.size();
  int m = pattern[0].size();

  int horizontal = -1;
  for (int index = 0; index < n - 1; index++)
    if (isHorizontal(pattern, index)) {
      horizontal = index;
      break;
    }

  int vertical = -1;
  vector<string> transposedPattern = transpose(pattern);
  for (int index = 0; index < m - 1; index++)
    if (isHorizontal(transposedPattern, index)) {
      vertical = index;
      break;
    }
  total += (vertical + 1) + (100 * (horizontal + 1));
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
      processPattern(pattern, total);
      pattern = {};
    } else {
      pattern.push_back(line);
    }
  }
  processPattern(pattern, total);

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
