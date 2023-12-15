/**
 * @file part1.cpp
 * @brief Snow Production Calibration
 *
 * This program aims to resolve issues with global snow production. The task is
 * to check the top fifty locations marked with stars on a provided map by
 * December 25th. The map, containing stars, represents potential problem areas.
 * Stars are collected by solving puzzles available in the Advent calendar.
 * Completing each puzzle grants one star. The goal is to collect all fifty
 * stars by resolving puzzles presented throughout December.
 *
 * The specific puzzle for this program involves a calibration document with
 * lines of text. Each line originally held a calibration value that is now
 * concealed due to an Elf's artistic amendment. The task is to recover the
 * calibration value on each line by combining the first and last digits in that
 * order to form a two-digit number. The program processes the entire
 * calibration document, calculates the sum of all calibration values, and
 * outputs the result.
 *
 * Example Input:
 * 1abc2
 * pqr3stu8vwx
 * a1b2c3d4e5f
 * treb7uchet
 *
 * The program outputs the sum of all calibration values obtained by combining
 * the first and last digits of each line.
 *
 * Puzzle Answer: 56049
 *
 * @author [gabrielzschmitz]
 * @date [01/12/2023]
 */
#include <iostream>
#include <fstream>
using namespace std;

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

  return 0;
}

int convert_line_to_int(int total, const string& line) {
  int first = -1;
  int last = 0;

  for (char c : line) {
    if (isdigit(c)) {
      last = c - '0';
      if (first == -1) {
        first = last;
      }
    }
  }

  unsigned long value = (first != -1) ? 10 * first + last : last;
  total += value;

  return total;
}
