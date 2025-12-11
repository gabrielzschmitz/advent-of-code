/**
 * @file part2.cpp
 * @brief Machine Joltage Configuration Optimizer
 *
 * This program determines the minimum number of button presses required to
 * configure a set of machines to their correct target joltage levels.
 *
 * Each machine contains a series of joltage counters, all initialized to zero.
 * A machine’s joltage requirements are represented as a list of integers,
 * where each integer specifies the final required value for the corresponding
 * counter index.
 *
 * Each button on a machine affects one or more counters. When a button is
 * pressed, it increments all counters listed in that button’s wiring schematic
 * by one. Buttons may be pressed any number of times.
 *
 * The objective is to compute the minimum total number of button presses needed
 * so that all counters match the required joltage configuration for every
 * machine. This is equivalent to solving a system of linear equations over the
 * natural numbers, minimizing the sum of all button usages.
 *
 * Example:
 * - A machine with requirements {3,5,4,7}
 *   and buttons (3), (1,3), (2), (2,3), (0,2), (0,1)
 *   can be configured in a minimum of 10 presses.
 *
 * Additional examples show machines requiring 12 and 11 minimum presses,
 * respectively. Summing the minimum presses across all machines yields the
 * total required button usage.
 *
 * The program parses each machine’s button schematics and joltage
 * requirements, computes the minimum valid combination of button presses using
 * an appropriate optimization method, and outputs the cumulative number of
 * presses across all machines.
 *
 * Puzzle Answer: [Total Minimum Button Presses]
 *
 * @author [gabrielzschmitz]
 * @date [10/12/2025]
 */
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test")
    debug = true;
}

static std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> ans;
  std::string cur;
  for (char c : s) {
    if (c == delim) {
      ans.push_back(cur);
      cur.clear();
    } else
      cur += c;
  }
  ans.push_back(cur);
  return ans;
}

static int parse_config(const std::string &s) {
  assert(!s.empty() && s.front() == '[' && s.back() == ']');
  return static_cast<int>(s.size()) - 2;
}

static int parse_toggle(const std::string &s) {
  assert(!s.empty() && s.front() == '(' && s.back() == ')');

  auto inner = s.substr(1, s.size() - 2);
  auto parts = split(inner, ',');

  int val = 0;
  for (auto &p : parts)
    val += (1 << std::stoi(p));
  return val;
}

static std::vector<int> parse_joltage(const std::string &s) {
  assert(!s.empty() && s.front() == '{' && s.back() == '}');

  auto inner = s.substr(1, s.size() - 2);
  auto parts = split(inner, ',');

  std::vector<int> ans;
  ans.reserve(parts.size());
  for (auto &p : parts)
    ans.push_back(std::stoi(p));

  return ans;
}

static int N = 0;
static int answer = 0;
static std::vector<int> contain;
static std::vector<int> need;
static std::vector<int> toggles_global;

static void rec(int mask, int taken) {
  if (mask == (1 << N) - 1) {
    for (int p : need)
      assert(p == 0);
    answer = std::min(answer, taken);
    return;
  }

  int best_need = *std::max_element(need.begin(), need.end());
  if (best_need + taken > answer)
    return;

  int best_id = -1;
  int best_cnt = std::numeric_limits<int>::max();

  for (int i = 0; i < N; i++) {
    if (mask & (1 << i))
      continue;

    int cnt = std::numeric_limits<int>::max();
    for (int j = 0; j < (int)need.size(); j++)
      if (toggles_global[i] & (1 << j))
        cnt = std::min(cnt, contain[j]);

    if (cnt < best_cnt) {
      best_cnt = cnt;
      best_id = i;
    }
  }

  std::vector<int> me;
  for (int i = 0; i < (int)need.size(); i++)
    if (toggles_global[best_id] & (1 << i))
      me.push_back(i);

  if (best_cnt == 1) {
    int must_value = -1;

    for (int p : me) {
      if (contain[p] == 1) {
        if (must_value == -1)
          must_value = need[p];
        else if (must_value != need[p])
          return;
      }
    }
    assert(must_value != -1);

    for (int p : me)
      if (must_value > need[p])
        return;

    for (int p : me) {
      need[p] -= must_value;
      contain[p] -= 1;
    }

    rec(mask | (1 << best_id), taken + must_value);

    for (int p : me) {
      need[p] += must_value;
      contain[p] += 1;
    }

    return;
  }

  int mx = std::numeric_limits<int>::max();
  for (int p : me)
    mx = std::min(mx, need[p]);

  for (int p : me)
    contain[p] -= 1;

  for (int p : me)
    need[p] -= mx;

  rec(mask | (1 << best_id), taken + mx);

  for (int take = 1; take <= mx; take++) {
    for (int p : me)
      need[p] += 1;
    rec(mask | (1 << best_id), taken + mx - take);
  }

  for (int p : me)
    contain[p] += 1;
}

static int solve(int n, const std::vector<int> &toggles,
                 const std::vector<int> &goal) {
  N = (int)toggles.size();

  need = goal;
  contain.assign(goal.size(), 0);

  for (int tg : toggles)
    for (int i = 0; i < (int)goal.size(); i++)
      if (tg & (1 << i))
        contain[i] += 1;

  toggles_global = toggles;
  answer = std::numeric_limits<int>::max();

  rec(0, 0);
  return answer;
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  int64_t total = 0;
  std::string line;

  while (getline(input_file, line)) {
    if (line.empty())
      continue;

    auto parts = split(line, ' ');
    int config_n = parse_config(parts[0]);

    std::vector<int> toggles;
    for (size_t i = 1; i + 1 < parts.size(); i++)
      toggles.push_back(parse_toggle(parts[i]));

    auto goal = parse_joltage(parts.back());
    total += solve(config_n, toggles, goal);

    if (debug)
      std::cerr << "Processed line, running total = " << total << std::endl;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
