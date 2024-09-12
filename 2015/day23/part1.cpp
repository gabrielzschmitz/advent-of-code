/**
 * @file part1.cpp
 * @brief Opening the Turing Lock
 *
 * This program simulates a simple state-of-the-art computer with two registers,
 * `a` and `b`, and a set of six instructions. The computer begins with both
 * registers initialized to 0. The task is to execute a series of instructions
 * and determine the value of register `b` once the program has finished
 * executing.
 *
 * The instructions are:
 * - `hlf r`: Halves the value in register `r`.
 * - `tpl r`: Triples the value in register `r`.
 * - `inc r`: Increments the value in register `r` by 1.
 * - `jmp offset`: Jumps to an instruction at the specified offset relative to
 *   the current instruction.
 * - `jie r, offset`: Jumps if the value in register `r` is even.
 * - `jio r, offset`: Jumps if the value in register `r` is exactly 1.
 *
 * The program executes instructions in sequence, potentially jumping forwards
 * or backwards based on the conditions of the jump instructions. It exits once
 * an instruction beyond the last defined instruction is attempted.
 *
 * Example Input:
 * ```
 * inc a
 * jio a, +2
 * tpl a
 * inc a
 * ```
 * In this example, register `a` is incremented to 1, then the program jumps
 * ahead to skip the `tpl a` instruction, and finally increments `a` again,
 * resulting in `a = 2`.
 *
 * The task is to run the input program and determine the final value of
 * register `b` after all instructions have been executed.
 *
 * Puzzle Answer: [Final Value of b]
 *
 * @author [gabrielzschmitz]
 * @date [12/09/2024]
 */
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

int simulateProgram(const std::vector<std::string>& instructions) {
  std::unordered_map<std::string, int> registers = {{"a", 0}, {"b", 0}};
  int ip = 0;

  while (ip >= 0 && ip < instructions.size()) {
    std::string inst = instructions[ip];
    std::stringstream ss(inst);
    std::string command, reg;
    int offset;

    ss >> command;

    if (command == "hlf") {
      ss >> reg;
      registers[reg] /= 2;
      ip++;
    } else if (command == "tpl") {
      ss >> reg;
      registers[reg] *= 3;
      ip++;
    } else if (command == "inc") {
      ss >> reg;
      registers[reg]++;
      ip++;
    } else if (command == "jmp") {
      ss >> offset;
      ip += offset;
    } else if (command == "jie") {
      ss >> reg >> offset;
      reg.pop_back();
      if (registers[reg] % 2 == 0) {
        ip += offset;
      } else {
        ip++;
      }
    } else if (command == "jio") {
      ss >> reg >> offset;
      reg.pop_back();
      if (registers[reg] == 1) {
        ip += offset;
      } else {
        ip++;
      }
    }

    if (debug) {
      std::cout << "Executed: " << inst << " | a: " << registers["a"]
                << ", b: " << registers["b"] << std::endl;
    }
  }

  return registers["b"];
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::string> instructions;
  std::string line;

  while (std::getline(input_file, line)) {
    instructions.push_back(line);
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = simulateProgram(instructions);
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
