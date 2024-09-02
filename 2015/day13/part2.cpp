/**
 * @file part2.cpp
 * @brief Optimal Seating Arrangement with Yourself Included
 *
 * This program builds upon the previous solution that determines the optimal
 * seating arrangement to maximize happiness at a circular dinner table.
 * Initially, it calculates the optimal arrangement without the user included,
 * but now the task is to include yourself in the seating arrangement.
 *
 * The user is assumed to have a neutral impact on the happiness of others,
 * meaning that sitting next to the user neither increases nor decreases
 * anyone's happiness. Similarly, the user's happiness remains unchanged
 * regardless of who they sit next to.
 *
 * The task is to add the user to the guest list and assign a happiness score of
 * 0 to all relationships involving the user. Then, the program recalculates the
 * optimal seating arrangement to include the user and determines the total
 * change in happiness for this arrangement.
 *
 * Example:
 * - If the original optimal arrangement had a total happiness of 330, adding
 *   the user might change the arrangement and result in a different optimal
 *   happiness score.
 *
 * Puzzle Answer: [Total Happiness with User Included]
 *
 * @author [gabrielzschmitz]
 * @date [02/09/2024]
 */
#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

std::vector<std::string> splitString(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter))
    tokens.push_back(token);
  return tokens;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::map<std::pair<std::string, std::string>, int> happiness_map;
  std::set<std::string> attendees;
  std::string line;

  const std::string self = "gabrielzschmitz";

  while (std::getline(input_file, line)) {
    auto tokens = splitString(line, ' ');
    std::string name1 = tokens[0];
    std::string name2 = tokens[10].substr(0, tokens[10].length() - 1);
    int happiness = std::stoi(tokens[3]);
    if (tokens[2] == "lose") happiness = -happiness;
    happiness_map[{name1, name2}] = happiness;
    attendees.insert(name1);

    happiness_map[{name1, self}] = 0;
    happiness_map[{self, name1}] = 0;
  }
  attendees.insert(self);

  std::vector<std::string> attendees_vector(attendees.begin(), attendees.end());
  int max_happiness = INT_MIN;

  do {
    int current_happiness = 0;
    for (size_t i = 0; i < attendees_vector.size(); ++i) {
      std::string left = attendees_vector[i];
      std::string right = attendees_vector[(i + 1) % attendees_vector.size()];
      current_happiness += happiness_map[{left, right}];
      current_happiness += happiness_map[{right, left}];
    }
    if (current_happiness > max_happiness) max_happiness = current_happiness;
  } while (
    std::next_permutation(attendees_vector.begin(), attendees_vector.end()));

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = max_happiness;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
