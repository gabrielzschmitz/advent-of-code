/**
 * @file part1.cpp
 * @brief Optimal Seating Arrangement for Maximum Happiness
 *
 * This program helps in determining the optimal seating arrangement for a group
 * of guests at a circular dinner table. Each guest has a specified change in
 * happiness when seated next to another guest, and the task is to maximize the
 * total happiness for all guests.
 *
 * The input consists of a list of happiness units that represent how much a
 * guest's happiness would increase or decrease when seated next to another
 * specific guest. The table is circular, meaning each guest has exactly two
 * neighbors.
 *
 * The task is to calculate the optimal seating arrangement that yields the
 * maximum possible change in happiness. The program explores all possible
 * seating arrangements and selects the one that maximizes total happiness.
 *
 * Example Input:
 * - Alice would gain 54 happiness units by sitting next to Bob.
 * - Alice would lose 79 happiness units by sitting next to Carol.
 * - Bob would gain 83 happiness units by sitting next to Alice.
 * - Bob would lose 7 happiness units by sitting next to Carol.
 * - Carol would gain 55 happiness units by sitting next to David.
 * - David would gain 46 happiness units by sitting next to Alice.
 *
 * Example Optimal Arrangement:
 * - Alice, Bob, Carol, David
 * - Total Happiness: 330 units
 *
 * The program processes the input to find and output the total change in
 * happiness for the optimal seating arrangement of the actual guest list.
 *
 * Puzzle Answer: [Optimal Total Happiness]
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

  while (std::getline(input_file, line)) {
    auto tokens = splitString(line, ' ');
    std::string name1 = tokens[0];
    std::string name2 = tokens[10].substr(0, tokens[10].length() - 1);
    int happiness = std::stoi(tokens[3]);
    if (tokens[2] == "lose") happiness = -happiness;
    happiness_map[{name1, name2}] = happiness;
    attendees.insert(name1);
  }

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
