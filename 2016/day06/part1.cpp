/**
 * @file part1.cpp
 * @brief Error-Corrected Message Recovery
 *
 * This program reconstructs the original message transmitted by Santa using a
 * repetition code. Multiple copies of the same message have been received, but
 * transmission errors have corrupted some of the characters.
 *
 * Each line of the input represents one received version of the message, and
 * every message has the same length. To recover the original message, the
 * program analyzes each character position independently and selects the
 * character that appears most frequently in that column.
 *
 * The recovered message is formed by combining the most common character from
 * every column in order.
 *
 * Example Input:
 * eedadn
 * drvtee
 * eandsr
 * raavrd
 * atevrs
 * tsrnev
 * sdttsa
 * rasrtv
 * nssdts
 * ntnada
 * svetve
 * tesnvt
 * vntsnd
 * vrdear
 * dvrsen
 * enarar
 *
 * Example Output:
 * easter
 *
 * The program processes all received messages and outputs the reconstructed
 * error-corrected message.
 *
 * Puzzle Answer: [Error-Corrected Message]
 *
 * @author [gabrielzschmitz]
 * @date [18/07/2026]
 */
#include <fstream>
#include <iostream>
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

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::string> messages;
  std::string line;

  while (std::getline(input_file, line)) messages.push_back(line);

  if (messages.empty()) {
    std::cout << "ANSWER:\n";
    return 0;
  }

  std::string answer;
  const std::size_t cols = messages[0].size();

  for (std::size_t col = 0; col < cols; ++col) {
    std::unordered_map<char, int> freq;

    for (const auto& msg : messages) ++freq[msg[col]];

    char most_common = '\0';
    int best = 0;

    for (const auto& [ch, count] : freq) {
      if (count > best) {
        best = count;
        most_common = ch;
      }
    }

    answer += most_common;
  }

  std::cout << "ANSWER: " << answer << '\n';

  return 0;
}
