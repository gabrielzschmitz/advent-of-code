/**
 * @file part1.cpp
 * @brief Reconciling Location ID Lists
 *
 * In this program, you help a group of Elvish Senior Historians reconcile two
 * lists of historically significant location IDs. These lists were found in the
 * Chief Historian's office and need to be compared to determine the total
 * distance between the corresponding location IDs.
 *
 * The lists are reconciled by pairing the smallest number from the left list
 * with the smallest number from the right list, the second-smallest from the
 * left with the second-smallest from the right, and so on. For each pair, the
 * distance is calculated as the absolute difference between the two numbers.
 * The task is to compute the sum of these distances.
 *
 * Example Input:
 * Left List:  [3, 4, 2, 1, 3, 3]
 * Right List: [4, 3, 5, 3, 9, 3]
 *
 * Pairing and Distances:
 * - Pair (1, 3), Distance = 2
 * - Pair (2, 3), Distance = 1
 * - Pair (3, 3), Distance = 0
 * - Pair (3, 4), Distance = 1
 * - Pair (3, 5), Distance = 2
 * - Pair (4, 9), Distance = 5
 *
 * Total Distance: 2 + 1 + 0 + 1 + 2 + 5 = 11
 *
 * The program processes the two lists of IDs, computes the pairwise distances,
 * and outputs the total distance between the lists.
 *
 * Puzzle Answer: [Total Distance]
 *
 * @author [gabrielzschmitz]
 * @date [04/12/2024]
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<int> left, right;
  int l, r;

  while (input_file >> l >> r) {
    left.push_back(l);
    right.push_back(r);
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  std::vector<std::pair<int, int>> sorted_pairs;
  for (size_t i = 0; i < std::min(left.size(), right.size()); ++i)
    sorted_pairs.emplace_back(left[i], right[i]);

  int total = 0;
  for (size_t i = 0; i < sorted_pairs.size(); ++i) {
    std::pair<int, int> current = sorted_pairs.at(i);
    int dist = std::abs(current.first - current.second);
    total += dist;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
