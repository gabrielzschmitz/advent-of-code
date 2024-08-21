/**
 * @file part2.cpp
 * @brief Enhanced Cube Game Minimum Set Calculator
 *
 * This program extends the functionality of the Cube Game ID Calculator
 * to determine the fewest number of cubes of each color that could have been
 * present in the bag for each game. The input file, specified by a command-line
 * argument or defaulting to "input", contains lines with the following
 * structure: "Game ID: round1; round2; ...". Each round represents cube counts
 * for red, green, and blue colors.
 *
 * The program extracts game IDs and rounds, calculating the minimum set of
 * cubes required for each game. It then sums the powers of these minimum sets,
 * where the power of a set is the product of cubes for red, green, and blue
 * colors. The result is the answer to the puzzle.
 *
 * Example Input:
 * Game 1: 3 red, 5 green, 2 blue; 2 red, 1 green, 6 blue
 *
 * The program outputs the sum of the powers of the minimum sets for all games.
 * If the input file is inaccessible, an error message is displayed.
 *
 * The program uses a Cube structure to represent the minimum required cubes for
 * each color in a set. It calculates the power of each minimum set and sums
 * these values to obtain the final result.
 *
 * @author [gabrielzschmitz]
 * @date [02/12/2023]
 */
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

struct Cube {
  int red;
  int green;
  int blue;
};

int get_game_ID(const string& line);
vector<string> separate_rounds(string line);
vector<int> get_round_cubes(const string& round);
unsigned int sumVector(const vector<Cube>& valid_sets);

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  int red_cubes_limit = 12;
  int green_cubes_limit = 13;
  int blue_cubes_limit = 14;

  unsigned int total_sets = 0;
  string line;
  vector<Cube> valid_sets;

  while (getline(input_file, line)) {
    vector<string> rounds = separate_rounds(line);
    Cube minimum = {0, 0, 0};
    for (const string& round : rounds) {
      vector<int> cubes = get_round_cubes(round);
      minimum.red = max(minimum.red, cubes[0]);
      minimum.green = max(minimum.green, cubes[1]);
      minimum.blue = max(minimum.blue, cubes[2]);
    }

    valid_sets.push_back(minimum);
  }

  total_sets = sumVector(valid_sets);
  cout << "NUMBER:" << total_sets << "\n";

  // Close the file
  input_file.close();
  return 0;
}

int get_game_ID(const string& line) {
  int ID = -1;
  char c;
  int i = 0;
  string number;

  while (c != ':') {
    c = line[i];
    if (isdigit(c)) { number += c; }
    i++;
  }

  if (!number.empty()) ID = stoi(number);
  return ID;
}

vector<string> separate_rounds(string line) {
  vector<string> rounds;
  string number;

  line.erase(line.begin(), line.begin() + 7);
  istringstream raw_input(line);

  string tmp;
  while (getline(raw_input, tmp, ';'))
    rounds.push_back(tmp);

  return rounds;
}

vector<int> get_round_cubes(const string& round) {
  vector<int> cubes(3, 0);

  size_t redpos = round.find("red");
  size_t greenpos = round.find("green");
  size_t bluepos = round.find("blue");

  if (redpos != string::npos) {
    int i = redpos - 2;
    int j = 0;
    while (round[i] != ' ') {
      int number = (j == 0) ? (round[i] - '0') * pow(10, j)
                            : cubes[0] + (round[i] - '0') * pow(10, j);
      cubes[0] = number;
      i--;
      j++;
    }
  }

  if (greenpos != string::npos) {
    int i = greenpos - 2;
    int j = 0;
    while (round[i] != ' ') {
      int number = (j == 0) ? (round[i] - '0') * pow(10, j)
                            : cubes[1] + (round[i] - '0') * pow(10, j);
      cubes[1] = number;
      i--;
      j++;
    }
  }

  if (bluepos != string::npos) {
    int i = bluepos - 2;
    int j = 0;
    while (round[i] != ' ') {
      int number = (j == 0) ? (round[i] - '0') * pow(10, j)
                            : cubes[2] + (round[i] - '0') * pow(10, j);
      cubes[2] = number;
      i--;
      j++;
    }
  }

  return cubes;
}

unsigned int sumVector(const vector<Cube>& valid_sets) {
  unsigned int sum = 0;

  for (const Cube& i : valid_sets) {
    unsigned int current_set = i.red * i.green * i.blue;
    sum += current_set;
  }

  return sum;
}
