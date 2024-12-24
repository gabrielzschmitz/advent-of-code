/**
 * @file part2.cpp
 * @brief Correcting Swapped Gate Outputs in a Binary Addition System
 *
 * This program analyzes and corrects a malfunctioning binary addition system. 
 * The system involves wires and gates to compute the sum of two binary numbers, 
 * where the binary numbers are represented by wires starting with `x` and `y`. 
 * The result of their addition is produced on wires starting with `z`.
 *
 * **Challenge Description:**
 * - The system is malfunctioning due to four pairs of gates whose output wires 
 *   have been swapped. These swaps disrupt the correct addition logic.
 * - The task is to identify the eight wires involved in these swaps, sort their 
 *   names lexicographically, and output them as a comma-separated list.
 *
 * **Steps to Solve:**
 * 1. Simulate the binary addition system using the given gates and initial wire
 * values.
 * 2. Analyze the output of the system to detect mismatches with expected
 * results (i.e., incorrect binary addition).
 * 3. Identify the swapped pairs of gates by tracing incorrect outputs.
 * 4. Correct the swaps and verify the system's functionality for any input.
 * 5. Output the names of the eight wires involved in the swaps, sorted and
 * joined by commas.
 *
 * **Example Input:**
 * ```
 * x00: 0
 * x01: 1
 * x02: 0
 * x03: 1
 * x04: 0
 * x05: 1
 * y00: 0
 * y01: 0
 * y02: 1
 * y03: 1
 * y04: 0
 * y05: 1
 * x00 AND y00 -> z05
 * x01 AND y01 -> z02
 * x02 AND y02 -> z01
 * x03 AND y03 -> z03
 * x04 AND y04 -> z04
 * x05 AND y05 -> z00
 * ```
 *
 * **Example Process:**
 * - Simulate the gates using the input values.
 * - Detect swapped outputs: `z05` and `z00`, `z02` and `z01`.
 * - Correct the swaps to restore correct addition logic.
 * - Identify involved wires: `z00,z01,z02,z05`.
 *
 * **Example Output:**
 * `z00,z01,z02,z05`
 *
 * **Puzzle Answer:** [Sorted List of Wires Involved in Swaps]
 *
 * @author [gabrielzschmitz]
 * @date [24/12/2024]
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

struct Gate {
  std::string op;
  std::pair<std::string, std::string> inputs;
};

using Wires = std::unordered_map<std::string, bool>;
using Gates = std::unordered_map<std::string, Gate>;
using Outputs = std::vector<std::string>;

std::string getOutput(const std::string& in1, const std::string& in2,
                      const std::string& op, const Gates& gates) {
  for (const auto& [out, gate] : gates) {
    if (gate.op != op) continue;

    std::string in3{gate.inputs.first};
    std::string in4{gate.inputs.second};

    if ((in1 == in3 && in2 == in4) || (in1 == in4 && in2 == in3)) {
      return out;
    }
  }
  return "";
}

void swapGates(const std::string in1, const std::string in2, Gates& gates,
               Outputs& as, Outputs& bs, Outputs& cs, Outputs& ds) {
  Gate g1{gates[in1]};
  Gate g2{gates[in2]};

  gates[in1] = g2;
  gates[in2] = g1;

  for (int i = 0; i < as.size(); ++i) {
    if (in1 == as[i]) {
      as[i] = in2;
    } else if (in2 == as[i]) {
      as[i] = in1;
    }

    if (in1 == bs[i]) {
      bs[i] = in2;
    } else if (in2 == bs[i]) {
      bs[i] = in1;
    }

    if (in1 == cs[i]) {
      cs[i] = in2;
    } else if (in2 == cs[i]) {
      cs[i] = in1;
    }

    if (in1 == ds[i]) {
      ds[i] = in2;
    } else if (in2 == ds[i]) {
      ds[i] = in1;
    }
  }
}

Outputs getExpectedOutputs(Gates& gates, Outputs& as, Outputs& bs, Outputs& cs,
                           Outputs& ds) {
  Outputs wrong;

  // Check first gate
  Gate gate{gates.at("z00")};
  std::string in1{gate.inputs.first};
  std::string in2{gate.inputs.second};
  bool wrongInputs{
    !((in1 == "x00" && in2 == "y00") || (in1 == "y00" && in2 == "x00"))};
  if (gate.op != "XOR" || wrongInputs) wrong.push_back("z00");

  cs.resize(as.size());
  ds.resize(as.size());
  cs[1] = bs[0];
  int i{0};

  std::ostringstream oss;
  while (i < as.size() - 1) {
    oss.str("");
    oss.clear();
    oss.width(2);
    oss.fill('0');
    oss << ++i;

    gate = gates.at("z" + oss.str());
    std::string in1{gate.inputs.first};
    std::string in2{gate.inputs.second};

    if (gate.op != "XOR") {
      wrong.push_back("z" + oss.str());
      oss.str("");
      oss.clear();
      oss.width(2);
      oss.fill('0');
      oss << i;

      ds[i] = getOutput(as[i - 1], cs[i - 1], "AND", gates);
      cs[i] = getOutput(bs[i - 1], ds[i], "OR", gates);

      std::string labelSwap{getOutput(as[i], cs[i], "XOR", gates)};
      swapGates(labelSwap, "z" + oss.str(), gates, as, bs, cs, ds);

      wrong.push_back(labelSwap);
    } else if (in1 != as[i] && in2 != as[i]) {
      wrong.push_back(as[i]);

      Gate g1{gates.at(in1)};
      Gate g2{gates.at(in2)};
      if (g1.inputs.first == bs[i - 1] || g1.inputs.second == bs[i - 1]) {
        swapGates(as[i], in2, gates, as, bs, cs, ds);
      } else if (g2.inputs.first == bs[i - 1] ||
                 g2.inputs.second == bs[i - 1]) {
        swapGates(as[i], in1, gates, as, bs, cs, ds);
      }

      wrong.push_back(as[i]);
    }

    if (i > 1) {
      ds[i] = getOutput(as[i - 1], cs[i - 1], "AND", gates);
      cs[i] = getOutput(bs[i - 1], ds[i], "OR", gates);
    }
  }

  return wrong;
}

void getGates(std::ifstream& inf, Gates& gates, Outputs& as, Outputs& bs) {
  std::string in1, in2, op, out, line;
  while (inf >> in1 >> op >> in2 >> line >> out) {
    gates[out] = {op, {in1, in2}};
    if ((in1[0] == 'x' && in2[0] == 'y') || (in1[0] == 'y' && in2[0] == 'x')) {
      int n1{std::stoi(in1.substr(1))};
      int n2{std::stoi(in2.substr(1))};

      if (n1 != n2) { continue; }

      if (n1 + 1 > as.size()) {
        as.resize(n1 + 1);
        bs.resize(n1 + 1);
      }

      if (op == "XOR") {
        as[n1] = out;
      } else if (op == "AND") {
        bs[n1] = out;
      }
    }
  }
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
  Wires wires;
  while (std::getline(input_file, line), line != "") {
    size_t sep{line.find(':')};
    std::string name{line.substr(0, sep)};
    wires[name] = std::stoi(line.substr(sep + 2));
  }

  Outputs as, bs, cs, ds, wrong;
  Gates gates;
  getGates(input_file, gates, as, bs);
  wrong = getExpectedOutputs(gates, as, bs, cs, ds);

  std::sort(wrong.begin(), wrong.end());

  std::cout << "ANSWER: ";
  for (const auto& s : wrong) {
    std::cout << s << ",";
  }
  std::cout << "\n";

  return 0;
}
