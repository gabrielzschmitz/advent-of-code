/**
 * @file part1.cpp
 * @brief Monkey Exchange Market - Secret Number Simulation
 *
 * In this challenge, you are tasked with simulating the pseudorandom sequence
 * of secret numbers used by buyers on the Monkey Exchange Market. Each buyer
 * generates a sequence of secret numbers starting from their initial secret
 * number (provided as input). The sequence evolves according to specific rules
 * involving arithmetic operations, bitwise manipulation, and modular
 * arithmetic.
 *
 * The process for generating the next secret number is as follows:
 * 1. Multiply the secret number by 64, mix the result into the secret number,
 * and prune it.
 * 2. Divide the secret number by 32, round down, mix the result into the secret
 * number, and prune it.
 * 3. Multiply the secret number by 2048, mix the result into the secret number,
 * and prune it.
 *
 * Mixing involves calculating the bitwise XOR of the secret number and the
 * given value. Pruning involves taking the secret number modulo 16777216.
 *
 * The goal is to simulate the generation of 2000 new secret numbers for each
 * buyer and sum the 2000th secret number from all buyers.
 *
 * Example Input:
 * 1
 * 10
 * 100
 * 2024
 *
 * Example Output:
 * Sum of 2000th Secret Numbers: 37327623
 *
 * Tasks:
 * 1. Parse the initial secret numbers for each buyer.
 * 2. Simulate the sequence of secret numbers for 2000 iterations per buyer.
 * 3. Output the sum of the 2000th secret number from all buyers.
 *
 * Puzzle Answer: [Sum of 2000th Secret Numbers]
 *
 * @author [gabrielzschmitz]
 * @date [22/12/2024]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

uint32_t evolveSecret(uint32_t secret) {
  // Multiply by 64, XOR, prune
  secret = (secret ^ (secret * 64)) % 16777216;

  // Divide by 32, XOR, prune
  secret = (secret ^ (secret / 32)) % 16777216;

  // Multiply by 2048, XOR, prune
  secret = (secret ^ (secret * 2048)) % 16777216;

  return secret;
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

  std::string line;
  std::vector<uint32_t> initial_secrets;

  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    uint32_t secret;
    while (iss >> secret)
      initial_secrets.push_back(secret);
  }

  for (uint32_t secret : initial_secrets) {
    for (int i = 1; i <= 2000; ++i) {
      secret = evolveSecret(secret);
      if (debug && i % 500 == 0)
        std::cout << "Intermediate secret after " << i
                  << " iterations: " << secret << std::endl;
    }
    total += secret;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
