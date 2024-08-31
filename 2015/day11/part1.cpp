/**
 * @file part1.cpp
 * @brief Santa's Password Generator
 *
 * This program assists Santa in generating a new secure password according to
 * strict corporate policy. Santa's new password must be exactly eight lowercase
 * letters and must adhere to several additional security rules:
 * - It must include one increasing straight of at least three letters, such as
 *   "abc", "bcd", "cde", etc.
 * - It must not contain the letters "i", "o", or "l" due to their similarity to
 *   other characters.
 * - It must contain at least two different, non-overlapping pairs of letters,
 *   such as "aa", "bb", or "zz".
 *
 * The program starts with Santa's current password and increments it until a
 * valid password is found. Incrementing is similar to counting: the rightmost
 * letter is increased first, and if it wraps from 'z' to 'a', the next letter
 * to the left is incremented, and so on.
 *
 * Example Input:
 * - "abcdefgh" → Next valid password is "abcdffaa"
 * - "ghijklmn" → Next valid password is "ghjaabcc"
 *
 * The program processes the current password and outputs the next valid
 * password according to the rules.
 *
 * Puzzle Answer: [Next Valid Password]
 *
 * @author [gabrielzschmitz]
 * @date [31/08/2024]
 */
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

std::string incrementPassword(std::string password) {
  for (int i = password.length() - 1; i >= 0; --i) {
    if (password[i] == 'z') password[i] = 'a';
    else {
      password[i]++;
      break;
    }
  }
  return password;
}

bool hasIncreasingStraight(const std::string& password) {
  for (size_t i = 0; i < password.length() - 2; ++i)
    if (password[i] + 1 == password[i + 1] &&
        password[i + 1] + 1 == password[i + 2])
      return true;
  return false;
}

bool containsForbiddenLetters(const std::string& password) {
  return password.find_first_of("iol") != std::string::npos;
}

bool hasTwoNonOverlappingPairs(const std::string& password) {
  int pairCount = 0;
  for (size_t i = 0; i < password.length() - 1; ++i) {
    if (password[i] == password[i + 1]) {
      pairCount++;
      i++;
      if (pairCount >= 2) return true;
    }
  }
  return false;
}

bool isValidPassword(const std::string& password) {
  return hasIncreasingStraight(password) &&
         !containsForbiddenLetters(password) &&
         hasTwoNonOverlappingPairs(password);
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string password;
  std::getline(input_file, password);

  do {
    password = incrementPassword(password);
  } while (!isValidPassword(password));

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::string total = password;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
