/**
 * @file part2.cpp
 * @brief Pulse Propagation - Fewest Button Presses for Module Activation
 *
 * Given a module configuration with different module types (flip-flop,
 * conjunction, broadcaster) and their connections, reset all modules to their
 * default states. Determine the fewest number of button presses required to
 * deliver a single low pulse to the module named rx. Waiting for all pulses to
 * be fully handled after each button press, simulate the pulse propagation
 * sequence and calculate the minimum number of button presses.
 *
 * Example Input:
 * broadcaster -> a, b, c
 * %a -> b
 * %b -> c
 * %c -> inv
 * &inv -> a
 * rx -> ...
 *
 * Example Output:
 * Fewest Button Presses for rx Activation: 42
 *
 * Calculate the fewest number of button presses required to deliver a single
 * low pulse to the module named rx, waiting for all pulses to be fully handled
 * after each button press.
 *
 * @author [gabrielzschmitz]
 * @date [20/12/2023]
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <deque>
#include <cmath>
#include <numeric>
using namespace std;

bool debug = false;

#define ALL(x) (x).begin(), (x).end()
#define ALLc(x) (x).cbegin(), (x).cend()
using u64 = unsigned long long;

u64 gcd(u64 a, u64 b) {
  while (b) {
    u64 temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

u64 lcm(u64 a, u64 b) { return (a / gcd(a, b)) * b; }

enum class ModuleType : char { None, FlipFlop, Conjunction };

struct Module {
  ModuleType type = ModuleType::None;

  bool flipflopOn;
  vector<string> destinations;
  map<string, bool> inputs;
};

struct Pulse {
  string from;
  string to;
  bool high;

  Pulse(const string& from, const string& to, bool high)
      : from(from), to(to), high(high) {}
};

string to_string(u64 value) {
  stringstream ss;
  ss << value;
  return ss.str();
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";
  if (debug) user_input = "test";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }
  string line, rxInput;
  map<string, Module> modules;
  vector<string> conjuctions;
  while (input_file >> line) {
    string name = line, out;
    input_file >> line;
    getline(input_file, line);
    stringstream ss(line);

    Module m;

    while (getline(ss, out, ',')) m.destinations.push_back(out.substr(1));

    if (name[0] == '%') {
      name = name.substr(1);
      m.type = ModuleType::FlipFlop;
      m.flipflopOn = false;
    } else if (name[0] == '&') {
      name = name.substr(1);
      m.type = ModuleType::Conjunction;
      conjuctions.push_back(name);
    }
    if (find(ALLc(m.destinations), "rx") != m.destinations.cend())
      rxInput = name;
    modules[name] = m;
  }

  // Process conjunctions
  sort(ALL(conjuctions));
  for (auto it = modules.begin(); it != modules.end(); ++it) {
    auto& pair = *it;
    auto& name = pair.first;
    auto& m = pair.second;
    for (const string& s : m.destinations) {
      if (binary_search(ALLc(conjuctions), s)) modules[s].inputs[name] = false;
    }
  }

  u64 part2 = 1, lows = 0, highs = 0, pulseCount = 0;
  deque<Pulse> pulseQueue;
  pulseQueue.emplace_back(Pulse{"", "", false});
  map<string, u64> search;
  for (auto& pair : modules[rxInput].inputs) {
    auto& input = pair.first;
    auto& _ = pair.second;
    search[input] = 0;
  }

  auto DoPulse = [&]() -> void {
    pulseQueue.emplace_back("button", "broadcaster",
                            false);  // false low, true high
    while (!pulseQueue.empty()) {
      Pulse& p = pulseQueue.front();
      if (p.high)
        ++highs;
      else
        ++lows;
      auto iter = modules.find(p.to);
      if (iter != modules.cend()) {
        Module& m = iter->second;
        switch (m.type) {
          case ModuleType::None:
            for (auto& d : m.destinations)
              pulseQueue.emplace_back(p.to, d, p.high);
            break;
          case ModuleType::FlipFlop:
            if (!p.high) {
              m.flipflopOn = !m.flipflopOn;
              for (auto& d : m.destinations)
                pulseQueue.emplace_back(p.to, d, m.flipflopOn);
            }
            break;
          default: {
            auto it = m.inputs.find(p.from);
            if (it != m.inputs.cend()) it->second = p.high;

            bool allHigh = true;
            for (auto& pair : m.inputs) {
              auto& _ = pair.first;
              auto& high = pair.second;
              allHigh = allHigh && high;
            }

            if (!allHigh) {
              auto iter = search.find(p.to);
              if (iter != search.cend() && iter->second == 0)
                iter->second = pulseCount;
            }

            for (auto& d : m.destinations)
              pulseQueue.emplace_back(p.to, d, !allHigh);
          }
        }
      }
      pulseQueue.pop_front();
    }
  };

  bool hasZero;
  do {
    ++pulseCount;
    DoPulse();

    hasZero = false;
    for (auto it = search.begin(); it != search.end(); ++it) {
      hasZero = hasZero || !it->second;
    }
  } while (hasZero);

  for (auto it = search.begin(); it != search.end(); ++it) {
    part2 = lcm(part2, it->second);
  }

  cout << "NUMBER: " << part2 << '\n';
}
