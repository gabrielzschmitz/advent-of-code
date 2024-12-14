/**
 * @file part1.cpp
 * @brief Verifying Print Queue Order
 *
 * This program assists the North Pole printing department by validating the 
 * order of pages in updates to the sleigh launch safety manual. Each update 
 * specifies a list of pages to be printed, and a set of ordering rules dictates 
 * which pages must be printed before others.
 *
 * The input consists of two sections:
 * 1. A set of ordering rules in the format X|Y, meaning that page X must 
 *    be printed before page Y if both are included in the update.
 * 2. A series of updates, each listing a sequence of page numbers that 
 *    must be checked against the ordering rules.
 *
 * The program determines:
 * - Which updates are in the correct order according to the rules.
 * - The middle page number of each correctly-ordered update.
 *
 * For correctly-ordered updates, the middle page numbers are identified, and 
 * their sum is calculated to produce the final result.
 *
 * Example Input:
 * Ordering Rules:
 * 47|53
 * 97|13
 * 75|29
 * ...
 * 
 * Updates:
 * 75,47,61,53,29
 * 97,61,53,29,13
 * ...
 *
 * Example Output:
 * Correctly-ordered updates: 
 * 75,47,61,53,29 -> Middle: 61
 * 97,61,53,29,13 -> Middle: 53
 * ...
 * Sum of middle page numbers: 143
 *
 * Puzzle Answer: [Sum of Middle Page Numbers]
 *
 * @author [gabrielzschmitz]
 * @date [13/12/2024]
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

std::vector<std::string> split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::istringstream tokenStream(s);
  std::string token;

  while (std::getline(tokenStream, token, delimiter))
    tokens.push_back(token);

  return tokens;
}

bool isValidUpdate(
  const std::vector<int>& update,
  const std::unordered_map<int, std::unordered_set<int>>& rules) {
  std::unordered_map<int, int> position;
  for (size_t i = 0; i < update.size(); ++i)
    position[update[i]] = i;

  for (const auto& [x, y_set] : rules)
    for (int y : y_set)
      if (position.count(x) && position.count(y) && position[x] > position[y])
        return false;

  return true;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::unordered_map<int, std::unordered_set<int>> rules;
  std::string line;

  while (std::getline(input_file, line) && !line.empty()) {
    auto parts = split(line, '|');
    if (parts.size() == 2) {
      int x = std::stoi(parts[0]);
      int y = std::stoi(parts[1]);
      rules[x].insert(y);
    }
  }

  int total = 0;

  std::vector<std::pair<std::vector<int>, int>> valid_updates;

  while (std::getline(input_file, line) && !line.empty()) {
    auto parts = split(line, ',');
    std::vector<int> update;
    for (const auto& part : parts)
      update.push_back(std::stoi(part));

    if (isValidUpdate(update, rules)) {
      int middle_index = update.size() / 2;
      int middle_page = update[middle_index];
      valid_updates.push_back({update, middle_page});
      total += middle_page;
    }
  }

  if (debug) std::cout << "Correctly-ordered updates:" << std::endl;
  for (const auto& [update, middle_page] : valid_updates) {
    for (size_t i = 0; i < update.size(); ++i) {
      if (debug) std::cout << update[i];
      if (i < update.size() - 1 && debug) std::cout << ",";
    }
    if (debug) std::cout << " -> Middle: " << middle_page << std::endl;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
