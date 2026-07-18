/**
 * @file part2.cpp
 * @brief Computing the Product of Microchips in Output Bins
 *
 * This program simulates the balance bot network described by the instruction
 * set. Bots receive numbered microchips and, once holding exactly two chips,
 * distribute their lower-valued and higher-valued chips to other bots or
 * output bins according to their assigned rules.
 *
 * The input consists of:
 * - Value assignments, which place a specific microchip into a bot.
 * - Distribution rules, which determine where each bot sends its low and high
 *   microchips.
 *
 * After processing all instructions and completing every possible chip
 * transfer, the program examines output bins 0, 1, and 2. It retrieves the
 * microchip stored in each of these bins and computes the product of their
 * values.
 *
 * Example Input:
 * value 5 goes to bot 2
 * bot 2 gives low to bot 1 and high to bot 0
 * value 3 goes to bot 1
 * bot 1 gives low to output 1 and high to bot 0
 * bot 0 gives low to output 2 and high to output 0
 * value 2 goes to bot 2
 *
 * Example Output:
 * Output 0: 5
 * Output 1: 2
 * Output 2: 3
 * Product: 30
 *
 * The program simulates all chip movements and outputs the product of the
 * values stored in output bins 0, 1, and 2.
 *
 * Puzzle Answer: [Product of Outputs 0, 1, and 2]
 *
 * @author [gabrielzschmitz]
 * @date [18/07/2026]
 */
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
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

struct Rule {
  bool lowOutput = false;
  int lowTarget = -1;
  bool highOutput = false;
  int highTarget = -1;
};

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::map<int, std::vector<int>> bots;
  std::map<int, Rule> rules;
  std::map<int, std::vector<int>> outputs;
  std::queue<int> ready;

  std::string line;
  while (std::getline(input_file, line)) {
    std::stringstream ss(line);

    if (line.find("value") == 0) {
      std::string tmp;
      int value, bot;

      ss >> tmp >> value >> tmp >> tmp >> tmp >> bot;
      bots[bot].push_back(value);

    } else {
      int bot;
      char lowType[16], highType[16];

      Rule r;

      sscanf(line.c_str(), "bot %d gives low to %15s %d and high to %15s %d",
             &bot, lowType, &r.lowTarget, highType, &r.highTarget);

      r.lowOutput = (std::string(lowType) == "output");
      r.highOutput = (std::string(highType) == "output");

      rules[bot] = r;
    }
  }

  // Initialize bots ready to act.
  for (const auto& [bot, chips] : bots)
    if (chips.size() == 2) ready.push(bot);

  while (!ready.empty()) {
    int bot = ready.front();
    ready.pop();

    if (bots[bot].size() != 2) continue;

    auto ruleIt = rules.find(bot);
    if (ruleIt == rules.end()) continue;

    auto& chips = bots[bot];
    std::sort(chips.begin(), chips.end());

    int low = chips[0];
    int high = chips[1];

    const Rule& r = ruleIt->second;

    if (r.lowOutput) {
      outputs[r.lowTarget].push_back(low);
    } else {
      bots[r.lowTarget].push_back(low);
      if (bots[r.lowTarget].size() == 2) ready.push(r.lowTarget);
    }

    if (r.highOutput) {
      outputs[r.highTarget].push_back(high);
    } else {
      bots[r.highTarget].push_back(high);
      if (bots[r.highTarget].size() == 2) ready.push(r.highTarget);
    }

    chips.clear();

    if (debug)
      std::cout << "Bot " << bot << " compared " << low << " and " << high
                << '\n';
  }

  std::uint64_t answer = 1;

  for (int i = 0; i < 3; ++i) {
    if (outputs[i].empty()) {
      std::cerr << "Output " << i << " is empty!\n";
      return 1;
    }

    answer *= outputs[i][0];

    if (debug) std::cout << "Output " << i << ": " << outputs[i][0] << '\n';
  }

  std::cout << "ANSWER: " << answer << '\n';

  return 0;
}
