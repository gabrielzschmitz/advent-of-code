/**
 * @file part2.cpp
 * @brief Enhanced Snow Production Calibration
 *
 * This program reads input data from a file, specified by a command-line
 * argument or defaulting to "input". It processes each line of random
 * characters, including numbers represented in words (one, two, three, ...,
 * nine), extracts digits, and converts them into integers. The program
 * calculates the total sum of integers obtained by combining the real first and
 * last digits or their word equivalents in each line. For instance, "two1nine"
 * will be interpreted as 29. The final sum is then output to the console.
 *
 * The input file is expected to contain lines with random characters and
 * optional numbers represented as words. The program filters out non-digit
 * characters and converts word representations to digits for summation.
 *
 * Example Input:
 * two1nine
 * eightwothree
 * abcone2threexyz
 * xtwone3four
 * 4nineeightseven2
 * zoneight234
 * 7pqrstsixteen
 *
 * The program outputs the sum of integers extracted from each line, considering
 * both numeric digits and numbers represented in words.
 * If the input file is inaccessible, an error message is displayed.
 *
 * The program utilizes a map to associate numeric values with their word
 * representations for conversion.
 *
 * @author [gabrielzschmitz]
 * @date [01/12/2023]
 */
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
using namespace std;

map<unsigned int, string> numbers = {
  {0, "zero"}, {1, "one"}, {2, "two"},   {3, "three"}, {4, "four"},
  {5, "five"}, {6, "six"}, {7, "seven"}, {8, "eight"}, {9, "nine"}};
int convert_line_to_int(int total, const string& line);

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  int total = 0;
  string line;
  while (getline(input_file, line)) {
    total = convert_line_to_int(total, line);
  }

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}

int convert_line_to_int(int total, const string& line) {
  int first = -1;
  int last = 0;

  for (string::const_iterator line_it = line.begin(); line_it != line.end();
       ++line_it) {
    char c = *line_it;
    if (isdigit(c)) {
      last = c - '0';
      if (-1 == first) { first = last; }
    } else {
      for (const pair<unsigned int, string>& num : numbers) {
        if (strncmp(&line[distance(line.begin(), line_it)], num.second.c_str(),
                    num.second.length()) == 0) {
          last = num.first;
          if (-1 == first) { first = last; }
        }
      }
    }
  }

  unsigned long value = 10 * first + last;
  total += value;

  return total;
}
