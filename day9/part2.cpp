/**
 * @file part2.cpp
 * @brief Mirage Maintenance - OASIS Report Backward Extrapolation
 *
 * To enhance the historical context in your OASIS report, you decide to
 * extrapolate backward in time as well. For each history, repeat the process of
 * finding differences until the sequence of differences is entirely zero. Add a
 * zero to the beginning of the sequence of zeroes and fill in new first values
 * for each previous sequence.
 *
 * Example History:
 * 5  10  13  16  21  30  45
 * 5   3   3   5   9  15
 * -2   0   2   4   6
 * 2   2   2   2
 * 0   0   0
 *
 * Extrapolate Backward:
 * New Values: 5, -3, 0
 * Sum of Extrapolated Values: 2
 *
 * Your task is to analyze the OASIS report, extrapolate the previous value for
 * each history, and find the sum of these extrapolated values.
 *
 * Example Input:
 * 0 3 6 9 12 15
 * 1 3 6 10 15 21
 * 10 13 16 21 30 45
 *
 * Example Output:
 * Total: 2
 *
 * @author [gabrielzschmitz]
 * @date [09/12/2023]
 */
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <sstream>
using namespace std;

void printVector(const vector<int>& vec, const string& name) {
  cout << name << ": ";
  for (int current : vec) {
    cout << current << ", ";
  }
  cout << endl;
}

vector<int> reverseVector(const vector<int>& original) {
  vector<int> reversed(original.rbegin(), original.rend());
  return reversed;
}

vector<int> differenceVector(const vector<int> vec) {
  vector<int> diff;
  for (int i = 0; i < vec.size() - 1; i++) {
    diff.push_back(vec[i + 1] - vec[i]);
  }
  return diff;
}

bool allZeros(const std::vector<int>& vec) {
  for (const int& num : vec) {
    if (num != 0) {
      return false;
    }
  }
  return true;
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
  while (getline(input_file, line)) {
    vector<int> numbers;
    istringstream iss(line);
    vector<vector<int>> layers;

    int num;
    while (iss >> num) {
      numbers.push_back(num);
    }
    numbers = reverseVector(numbers);

    vector<int> diff = differenceVector(numbers);
    layers.push_back(numbers);
    while (!allZeros(layers.back())) {
      layers.push_back(differenceVector(layers.back()));
    }

    int i = layers.size() - 1;
    layers.back().push_back(0);
    int current = -1;
    while (i != 0) {
      current = layers[i].back();
      current = current + layers[i - 1].back();
      layers[i - 1].push_back(current);
      i--;
    }

    total = total + current;
  }

  cout << "Total: " << total << endl;

  return 0;
}
