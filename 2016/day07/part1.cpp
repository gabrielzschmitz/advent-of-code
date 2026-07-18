/**
 * @file part1.cpp
 * @brief Detecting IPv7 Addresses that Support TLS
 *
 * This program analyzes a list of IPv7 addresses to determine which support
 * TLS (Transport-Layer Snooping). An address supports TLS if it contains a
 * valid Autonomous Bridge Bypass Annotation (ABBA) outside of any hypernet
 * sequence while containing no ABBAs within hypernet sequences.
 *
 * An IPv7 address consists of supernet sequences (text outside square
 * brackets) and hypernet sequences (text inside square brackets). A valid ABBA
 * is a four-character pattern of the form ABBA, where the first and last
 * characters are the same, the two middle characters are the same, and the two
 * distinct characters differ from each other (e.g., "abba" or "xyyx").
 * Patterns such as "aaaa" are not considered valid ABBAs.
 *
 * The program scans each IPv7 address and:
 *   - Detects whether at least one valid ABBA appears in a supernet sequence.
 *   - Ensures that no valid ABBA appears within any hypernet sequence.
 *   - Counts the address as supporting TLS only if both conditions are met.
 *
 * Example Input:
 * abba[mnop]qrst
 * abcd[bddb]xyyx
 * aaaa[qwer]tyui
 * ioxxoj[asdfgh]zxcvbn
 *
 * Example Results:
 * abba[mnop]qrst        // Supports TLS
 * abcd[bddb]xyyx        // Does not support TLS
 * aaaa[qwer]tyui        // Does not support TLS
 * ioxxoj[asdfgh]zxcvbn  // Supports TLS
 * 2
 *
 * The program processes all IPv7 addresses in the input and outputs the number
 * of addresses that support TLS.
 *
 * Puzzle Answer: [Number of TLS-Supported IPv7 Addresses]
 *
 * @author [gabrielzschmitz]
 * @date [18/07/2026]
 */
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

// Returns true if s[a..a+3] is an ABBA.
bool isABBA(const std::string& s, std::size_t i) {
  return s[i] == s[i + 3] && s[i + 1] == s[i + 2] && s[i] != s[i + 1];
}

bool supportsTLS(const std::string& ip) {
  bool insideHypernet = false;
  bool hasSupernetABBA = false;

  for (std::size_t i = 0; i < ip.size(); ++i) {
    if (ip[i] == '[') {
      insideHypernet = true;
      continue;
    }

    if (ip[i] == ']') {
      insideHypernet = false;
      continue;
    }

    if (i + 3 >= ip.size()) continue;

    // Prevent windows from crossing brackets.
    if (!std::isalpha(static_cast<unsigned char>(ip[i])) ||
        !std::isalpha(static_cast<unsigned char>(ip[i + 1])) ||
        !std::isalpha(static_cast<unsigned char>(ip[i + 2])) ||
        !std::isalpha(static_cast<unsigned char>(ip[i + 3])))
      continue;

    if (!isABBA(ip, i)) continue;

    if (insideHypernet)
      return false;  // Any ABBA inside brackets invalidates the IP.

    hasSupernetABBA = true;
  }

  return hasSupernetABBA;
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
    if (supportsTLS(ip)) {
      ++answer;

      if (debug) std::cout << ip << " supports TLS\n";
    } else if (debug)
      std::cout << ip << " does NOT support TLS\n";
  }

  std::cout << "ANSWER: " << answer << '\n';

  return 0;
}
