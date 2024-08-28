/**
 * @file part2.cpp
 * @brief Santa's Circuit Assembly - Signal Override
 *
 * This program helps Santa assemble a complex circuit using a series of wires
 * and bitwise logic gates. Each wire in the circuit has an identifier (some
 * lowercase letters) and can carry a 16-bit signal (a number from 0 to 65535).
 * The circuit is configured based on a series of instructions that describe how
 * signals should be provided to the wires.
 *
 * Instructions can involve:
 * - Directly setting a signal on a wire (e.g., "123 -> x" sets wire `x` to
 *   123).
 * - Performing bitwise operations like AND, OR, LSHIFT, RSHIFT, and NOT on one
 *   or more wires, and then setting the result on another wire (e.g., "x AND y
 *   -> d" sets wire `d` to the result of `x` AND `y`).
 *
 * The task is to: 1. Calculate the signal on wire `a` by processing the circuit
 * instructions. 2. Once the signal on wire `a` is determined, override wire `b`
 * with that signal. 3. Reset all other wires (including wire `a`) and reprocess
 * the instructions to determine the new signal on wire `a`.
 *
 * Example Instructions:
 * - "123 -> x" means that the signal 123 is provided to wire x.
 * - "x AND y -> d" means that the bitwise AND of wire x and wire y is provided
 *   to wire d.
 * - "p LSHIFT 2 -> q" means that the value from wire p is left-shifted by 2 and
 *   then provided to wire q.
 * - "NOT e -> f" means that the bitwise complement of the value from wire e is
 *   provided to wire f.
 *
 * Example Circuit:
 * 123 -> x
 * 456 -> y
 * x AND y -> d
 * x OR y -> e
 * x LSHIFT 2 -> f
 * y RSHIFT 2 -> g
 * NOT x -> h
 * NOT y -> i
 *
 * After processing the instructions, the final signal on wire `a` is
 * determined. Then, wire `b` is overridden with this signal, and the circuit is
 * recalculated to find the new signal on wire `a`.
 *
 * Puzzle Answer: [New Signal on Wire a]
 *
 * @author [gabrielzschmitz]
 * @date [27/08/2024]
 */
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

enum OPS { OP_ERR, OP_AND, OP_OR, OP_NOT, OP_LSHIFT, OP_RSHIFT, OP_STORE };

bool isNumber(const std::string& n) {
  return std::all_of(n.begin(), n.end(), ::isdigit);
}

OPS stringToOp(const std::string& s) {
  if (s == "AND") return OP_AND;
  if (s == "OR") return OP_OR;
  if (s == "LSHIFT") return OP_LSHIFT;
  if (s == "RSHIFT") return OP_RSHIFT;
  return OP_ERR;
}

struct Gate {
  OPS op;
  std::string lhs;
  std::string rhs;
  bool memoized;
  uint16_t val;

  Gate() : op(OP_ERR), lhs(""), rhs(""), memoized(false), val(0) {}
};

uint16_t evaluateOutput(const std::map<std::string, Gate>& circuit,
                        const std::string& input);
uint16_t getOutput(std::map<std::string, Gate>& circuit,
                   const std::string& wire);
void addGate(std::map<std::string, Gate>& circuit, const std::string& wire,
             const Gate& g);
void resetMemoization(std::map<std::string, Gate>& circuit);
void displayCircuit(std::map<std::string, Gate>& circuit);

uint16_t evaluateOutput(const std::map<std::string, Gate>& circuit,
                        const std::string& input) {
  return isNumber(input)
           ? static_cast<uint16_t>(std::stoi(input))
           : getOutput(const_cast<std::map<std::string, Gate>&>(circuit),
                       input);
}

uint16_t getOutput(std::map<std::string, Gate>& circuit,
                   const std::string& wire) {
  auto it = circuit.find(wire);
  if (it == circuit.end()) {
    std::cerr << "Unknown wire: " << wire << std::endl;
    return 0;
  }

  Gate& g = it->second;
  if (g.memoized) return g.val;

  uint16_t result = 0;
  switch (g.op) {
    case OP_STORE: result = evaluateOutput(circuit, g.lhs); break;
    case OP_AND:
      result = evaluateOutput(circuit, g.lhs) & evaluateOutput(circuit, g.rhs);
      break;
    case OP_OR:
      result = evaluateOutput(circuit, g.lhs) | evaluateOutput(circuit, g.rhs);
      break;
    case OP_LSHIFT:
      result = evaluateOutput(circuit, g.lhs) << evaluateOutput(circuit, g.rhs);
      break;
    case OP_RSHIFT:
      result = evaluateOutput(circuit, g.lhs) >> evaluateOutput(circuit, g.rhs);
      break;
    case OP_NOT: result = ~evaluateOutput(circuit, g.lhs); break;
    default:
      std::cerr << "Unknown operation for wire: " << wire << std::endl;
      return 0;
  }

  g.memoized = true;
  g.val = result;
  return result;
}

void addGate(std::map<std::string, Gate>& circuit, const std::string& wire,
             const Gate& g) {
  circuit[wire] = g;
}

void resetMemoization(std::map<std::string, Gate>& circuit) {
  for (auto& pair : circuit)
    pair.second.memoized = false;
}

void displayCircuit(std::map<std::string, Gate>& circuit) {
  for (const auto& pair : circuit)
    std::cout << pair.first << ": " << getOutput(circuit, pair.first)
              << std::endl;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::map<std::string, Gate> circuit;

  std::regex assign_op{"(\\w+) -> (\\w+)"};
  std::regex not_op{"NOT (\\w+) -> (\\w+)"};
  std::regex binary_op{"(\\w+) (AND|OR|LSHIFT|RSHIFT) (\\w+) -> (\\w+)"};

  std::string command;
  while (std::getline(input_file, command)) {
    std::smatch fields;
    Gate g;

    if (std::regex_match(command, fields, assign_op)) {
      g.op = OP_STORE;
      g.lhs = fields[1];
      addGate(circuit, fields[2], g);
    } else if (std::regex_match(command, fields, not_op)) {
      g.op = OP_NOT;
      g.lhs = fields[1];
      addGate(circuit, fields[2], g);
    } else if (std::regex_match(command, fields, binary_op)) {
      g.op = stringToOp(fields[2]);
      g.lhs = fields[1];
      g.rhs = fields[3];
      addGate(circuit, fields[4], g);
    } else std::cerr << "Unknown gate: " << command << std::endl;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  uint16_t a_output = getOutput(circuit, "a");

  resetMemoization(circuit);
  Gate b_gate;
  b_gate.op = OP_STORE;
  b_gate.lhs = std::to_string(a_output);
  b_gate.memoized = true;
  b_gate.val = a_output;
  addGate(circuit, "b", b_gate);

  a_output = getOutput(circuit, "a");

  int total = a_output;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
