/**
 * @file part1.cpp
 * @brief Identifying the Bot Comparing Specific Microchips
 *
 * This program simulates a network of balance bots that distribute numbered
 * microchips according to a set of instructions. Each bot waits until it holds
 * exactly two microchips, then passes its lower-valued chip and higher-valued
 * chip to other bots or output bins as specified.
 *
 * The input consists of two types of instructions:
 * - Value assignments, which give a specific microchip to a bot.
 * - Distribution rules, which specify where a bot sends its low and high
 *   microchips once it holds two chips.
 *
 * The program processes the instructions in the correct order, simulating
 * every chip transfer until all possible actions have been completed. During
 * the simulation, it identifies the bot responsible for comparing two specific
 * microchip values.
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
 * 2
 *
 * The program simulates all chip movements and outputs the number of the bot
 * that compares the specified pair of microchip values.
 *
 * Puzzle Answer: [Bot Number]
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

  std::uint64_t answer = 0;

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

    if ((debug && low == 2 && high == 5) || (!debug && low == 17 && high == 61))
      answer = bot;

    const Rule& r = ruleIt->second;

    if (r.lowOutput)
      outputs[r.lowTarget].push_back(low);
    else {
      bots[r.lowTarget].push_back(low);
      if (bots[r.lowTarget].size() == 2) ready.push(r.lowTarget);
    }

    if (r.highOutput)
      outputs[r.highTarget].push_back(high);
    else {
      bots[r.highTarget].push_back(high);
      if (bots[r.highTarget].size() == 2) ready.push(r.highTarget);
    }

    chips.clear();

    if (debug)
      std::cout << "Bot " << bot << " compared " << low << " and " << high
                << '\n';
  }

  std::cout << "ANSWER: " << answer << '\n';

  return 0;
}
