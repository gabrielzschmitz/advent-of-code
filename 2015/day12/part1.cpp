/**
 * @file part1.cpp
 * @brief JSON Number Summation
 *
 * This program assists Santa's Accounting-Elves in balancing the books by
 * processing a JSON document that contains various data types, including
 * arrays, objects, numbers, and strings. The goal is to find and sum all the
 * numbers present in the entire JSON document.
 *
 * The JSON document may contain:
 * - Arrays, such as [1,2,3], where each element is a number.
 * - Objects, such as {"a":1, "b":2}, where values may be numbers.
 * - Nested structures, such as [[[3]]] or {"a":{"b":4},"c":-1}.
 * - Empty arrays [] or objects {} which do not contribute to the sum.
 *
 * The task is to iterate through the JSON document, extract all the numbers,
 * and calculate the total sum.
 *
 * Example Input:
 * - [1,2,3] and {"a":2,"b":4} both yield a sum of 6.
 * - [[[3]]] and {"a":{"b":4},"c":-1} both yield a sum of 3.
 * - {"a":[-1,1]} and [-1,{"a":1}] both yield a sum of 0.
 * - [] and {} both yield a sum of 0.
 *
 * The program processes the input JSON document and outputs the sum of all
 * numbers found.
 *
 * Puzzle Answer: [Sum of All Numbers]
 *
 * @author [gabrielzschmitz]
 * @date [01/09/2024]
 */
#include <cctype>
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

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  char ch;
  int total = 0;
  int current_number = 0;
  bool isNegative = false;
  bool inNumber = false;
  while (input_file.get(ch)) {
    if (ch == '-') {
      isNegative = true;
    } else if (isdigit(ch)) {
      current_number = current_number * 10 + (ch - '0');
      inNumber = true;
    } else {
      if (inNumber) {
        if (isNegative) { current_number = -current_number; }
        total += current_number;
        current_number = 0;
        isNegative = false;
        inNumber = false;
      }
    }
  }
  if (inNumber) {
    if (isNegative) { current_number = -current_number; }
    total += current_number;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
