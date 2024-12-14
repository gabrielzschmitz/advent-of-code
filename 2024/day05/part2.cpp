/**
 * @file part2.cpp
 * @brief Correcting and Verifying Print Queue Order
 *
 * This program builds on the solution from Part 1, where updates to the sleigh 
 * launch safety manual were validated against a set of page ordering rules. In 
 * this part, the focus shifts to fixing the updates that are not in the correct 
 * order and recalculating their middle page numbers.
 *
 * The input remains the same:
 * 1. A set of ordering rules in the format X|Y, meaning page X must precede 
 *    page Y if both appear in the update.
 * 2. A series of updates, each listing a sequence of page numbers.
 *
 * Tasks:
 * - Identify updates that are not in the correct order.
 * - Reorder these updates to conform to the rules.
 * - Calculate the middle page number for each corrected update.
 * - Sum the middle page numbers from the corrected updates to produce the final
 *   result.
 *
 * Example Input:
 * Ordering Rules:
 * 47|53
 * 97|13
 * 75|29
 * ...
 *
 * Updates:
 * 75,97,47,61,53 -> Incorrect
 * 61,13,29 -> Incorrect
 * 97,13,75,29,47 -> Incorrect
 * ...
 *
 * Example Output:
 * Corrected Updates:
 * 97,75,47,61,53 -> Middle: 47
 * 61,29,13 -> Middle: 29
 * 97,75,47,29,13 -> Middle: 47
 * Sum of middle page numbers: 123
 *
 * Puzzle Answer: [Sum of Middle Page Numbers from Corrected Updates]
 *
 * @author [gabrielzschmitz]
 * @date [13/12/2024]
 */
#include <algorithm>
#include <fstream>
#include <functional>
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

std::vector<int> reorderUpdate(
  const std::vector<int>& update,
  const std::unordered_map<int, std::unordered_set<int>>& rules) {
  std::unordered_map<int, std::vector<int>> graph;
  std::unordered_set<int> in_update(update.begin(), update.end());
  std::unordered_set<int> visited, temp_mark;
  std::vector<int> sorted;

  for (const auto& [x, y_set] : rules)
    if (in_update.count(x))
      for (int y : y_set)
        if (in_update.count(y)) { graph[x].push_back(y); }

  std::function<bool(int)> visit = [&](int node) {
    if (temp_mark.count(node)) return false;
    if (!visited.count(node)) {
      temp_mark.insert(node);
      for (int neighbor : graph[node])
        if (!visit(neighbor)) return false;
      temp_mark.erase(node);
      visited.insert(node);
      sorted.push_back(node);
    }
    return true;
  };

  for (int page : update) {
    if (!visited.count(page) && !visit(page)) {
      std::cerr << "Cycle detected in the rules!" << std::endl;
      return update;
    }
  }

  std::vector<int> remaining(update);
  for (auto it = sorted.begin(); it != sorted.end(); /* no increment here */) {
    int current_value = *it;

    auto remaining_it =
      std::find(remaining.begin(), remaining.end(), current_value);

    if (remaining_it != remaining.end()) {
      remaining.erase(remaining_it);
      ++it;
    } else it = sorted.erase(it);
  }

  return sorted;
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

  std::vector<std::pair<std::vector<int>, int>> corrected_updates;

  while (std::getline(input_file, line) && !line.empty()) {
    auto parts = split(line, ',');
    std::vector<int> update;
    for (const auto& part : parts)
      update.push_back(std::stoi(part));

    if (!isValidUpdate(update, rules)) {
      auto corrected = reorderUpdate(update, rules);
      int middle_index = corrected.size() / 2;
      int middle_page = corrected[middle_index];
      corrected_updates.push_back({corrected, middle_page});
      total += middle_page;
    }
  }

  if (debug) {
    std::cout << "Corrected updates:" << std::endl;
    for (const auto& [update, middle_page] : corrected_updates) {
      for (size_t i = 0; i < update.size(); ++i) {
        std::cout << update[i];
        if (i < update.size() - 1) std::cout << ",";
      }
      std::cout << " -> Middle: " << middle_page << std::endl;
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
