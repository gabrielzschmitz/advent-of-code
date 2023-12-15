/**
 * @file part2.cpp
 * @brief Cosmic Expansion - Extended Universe Shortest Paths
 *
 * Explore the extended universe with galaxies and expanded empty space.
 * Rows or columns containing no galaxies are expanded one million times larger.
 * Calculate the sum of the lengths of the shortest path between every pair of
 * galaxies.
 *
 * For each pair of galaxies, find the shortest path using only steps that move
 * up, down, left, or right exactly one '.' or '#' at a time. Order within the
 * pair doesn't matter.
 *
 * Example Shortest Path Lengths:
 * Between galaxy 1 and galaxy 7: 15
 * Between galaxy 3 and galaxy 6: 17
 * Between galaxy 8 and galaxy 9: 5
 *
 * Example Input:
 * Extended Universe:
 * ...#......
 * .......#..
 * #.........
 * ..........
 * ......#...
 * .#........
 * .........#
 * ..........
 * .......#..
 * #...#.....
 *
 * Example Output:
 * Sum of Shortest Path Lengths: 82000210
 *
 * @author [gabrielzschmitz]
 * @date [11/12/2023]
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cinttypes>
using namespace std;

void markGalaxiesAndEmptys(const vector<string>& inputVector,
                           vector<tuple<int, int>>& galaxies,
                           vector<string>& emptys) {
  int numRows = inputVector.size();
  int numCols = (numRows > 0) ? inputVector[0].size() : 0;

  for (int i = 0; i < numRows; ++i) {
    for (int j = 0; j < numCols; ++j) {
      if (inputVector[i][j] == '#') {
        galaxies.push_back(make_tuple(i, j));
      }
    }
    emptys[i] = string(numCols, '.');

    if (inputVector[i].find_first_not_of('.') == string::npos) {
      emptys[i] = string(numCols, '1');
    } else {
      for (int j = 0; j < numCols; ++j) {
        bool isColumnEmpty = true;
        for (int k = 0; k < numRows; ++k) {
          if (inputVector[k][j] != '.') {
            isColumnEmpty = false;
            break;
          }
        }
        if (isColumnEmpty) {
          emptys[i][j] = '1';
        }
      }
    }
  }
}

uint64_t distance(const tuple<int, int>& A, const tuple<int, int>& B,
                  const vector<string>& binaryMatrix) {
  int i1, j1, i2, j2;
  tie(i1, j1) = A;
  tie(i2, j2) = B;

  uint64_t distance = 0;

  // Calculate distance along rows
  for (int i = min(i1, i2); i < max(i1, i2); ++i) {
    distance += (binaryMatrix[i][j1] == '1') ? 1000000 : 1;
  }

  // Calculate distance along columns
  for (int j = min(j1, j2); j < max(j1, j2); ++j) {
    distance += (binaryMatrix[i1][j] == '1') ? 1000000 : 1;
  }

  return distance;
}

uint64_t sumDistances(const vector<tuple<int, int>>& galaxies,
                      const vector<string>& binaryMatrix) {
  int numGalaxies = galaxies.size();
  uint64_t sumDistances = 0;

  // Loop through all pairs of galaxies without repeating
  for (int i = 0; i < numGalaxies - 1; ++i) {
    for (int j = i + 1; j < numGalaxies; ++j) {
      uint64_t result = distance(galaxies[i], galaxies[j], binaryMatrix);
      sumDistances += result;
    }
  }

  return sumDistances;
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  uint64_t total = 0;
  string line;

  vector<string> universe;
  while (getline(input_file, line)) {
    universe.push_back(line);
  }

  int numRows = universe.size();
  int numCols = (numRows > 0) ? universe[0].size() : 0;
  vector<string> emptys(numRows, string(numCols, '.'));
  vector<tuple<int, int>> galaxies = {};

  markGalaxiesAndEmptys(universe, galaxies, emptys);

  total = sumDistances(galaxies, emptys);

  cout << "NUMBER:" << total << endl;

  return 0;
}
