/**
 * @file part1.cpp
 * @brief Boolean Logic Gate Simulator
 *
 * This program simulates a system of boolean logic gates and wires, as
 * described in the problem. The task involves determining the final output of a
 * system of interconnected logic gates using initial wire values and gate
 * definitions.
 *
 * **Challenge Description:**
 * - Each gate operates on two inputs and produces one output based on the type
 *   of gate:
 *   - AND: Outputs 1 if both inputs are 1; otherwise, outputs 0.
 *   - OR: Outputs 1 if at least one input is 1; otherwise, outputs 0.
 *   - XOR: Outputs 1 if inputs are different; otherwise, outputs 0.
 * - Wires can carry values of 1 (true), 0 (false), or no value initially.
 * - The system operates in sequence, with gates producing outputs once all
 *   inputs are received.
 * - The outputs from gates can serve as inputs to other gates.
 *
 * The objective is to compute the decimal number represented by the outputs of
 * all wires starting with the prefix `z`. These outputs, interpreted as binary
 * digits, represent the final result.
 *
 * **Input Format:**
 * 1. Initial wire values: `wire_name: value`
 * 2. Gate definitions: `input1 GATE_TYPE input2 -> output`
 *
 * **Example Input:**
 * ```
 * x00: 1
 * x01: 0
 * y00: 1
 * y01: 1
 * x00 AND y00 -> z00
 * x01 XOR y01 -> z01
 * ```
 *
 * **Example Process:**
 * - Initialize wire values.
 * - Simulate each gate sequentially, computing outputs based on the gate logic.
 * - Combine the outputs of all wires starting with `z` as binary digits.
 *
 * **Example Output:**
 * If wires `z00`, `z01`, and `z02` have values `0`, `1`, and `1`, respectively,
 * the binary number is `011`, which equals 3 in decimal.
 *
 * **Puzzle Answer:** [Decimal Output]
 *
 * @author [gabrielzschmitz]
 * @date [24/12/2024]
 */
#include <bitset>
#include <cstdint>
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

int computeGateOutput(const std::string& gate, int input1, int input2) {
  if (gate == "AND") return input1 & input2;
  if (gate == "OR") return input1 | input2;
  if (gate == "XOR") return input1 ^ input2;
  throw std::invalid_argument("Invalid gate type: " + gate);
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  uint64_t total = 0;

  std::unordered_map<std::string, int> wire_values;
  std::vector<std::tuple<std::string, std::string, std::string, std::string>>
    gates;
  std::string line;

  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    std::string token;

    if (line.find(":") != std::string::npos) {
      std::string wire;
      int value;
      if (std::getline(iss, wire, ':') && iss >> value) {
        wire_values[wire] = value;
        if (debug)
          std::cout << "Initialized " << wire << " = " << value << std::endl;
      }
    } else if (line.find("->") != std::string::npos) {
      std::string input1, gate, input2, arrow, output;
      if (iss >> input1 >> gate >> input2 >> arrow >> output && arrow == "->") {
        gates.emplace_back(input1, gate, input2, output);
        if (debug)
          std::cout << "Gate: " << input1 << " " << gate << " " << input2
                    << " -> " << output << std::endl;
      }
    }
  }

  bool updated = true;
  while (updated) {
    updated = false;
    for (auto& [input1, gate, input2, output] : gates) {
      if (wire_values.find(input1) != wire_values.end() &&
          wire_values.find(input2) != wire_values.end() &&
          wire_values.find(output) == wire_values.end()) {

        int result =
          computeGateOutput(gate, wire_values[input1], wire_values[input2]);
        wire_values[output] = result;
        updated = true;
        if (debug)
          std::cout << "Computed " << output << " = " << result << std::endl;
      }
    }
  }

  int max_suffix = -1;
  for (const auto& [wire, value] : wire_values) {
    if (wire.rfind("z", 0) == 0) {
      try {
        int suffix = std::stoi(wire.substr(1));
        max_suffix = std::max(max_suffix, suffix);
      } catch (const std::invalid_argument&) {
        std::cerr << "Invalid wire name: " << wire << std::endl;
        return 1;
      }
    }
  }

  if (max_suffix < 0) {
    std::cerr << "No `z`-prefixed wires found!" << std::endl;
    return 1;
  }

  std::string binary_result(max_suffix + 1, '0');
  for (const auto& [wire, value] : wire_values) {
    if (wire.rfind("z", 0) == 0) {
      int suffix = std::stoi(wire.substr(1));
      binary_result[max_suffix - suffix] = value ? '1' : '0';
    }
  }

  if (!binary_result.empty())
    total = std::bitset<64>(binary_result).to_ullong();

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
