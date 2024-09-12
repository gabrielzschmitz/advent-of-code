/**
 * @file part2.cpp
 * @brief Opening the Turing Lock - Modified Starting Condition
 *
 * In this continuation of the Turing Lock challenge, the computer operates
 * similarly to the previous program, but with one key difference: register `a`
 * now starts with a value of 1. The same set of instructions and behavior
 * apply, including the two registers (`a` and `b`) and six supported
 * instructions:
 *
 * - `hlf r`: Halves the value in register `r`.
 * - `tpl r`: Triples the value in register `r`.
 * - `inc r`: Increments the value in register `r` by 1.
 * - `jmp offset`: Jumps to an instruction at the specified offset relative to
 *   the current instruction.
 * - `jie r, offset`: Jumps if the value in register `r` is even.
 * - `jio r, offset`: Jumps if the value in register `r` is exactly 1.
 *
 * As before, the instructions execute in sequence, with jumps modifying the
 * flow of execution. The program terminates when an instruction beyond the last
 * defined one is attempted.
 *
 * This time, register `a` begins with a value of 1 instead of 0, and the task
 * is to determine the final value in register `b` when the program completes
 * with this new initial condition.
 *
 * Example Starting Condition:
 * - `a = 1`
 * - `b = 0`
 *
 * The task is to execute the input program with this modified starting
 * condition and determine the final value in register `b`.
 *
 * Puzzle Answer: [Final Value of b]
 *
 * @author [gabrielzschmitz]
 * @date [12/09/2024]
 */
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

uint32_t simulateProgram(const std::vector<std::string>& instructions,
                         const int initialA) {
  std::unordered_map<std::string, uint32_t> registers = {{"a", initialA},
                                                         {"b", 0}};
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

  while (std::getline(input_file, line))
    instructions.push_back(line);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  uint32_t total = simulateProgram(instructions, 1);
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
