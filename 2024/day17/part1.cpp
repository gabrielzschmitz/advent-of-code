/**
 * @file part1.cpp
 * @brief Chronospatial Computer Simulation
 *
 * This program simulates the operation of a 3-bit computer at the North Pole,
 * which executes a series of instructions involving three registers: A, B, and
 * C. The registers can store any integer, and the program consists of 3-bit
 * opcodes paired with operands to perform operations on these registers.
 *
 * **Problem Description:**
 * - The computer begins with a set of initial values for registers A, B, and C.
 * - The program consists of a series of opcodes and operands. Each opcode
 *   corresponds to a specific operation on the registers:
 *   - **Adv** (opcode 0) divides the value in register A by a power of 2 based
 *   on the operand.
 *   - **Bxl** (opcode 1) performs bitwise XOR on register B with the operand.
 *   - **Bst** (opcode 2) stores the operand modulo 8 in register B.
 *   - **Jnz** (opcode 3) conditionally jumps based on the value in register A.
 *   - **Bxc** (opcode 4) performs bitwise XOR on registers B and C.
 *   - **Out** (opcode 5) outputs a value based on the operand modulo 8.
 *   - **Bdv** (opcode 6) works like Adv but stores the result in register B.
 *   - **Cdv** (opcode 7) works like Adv but stores the result in register C.
 *
 * **Objective:**
 * 1. Initialize the registers (A, B, C) as provided.
 * 2. Execute the instructions in the program until it halts.
 * 3. Collect and output the values produced by any **out** instructions.
 * 4. Return the output as a comma-separated string of values.
 *
 * **Example Input:**
 * ```
 * Register A: 729
 * Register B: 0
 * Register C: 0
 * Program: 0,1,5,4,3,0
 * ```
 *
 * **Puzzle Answer:** [Comma-Separated Output]
 *
 * @author [gabrielzschmitz]
 * @date [04/02/2025]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
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

using Program = std::vector<int>;
using Registers = std::unordered_map<std::string, int64_t>;

std::string runProgram(const Program& program, Registers& registers) {
  std::string result;
  size_t pointer = 0;

  auto comboOperand = [&](int value) -> int64_t {
    switch (value) {
      case 0: return value;
      case 1: return value;
      case 2: return value;
      case 3: return value;
      case 4: return registers["A"];
      case 5: return registers["B"];
      case 6: return registers["C"];
      default: throw std::runtime_error("Combo operand is reserved");
    }
  };

  while (pointer < program.size()) {
    int opcode = program[pointer];
    int operand = (pointer + 1 < program.size()) ? program[pointer + 1] : 0;
    pointer += 2;

    switch (opcode) {
      case 0: /* adv (A DiVide) */
        registers["A"] >>= comboOperand(operand);
        break;
      case 1: /* bxl (B Xor Literal) */ registers["B"] ^= operand; break;
      case 2: /* bst (B STore) */
        registers["B"] = comboOperand(operand) & 0b111;
        break;
      case 3: /* jnz (Jump if Not Zero) */
        if (registers["A"]) { pointer = operand; }
        break;
      case 4: /* bxc (B Xor C) */ registers["B"] ^= registers["C"]; break;
      case 5: /* out (OUTput) */
        result += std::to_string(comboOperand(operand) & 0b111) + ",";
        break;
      case 6: /* bdv (B DiVide) */
        registers["B"] = registers["A"] >> comboOperand(operand);
        break;
      case 7: /* cdv (C DiVide) */
        registers["C"] = registers["A"] >> comboOperand(operand);
        break;
      default: throw std::runtime_error("Unknown opcode encountered");
    }
  }

  if (!result.empty()) result.pop_back();

  return result;
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
      if (i + 1 < program.size()) {
        std::cout << ", Operand: " << program[i + 1];
      }
      std::cout << " | ";
    }
    std::cout << std::endl;
  }

  std::string output = runProgram(program, registers);

  std::cout << "ANSWER: " << output << std::endl;
  return 0;
}
