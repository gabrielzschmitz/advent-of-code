/**
 * @file part2.cpp
 * @brief Detecting IPv7 Addresses that Support SSL
 *
 * This program analyzes a list of IPv7 addresses to determine which support
 * SSL (Super-Secret Listening). An address supports SSL if it contains at
 * least one ABA pattern in any supernet sequence (outside square brackets)
 * with a corresponding BAB pattern in any hypernet sequence (inside square
 * brackets).
 *
 * An IPv7 address consists of supernet sequences (text outside square
 * brackets) and hypernet sequences (text inside square brackets). A valid ABA
 * is a three-character pattern of the form ABA, where the first and last
 * characters are the same and the middle character is different (e.g., "aba"
 * or "xyx"). The corresponding BAB swaps the two characters, producing
 * patterns such as "bab" or "yxy". Sequences like "aaa" are not valid because
 * the middle character must differ.
 *
 * The program scans each IPv7 address and:
 *   - Finds every valid ABA pattern in the supernet sequences.
 *   - Searches the hypernet sequences for the corresponding BAB pattern.
 *   - Counts the address as supporting SSL if at least one ABA/BAB pair
 *   exists.
 *
 * Example Input:
 * aba[bab]xyz
 * xyx[xyx]xyx
 * aaa[kek]eke
 * zazbz[bzb]cdb
 *
 * Example Results:
 * aba[bab]xyz      // Supports SSL
 * xyx[xyx]xyx      // Does not support SSL
 * aaa[kek]eke      // Supports SSL
 * zazbz[bzb]cdb    // Supports SSL
 * 3
 *
 * The program processes all IPv7 addresses in the input and outputs the number
 * of addresses that support SSL.
 *
 * Puzzle Answer: [Number of SSL-Supported IPv7 Addresses]
 *
 * @author [gabrielzschmitz]
 * @date [18/07/2026]
 */
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

bool isABA(const std::string& s, std::size_t i) {
  return s[i] == s[i + 2] && s[i] != s[i + 1];
}

bool supportsSSL(const std::string& ip) {
  bool insideHypernet = false;

  std::set<std::string> abas;
  std::set<std::string> babs;

  for (std::size_t i = 0; i < ip.size(); ++i) {
    if (ip[i] == '[') {
      insideHypernet = true;
      continue;
    }

    if (ip[i] == ']') {
      insideHypernet = false;
      continue;
    }

    if (i + 2 >= ip.size()) continue;

    // Don't allow windows to cross brackets.
    if (!std::isalpha(static_cast<unsigned char>(ip[i])) ||
        !std::isalpha(static_cast<unsigned char>(ip[i + 1])) ||
        !std::isalpha(static_cast<unsigned char>(ip[i + 2])))
      continue;

    if (!isABA(ip, i)) continue;

    std::string pattern = ip.substr(i, 3);

    if (insideHypernet)
      babs.insert(pattern);
    else
      abas.insert(pattern);
  }

  for (const auto& aba : abas) {
    std::string bab;
    bab += aba[1];
    bab += aba[0];
    bab += aba[1];

    if (babs.count(bab)) return true;
  }

  return false;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string ip;
  std::uint64_t answer = 0;

  while (std::getline(input_file, ip)) {
    if (supportsSSL(ip)) {
      ++answer;

      if (debug) std::cout << ip << " supports SSL\n";
    } else if (debug)
      std::cout << ip << " does NOT support SSL\n";
  }

  std::cout << "ANSWER: " << answer << '\n';

  return 0;
}
