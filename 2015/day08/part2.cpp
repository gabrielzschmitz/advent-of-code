/**
 * @file part2.cpp
 * @brief Santa's Encoded List Storage Calculation
 *
 * This program extends the previous challenge by encoding each original string
 * literal representation into a new format and calculating the space required
 * for these newly encoded strings. Each original string is represented with
 * surrounding double quotes, escape sequences, and special characters.
 * Your task is to determine the difference in space between the original string
 * literals and their newly encoded representations.
 *
 * Encoding Rules:
 * - Each string literal should be surrounded by additional double quotes.
 * - Escape sequences in the original string (\\, \", and \xNN) need to be
 *   encoded as follows:
 *   - \\" should be encoded as \\\".
 *   - \\ should be encoded as \\\\.
 *   - \xNN should be encoded as \\xNN.
 * 
 * Example Encoding:
 * - "" encodes to "\"\"", an increase from 2 characters to 6.
 * - "abc" encodes to "\"abc\"", an increase from 5 characters to 9.
 * - "aaa\"aaa" encodes to "\"aaa\\\"aaa\"", an increase from 10 characters to
 *   16.
 * - "\x27" encodes to "\"\\x27\"", an increase from 6 characters to 11.
 *
 * The program should calculate:
 * 1. The total number of characters in the encoded representations of all
 * string literals.
 * 2. The total number of characters in the original code representations.
 * 3. The difference between these two totals.
 *
 * Puzzle Answer: [Difference in Characters]
 *
 * @author [gabrielzschmitz]
 * @date [28/08/2024]
 */
#include <cmath>
#include <cstring>
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

std::string encodeString(const std::string& str) {
  std::string encoded = "\"";

  for (char ch : str) {
    if (ch == '\\' || ch == '\"') encoded += '\\';
    encoded += ch;
  }

  encoded += "\"";
  return encoded;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string line;
  int total_code_characters = 0;
  int total_encoded_characters = 0;

  while (std::getline(input_file, line)) {
    int code_characters = line.size();
    std::string encoded_line = encodeString(line);
    int encoded_characters = encoded_line.size();

    total_code_characters += code_characters;
    total_encoded_characters += encoded_characters;

    if (debug) {
      std::cout << "orig: " << line << " -" << code_characters << ""
                << std::endl;
      std::cout << "enc : " << encoded_line << " - " << encoded_characters
                << "\n\n";
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  if (debug)
    std::cout << "total: " << total_code_characters << " - "
              << total_encoded_characters << std::endl;

  int total_difference = total_encoded_characters - total_code_characters;
  std::cout << "ANSWER: " << total_difference << std::endl;

  return 0;
}
