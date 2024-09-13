/**
 * @file part1.cpp
 * @brief Santa's Sleigh Balancing Problem
 *
 * This program helps Santa balance his sleigh by organizing the packages into
 * three groups of equal weight. Santa must ensure the sleigh is perfectly
 * balanced between the passenger compartment and two storage containers on
 * either side.
 *
 * The challenge is to split a list of package weights into three groups where:
 * - Each group has the same total weight.
 * - The first group (passenger compartment) should contain the fewest number of
 *   packages.
 * - If multiple solutions have the same number of packages in the first group,
 *   choose the one with the smallest quantum entanglement (the product of the
 *   package weights in the group).
 *
 * The task is to find the quantum entanglement of the first group in the ideal
 * configuration. The quantum entanglement is used to reduce the chance of any
 * "complications" during the flight.
 *
 * Example Input:
 * Packages: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
 *
 * Example Output:
 * Ideal first group: {11, 9}
 * Quantum entanglement: 99
 *
 * The program processes the input package weights, identifies valid groupings,
 * and calculates the quantum entanglement of the ideal first group that
 * provides the most legroom for Santa.
 *
 * Puzzle Answer: [Quantum Entanglement of Ideal First Group]
 *
 * @author [gabrielzschmitz]
 * @date [13/09/2024]
 */
#include <algorithm>
#include <cstdint>
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

void findCombinations(const std::vector<int32_t>& weights,
                      int32_t current_weight, std::vector<int32_t>& combination,
                      int32_t idx, int32_t target_weight,
                      std::vector<std::vector<int32_t>>& combinations) {
  for (int32_t i = idx; i < weights.size(); i++) {
    if (current_weight + weights[i] < target_weight) {
      combination.push_back(weights[i]);
      findCombinations(weights, current_weight + weights[i], combination, i + 1,
                       target_weight, combinations);
      combination.pop_back();
    } else if (current_weight + weights[i] == target_weight) {
      combination.push_back(weights[i]);
      combinations.push_back(combination);
      combination.pop_back();
    }
  }
}

bool hasOverlap(const std::vector<int32_t>& v1,
                const std::vector<int32_t>& v2) {
  for (const auto& ele_v1 : v1) {
    if (std::find(v2.begin(), v2.end(), ele_v1) != v2.end()) { return true; }
  }
  return false;
}

uint64_t calculateQuantumEntanglement(const std::vector<int32_t>& combination) {
  uint64_t product = 1;
  for (const auto& weight : combination) {
    product *= weight;
  }
  return product;
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
  std::vector<int32_t> weights;
  int32_t total_weight = 0;
  while (std::getline(input_file, line)) {
    int32_t weight = std::stoi(line);
    total_weight += weight;
    weights.push_back(weight);
  }

  if (total_weight % 3 != 0) {
    std::cerr << "Error: Packages cannot be split into three equal groups."
              << std::endl;
    return 1;
  }
  int32_t target_weight = total_weight / 3;

  std::vector<std::vector<int32_t>> valid_combinations;
  {
    std::vector<int32_t> current_combination;
    findCombinations(weights, 0, current_combination, 0, target_weight,
                     valid_combinations);
  }

  std::sort(valid_combinations.begin(), valid_combinations.end(),
            [](const std::vector<int32_t>& c1, const std::vector<int32_t>& c2) {
              if (c1.size() != c2.size()) return c1.size() < c2.size();
              return calculateQuantumEntanglement(c1) <
                     calculateQuantumEntanglement(c2);
            });

  uint64_t total = 0;
  for (int32_t i = 0; i < valid_combinations.size(); ++i) {
    if (total != 0) break;
    for (int32_t j = i + 1; j < valid_combinations.size(); ++j) {
      if (!hasOverlap(valid_combinations[i], valid_combinations[j])) {
        total = calculateQuantumEntanglement(valid_combinations[i]);
        break;
      }
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
