/**
 * @file part2.cpp
 * @brief Chronospatial Computer Self-Replication
 *
 * This program continues the simulation of the 3-bit computer, but the goal now
 * is to find the lowest positive value that can be initialized in register A so
 * that the program outputs an exact copy of itself. The program's output is
 * influenced by the state of registers A, B, and C, and we need to adjust the
 * initial value of register A to ensure that the **out** instructions in the
 * program produce the program's own code as output.
 *
 * **Problem Description:**
 * - The program consists of the same opcodes and operands as before, but now
 *   the task is to determine the minimum value that should be set in register A
 *   for the program to output its own code.
 * - The program halts after generating a self-replicating output sequence.
 * 
 * **Objective:**
 * 1. Given the current state of registers (A, B, C), and the program itself,
 * find the lowest positive value that can be assigned to register A.
 * 2. Ensure that when the program is executed with this value, the output
 * matches the program's own code.
 * 3. Return this value as the answer.
 *
 * **Example Input:**
 * ```
 * Register A: 2024
 * Register B: 0
 * Register C: 0
 * Program: 0,3,5,4,3,0
 * ```
 *
 * **Puzzle Answer:** [Minimum Value for Register A]
 *
 * @author [gabrielzschmitz]
 * @date [06/02/2025]
 */
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

using Program = std::vector<std::int64_t>;
using Registers = std::unordered_map<std::string, std::int64_t>;

std::vector<std::int64_t> stringToVector(const std::string& str) {
  std::vector<std::int64_t> result;
  std::stringstream ss(str);
  std::string item;
  while (std::getline(ss, item, ','))
    result.push_back(std::stoi(item));
  return result;
}

std::string runProgram(const Program& program, Registers& registers,
                       std::unordered_map<std::string, std::string>& memo) {
  std::string result;
  size_t pointer = 0;

  auto comboOperand = [&](std::int64_t value) -> int64_t {
    switch (value) {
      case 0: return 0;
      case 1: return 1;
      case 2: return 2;
      case 3: return 3;
      case 4: return registers["A"];
      case 5: return registers["B"];
      case 6: return registers["C"];
      default: throw std::runtime_error("Combo operand is reserved");
    }
  };

  while (pointer < program.size()) {
    std::int64_t opcode = program[pointer];
    int operand = (pointer + 1 < program.size()) ? program[pointer + 1] : 0;
    pointer += 2;

    switch (opcode) {
      case 0: registers["A"] >>= comboOperand(operand); break;
      case 1: registers["B"] ^= operand; break;
      case 2: registers["B"] = comboOperand(operand) & 0b111; break;
      case 3:
        if (registers["A"]) pointer = operand;
        break;
      case 4: registers["B"] ^= registers["C"]; break;
      case 5:
        result += std::to_string(comboOperand(operand) & 0b111) + ",";
        break;
      case 6: registers["B"] = registers["A"] >> comboOperand(operand); break;
      case 7: registers["C"] = registers["A"] >> comboOperand(operand); break;
      default: throw std::runtime_error("Unknown opcode encountered");
    }
  }

  if (!result.empty()) result.pop_back();

  return result;
}

std::vector<std::int64_t> generateA(const Program& program,
                                    const std::vector<std::int64_t>& output) {
  if (output.empty()) return {0};

  std::vector<std::int64_t> results;
  std::vector<std::int64_t> previousResults = generateA(
    program, std::vector<std::int64_t>(output.begin() + 1, output.end()));

  std::unordered_map<std::string, std::string> memo;

  for (std::int64_t ah : previousResults) {
    for (std::int64_t al = 0; al < 8; ++al) {
      std::int64_t a = ah * 8 + al;
      Registers state = {{"A", a}, {"B", 0}, {"C", 0}};

      std::string state_key = "A:" + std::to_string(a) + "_B:0_C:0";

      if (memo.find(state_key) != memo.end()) {
        if (memo[state_key] == std::to_string(a)) { results.push_back(a); }
      } else {
        std::string result = runProgram(program, state, memo);
        memo[state_key] = result;
        if (stringToVector(result) == output) { results.push_back(a); }
      }
    }
  }

  return results;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  Registers registers;
  Program program;
  std::string line;

  while (std::getline(input_file, line)) {
    if (line.empty()) break;

    std::string register_name;
    int64_t value;
    std::string ignore;
    std::istringstream line_stream(line);
    line_stream >> ignore >> register_name >> value;
    size_t colon_pos = register_name.find(':');
    register_name = register_name.substr(0, colon_pos);
    registers[register_name] = value;

    if (debug) std::cout << register_name << ": " << value << ", ";
  }
  if (debug) std::cout << std::endl;

  while (std::getline(input_file, line))
    if (!line.empty()) break;

  size_t program_start = line.find("Program: ") + 9;
  if (program_start == std::string::npos)
    throw std::runtime_error("Program line format incorrect");
  std::string program_str = line.substr(program_start);
  std::istringstream program_stream(program_str);
  std::string opcode;

  while (std::getline(program_stream, opcode, ','))
    program.push_back(std::stoi(opcode));

  if (debug) {
    for (size_t i = 0; i < program.size(); i += 2) {
      std::cout << "Opcode: " << program[i];
      if (i + 1 < program.size()) std::cout << ", Operand: " << program[i + 1];
      std::cout << " | ";
    }
    std::cout << std::endl;
  }

  std::vector<std::int64_t> total = generateA(program, program);
  auto min_value = *std::min_element(total.begin(), total.end());
  std::cout << "ANSWER: " << min_value << std::endl;

  return 0;
}
