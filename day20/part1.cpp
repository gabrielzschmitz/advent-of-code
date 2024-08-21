/**
 * @file part1.cpp
 * @brief Pulse Propagation - Number of Pulses Calculation
 *
 * Given a module configuration with different module types (flip-flop,
 * conjunction, broadcaster) and their connections, simulate the pulse
 * propagation sequence after pushing the button 1000 times. Determine the
 * number of low pulses and high pulses sent after each push of the button and
 * calculate the total by multiplying the respective counts. The pulses are
 * processed in the order they are sent, and modules have specific behaviors
 * based on their types.
 *
 * Example Input:
 * broadcaster -> a, b, c
 * %a -> b
 * %b -> c
 * %c -> inv
 * &inv -> a
 *
 * Example Output:
 * Total Low Pulses * Total High Pulses: 32000000
 *
 * Calculate the total number of low pulses and high pulses sent after pushing
 * the button 1000 times, waiting for all pulses to be fully handled after each
 * push.
 *
 * @author [gabrielzschmitz]
 * @date [20/12/2023]
 */
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

bool debug = false;

enum class EModuleType : int { broadcaster, flipflop, conjunction, dummy };

long low_pulses = 0;
long high_pulses = 0;

struct Module;

queue<tuple<Module*, Module*, bool>> q;

struct Module {
  string name = "";
  EModuleType module_type = EModuleType::broadcaster;

  vector<pair<Module*, bool>> inputs;
  vector<Module*> outputs;

  bool bTurnedOn = false;

  int num_highs = 0;

  Module(string _n, EModuleType type) : name(_n), module_type(type) {}

  void update_num_highs() {
    num_highs = 0;
    for (auto& p : inputs) {
      num_highs += p.second ? 1 : 0;
    }
  }

  void append_state(stringstream& ss) {
    if (module_type == EModuleType::broadcaster) {
      ss << "broadcaster";
    } else if (module_type == EModuleType::flipflop) {
      ss << name << (bTurnedOn ? "[ON]" : "[OFF]") << endl;
    } else {
      ss << name;
      for (auto& p : inputs) {
        ss << "_input{" << p.first->name << "|" << (p.second ? "HIGH" : "LOW")
           << "}" << endl;
      }
    }
  }

  void send_pulse(bool bHigh) {
    for (Module* m : outputs) {
      if (bHigh) {
        high_pulses++;
      } else {
        low_pulses++;
      }
      if (debug)
        cout << name << " -" << (bHigh ? "high" : "low") << "-> " << m->name
             << endl;
      q.push({this, m, bHigh});
    }
  }

  void print_last_input_state() {
    for (auto& p : inputs) {
      cout << (p.second ? "ON" : "OFF") << " ";
    }
    cout << endl;
  }

  void receive_pulse(bool bHigh, const Module* from) {
    if (module_type == EModuleType::flipflop) {
      if (bHigh) { return; }

      bTurnedOn = !bTurnedOn;
      send_pulse(bTurnedOn);
    } else {
      for (auto& p : inputs) {
        if (from->name == p.first->name) {
          p.second = bHigh;

          break;
        }
      }

      if (debug) print_last_input_state();

      update_num_highs();
      if (num_highs == inputs.size()) {
        send_pulse(false);
      } else {
        send_pulse(true);
      }
    }
  }
};

void strip_spaces(string& s) {
  stringstream ss(s);
  string ln;
  stringstream outss;
  while (ss >> ln) {
    outss << ln;
  }
  s = outss.str();
}

vector<Module*> module_order;
map<string, pair<long, long>> states;

string make_key() {
  stringstream ss;
  for (Module* m : module_order) {
    m->append_state(ss);
    ss << "||";
  }

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

  string ln;

  vector<string> modules_to_create;
  vector<string> outputs_to_create;

  while (getline(input_file, ln)) {
    size_t arrow_before = ln.find("->") - 1;
    size_t arrow_after = ln.find("->") + 3;

    string new_module = ln.substr(0, arrow_before);
    string new_outputs = ln.substr(arrow_after);
    strip_spaces(new_outputs);

    modules_to_create.push_back(new_module);
    outputs_to_create.push_back(new_outputs);
  }

  unordered_map<string, Module*> modules;

  for (string& m : modules_to_create) {
    if (m[0] != '%' && m[0] != '&') {
      Module* bcast = new Module("broadcaster", EModuleType::broadcaster);
      modules["broadcaster"] = bcast;
      module_order.push_back(bcast);
      continue;
    }

    EModuleType type =
      (m[0] == '%') ? EModuleType::flipflop : EModuleType::conjunction;
    Module* new_entry = new Module(m.substr(1), type);
    modules[new_entry->name] = new_entry;
    module_order.push_back(new_entry);
  }
  int n = modules.size();
  for (int i = 0; i < n; i++) {
    Module* m = module_order[i];
    stringstream ss(outputs_to_create[i]);
    while (getline(ss, ln, ',')) {
      if (modules.find(ln) == modules.end()) {
        modules[ln] = new Module(ln, EModuleType::dummy);
      }

      m->outputs.push_back(modules[ln]);
      modules[ln]->inputs.push_back(make_pair(m, false));
    }
  }

  for (int i = 0; i < 1000; i++) {
    string key = make_key();

    if (debug) cout << "button -low-> broadcaster" << endl;
    modules["broadcaster"]->send_pulse(false);
    low_pulses++;

    while (!q.empty()) {
      Module* sender = get<0>(q.front());
      Module* receiver = get<1>(q.front());
      bool bHigh = get<2>(q.front());

      q.pop();
      receiver->receive_pulse(bHigh, sender);
    }
    if (debug) cout << endl;

    states[key] = make_pair(low_pulses, high_pulses);
  }

  int total = low_pulses * high_pulses;
  cout << "NUMBER: " << total << endl;
}
