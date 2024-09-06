/**
 * @file part1.cpp
 * @brief Eggnog Storage Combinations
 *
 * This program determines how many different ways you can fit a specific amount
 * of eggnog into a set of containers with various capacities. You are given a
 * target volume and an inventory of container sizes, and the task is to find
 * how many combinations of these containers can exactly match the target
 * volume.
 *
 * The containers must be filled entirely, and each container can be used at 
 * most once.
 *
 * For example:
 * If you have containers of size 20, 15, 10, 5, and 5 liters, and you need to 
 * store 25 liters of eggnog, the following combinations are valid:
 * - 15 and 10
 * - 20 and 5 (the first 5)
 * - 20 and 5 (the second 5)
 * - 15, 5, and 5
 *
 * The program processes the container sizes and computes how many different 
 * combinations can exactly fit the specified amount of eggnog.
 *
 * Example Input:
 * - Target volume: 150 liters
 * - Container sizes: [20, 15, 10, 5, 5]
 *
 * Example Output:
 * - Number of valid combinations: 4
 *
 * Puzzle Answer: [Number of Combinations]
 *
 * @author [gabrielzschmitz]
 * @date [06/09/2024]
 */
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
                      int current_sum, int& total) {
  if (current_sum == liters_of_eggnog) {
    total++;
    return;
  }

  if (current_sum > liters_of_eggnog || index >= containers.size()) return;

  findCombinations(containers, index + 1, current_sum + containers[index],
                   total);
  findCombinations(containers, index + 1, current_sum, total);
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  int total = 0;

  std::string line;
  std::vector<int> containers;
  while (std::getline(input_file, line)) {
    int current_container = std::stoi(line);
    containers.push_back(current_container);
  }
  if (debug)
    for (int i : containers)
      std::cout << i << " \n";

  findCombinations(containers, 0, 0, total);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
