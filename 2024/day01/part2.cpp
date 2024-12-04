/**
 * @file part2.cpp
 * @brief Calculating Similarity Score Between Location ID Lists
 *
 * In this continuation of the Historian Hysteria challenge, the task is to
 * calculate a similarity score between two lists of location IDs. Each number
 * in the left list is checked against the right list, and the similarity score
 * is determined by summing the product of each number from the left list and
 * the number of times it appears in the right list.
 *
 * Example Input:
 * Left List:  [3, 4, 2, 1, 3, 3]
 * Right List: [4, 3, 5, 3, 9, 3]
 *
 * Similarity Score Calculation:
 * - Number 3 appears 3 times in the right list; score += 3 * 3 = 9
 * - Number 4 appears 1 time in the right list; score += 4 * 1 = 4
 * - Number 2 does not appear in the right list; score += 2 * 0 = 0
 * - Number 1 does not appear in the right list; score += 1 * 0 = 0
 * - Number 3 appears 3 times in the right list; score += 3 * 3 = 9
 * - Number 3 appears 3 times in the right list; score += 3 * 3 = 9
 *
 * Total Similarity Score: 9 + 4 + 0 + 0 + 9 + 9 = 31
 *
 * The program processes the two lists, calculates the similarity score based on
 * occurrences in the right list, and outputs the final score.
 *
 * Puzzle Answer: [Similarity Score]
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

  int total = 0;
  for (const int& left_element : left) {
    int count = std::count(right.begin(), right.end(), left_element);
    total += left_element * count;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
