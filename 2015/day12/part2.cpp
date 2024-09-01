/**
 * @file part2.cpp
 * @brief JSON Number Summation with Red Filter
 *
 * This program builds on the previous task of summing all numbers in a JSON
 * document for Santa's Accounting-Elves. However, it adds a new requirement:
 * ignoring any object that contains a property with the value "red", along with
 * all its children. This rule applies only to objects ({...}), not arrays
 * ([...]).
 *
 * The JSON document may contain:
 * - Arrays, such as [1,2,3], where each element is a number.
 * - Objects, such as {"a":1, "b":2}, where values may be numbers, strings, or
 *   other objects.
 * - Nested structures, such as [1,{"c":"red","b":2},3], where objects with
 *   "red" are ignored in the summation.
 * - Empty arrays [] or objects {} which do not contribute to the sum.
 *
 * The task is to iterate through the JSON document, extract all the numbers
 * while filtering out any object with "red" in it, and calculate the total sum.
 *
 * Example Input:
 * - [1,2,3] still yields a sum of 6.
 * - [1,{"c":"red","b":2},3] now yields a sum of 4, because the middle object is
 *   ignored.
 * - {"d":"red","e":[1,2,3,4],"f":5} now yields a sum of 0, because the entire
 *   structure is ignored.
 * - [1,"red",5] yields a sum of 6, because "red" in an array has no effect.
 *
 * The program processes the input JSON document, applies the "red" filter, and
 * outputs the sum of all valid numbers.
 *
 * Puzzle Answer: [Filtered Sum of All Numbers]
 *
 * @author [gabrielzschmitz]
 * @date [01/09/2024]
 */
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

enum class Nestable {
  Array,
  Object,
};

void must_pop(std::vector<Nestable>& nestables, Nestable nestable) {
  if (nestables.back() != nestable) {
    std::cerr << "malformed: bad close" << std::endl;
    std::abort();
  }
  nestables.pop_back();
}

void solve(std::istream& is, int& total) {
  std::vector<Nestable> nestables;
  std::vector<std::pair<int, bool>> objs{{0, false}};

  bool neg = false;
  int cur_num = 0;

  int quote_start = 0;
  int quote_red = 0;
  bool in_string = false;

  auto parse_num = [&total, &cur_num, &neg, &objs]() {
    int val = cur_num * (neg ? -1 : 1);
    total += val;
    std::get<int>(objs.back()) += val;
    cur_num = 0;
    neg = false;
  };

  char c;
  int i = 0;
  while (is.get(c)) {
    i++;
    if (in_string) {
      switch (c) {
        case 'r':
          if (quote_start == i - 1) { quote_red++; }
          break;
        case 'e':
          if (quote_start == i - 2) { quote_red++; }
          break;
        case 'd':
          if (quote_start == i - 3) { quote_red++; }
          break;
        case '"':
          if (quote_red == 3 && quote_start == i - 4 &&
              nestables.back() == Nestable::Object) {
            std::get<bool>(objs.back()) = true;
          }
          in_string = false;
          break;
      }
      continue;
    }
    switch (c) {
      case '{':
        nestables.push_back(Nestable::Object);
        objs.emplace_back(0, false);
        break;
      case '}':
        parse_num();
        must_pop(nestables, Nestable::Object);
        {
          int objval =
            std::get<bool>(objs.back()) ? 0 : std::get<int>(objs.back());
          objs.pop_back();
          std::get<int>(objs.back()) += objval;
        }
        break;
      case '[': nestables.push_back(Nestable::Array); break;
      case ']':
        parse_num();
        must_pop(nestables, Nestable::Array);
        break;
      case '"':
        in_string = true;
        quote_start = i;
        quote_red = 0;
        break;
      case ',': parse_num(); break;
      case '-':
        if (neg || cur_num != 0) {
          std::cerr << "malformed: bad neg" << std::endl;
          std::abort();
        }
        neg = true;
        break;
      case ':':
      case '\n': break;
      default:
        if ('0' <= c && c <= '9') {
          cur_num = cur_num * 10 + (c - '0');
        } else {
          std::cerr << "malformed: bad char" << c << std::endl;
          std::abort();
        }
        break;
    }
  }

  if (objs.size() > 1) {
    std::cerr << "malformed: unclosed obj" << std::endl;
    std::abort();
  }
  if (!nestables.empty()) {
    std::cerr << "malformed: unclosed arr" << std::endl;
    std::abort();
  }
  if (objs.empty()) {
    std::cerr << "impossible: always have one obj" << std::endl;
    std::abort();
  }

  total = std::get<bool>(objs.back()) ? 0 : std::get<int>(objs.back());
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  int total = 0;
  try {
    solve(input_file, total);
  } catch (...) {
    std::cerr << "Error processing the JSON file" << std::endl;
    return 1;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
