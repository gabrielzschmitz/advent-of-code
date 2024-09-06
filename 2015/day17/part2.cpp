/**
 * @file part2.cpp
 * @brief Minimal Container Eggnog Storage
 *
 * This program determines the minimum number of containers needed to exactly
 * fit a given amount of eggnog and calculates how many different ways this can
 * be done. The containers must be filled entirely, and each container can be
 * used at most once.
 *
 * The task is to find the minimum number of containers that can exactly store
 * the target volume of eggnog, and how many different combinations of those
 * minimal containers exist.
 *
 * For example:
 * If you have containers of size 20, 15, 10, 5, and 5 liters, and you need to
 * store 25 liters of eggnog, the minimal number of containers is 2, and there
 * are 3 ways to achieve this:
 * - 15 and 10
 * - 20 and 5 (the first 5)
 * - 20 and 5 (the second 5)
 *
 * The program processes the container sizes and computes how many different
 * ways the minimum number of containers can exactly fit the target volume.
 *
 * Example Input:
 * - Target volume: 150 liters
 * - Container sizes: [20, 15, 10, 5, 5]
 *
 * Example Output:
 * - Minimum number of containers: 2
 * - Number of valid combinations: 3
 *
 * Puzzle Answer: [Number of Valid Combinations]
 *
 * @author [gabrielzschmitz]
 * @date [06/09/2024]
 */
#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool debug = false;
int liters_of_eggnog = 150;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") {
    debug = true;
    liters_of_eggnog = 25;
  }
}

void findCombinations(const std::vector<int>& containers, int index,
                      int current_sum, int current_containers, int& total,
                      int& min_containers, int& ways_with_min) {
  if (current_sum == liters_of_eggnog) {
    total++;
    if (current_containers < min_containers) {
      min_containers = current_containers;
      ways_with_min = 1;
    } else if (current_containers == min_containers) {
      ways_with_min++;
    }
    return;
  }

  if (current_sum > liters_of_eggnog || index >= containers.size()) { return; }

  findCombinations(containers, index + 1, current_sum + containers[index],
                   current_containers + 1, total, min_containers,
                   ways_with_min);
  findCombinations(containers, index + 1, current_sum, current_containers,
                   total, min_containers, ways_with_min);
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string line;
  std::vector<int> containers;
  while (std::getline(input_file, line)) {
    int current_container = std::stoi(line);
    containers.push_back(current_container);
  }
  if (debug)
    for (int i : containers)
      std::cout << i << " \n";

  int total_combinations = 0;
  int min_containers = INT_MAX;
  int ways_with_min_containers = 0;
  findCombinations(containers, 0, 0, 0, total_combinations, min_containers,
                   ways_with_min_containers);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = ways_with_min_containers;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
