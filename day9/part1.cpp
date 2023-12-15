/**
 * @file part1.cpp
 * @brief Mirage Maintenance - OASIS Report Analysis
 *
 * As you find yourself standing at an oasis in the middle of Desert Island, you
 * decide to take ecological readings using the Oasis And Sand Instability
 * Sensor (OASIS). The OASIS produces a report with histories of values and how
 * they change over time. Your task is to predict the next value in each history
 * to safeguard the oasis.
 *
 * To make predictions, create a new sequence from the differences at each step
 * of the history. If the sequence is not all zeroes, repeat the process using
 * the new sequence as the input. Once all values in the latest sequence are
 * zeroes, you can extrapolate the next value for the original history.
 *
 * Example History:
 * 0 3 6 9 12 15
 * 1 3 6 10 15 21
 * 10 13 16 21 30 45
 *
 * Process:
 * Sequence 1: 3 3 3 3 3 -> Sequence 2: 0 0 0 0 -> Extrapolate: 18
 * Sequence 1: 2 3 4 5 6 -> Sequence 2: 1 1 1 1 -> Extrapolate: 28
 * Sequence 1: 3 3 5 9 15 -> Sequence 2: 0 2 4 6 8 -> Sequence 3: 2 2 2 2 ->
 * Extrapolate: 68
 *
 * Sum of Extrapolated Values: 114
 *
 * Your task is to analyze the OASIS report, extrapolate the next value for each
 * history, and find the sum of these extrapolated values.
 *
 * Example Input:
 * 0 3 6 9 12 15
 * 1 3 6 10 15 21
 * 10 13 16 21 30 45
 *
 * Example Output:
 * Total: 114
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
