/**
 * @file part1.cpp
 * @brief Cube Game ID Calculator
 *
 * This program processes input data from a file, representing multiple cube
 * games with revealed cube subsets. Each game is identified by an ID, and the
 * subsets contain counts of red, green, and blue cubes. The goal is to
 * determine which games would have been possible if the bag originally
 * contained 12 red cubes, 13 green cubes, and 14 blue cubes.
 *
 * The input file structure consists of lines for each game, including the game
 * ID and semicolon-separated subsets of cubes with counts for red, green, and
 * blue. The program calculates the possible games based on the specified cube
 * limits and outputs the sum of their IDs.
 *
 * Example Input:
 * Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
 * Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
 * Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
 * Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
 * Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green
 *
 * The program outputs the sum of IDs for the games that would have been
 * possible if the bag originally contained 12 red, 13 green, and 14 blue cubes.
 *
 * @author [gabrielzschmitz]
 * @date [02/12/2023]
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
using namespace std;

int get_game_ID(string line);
vector<string> separate_rounds(string line);
vector<int> get_round_cubes(string round);
int sumVector(const vector<int>& vec);

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  int red_cubes = 12;
  int green_cubes = 13;
  int blue_cubes = 14;
  int total_IDs = 0;

  string line;
  vector<int> valid_IDs;

  while (getline(input_file, line)) {
    int current_ID = get_game_ID(line);

    vector<string> rounds = separate_rounds(line);
    int pass = 1;
    for (string round : rounds) {
      vector<int> cubes = get_round_cubes(round);
      if (cubes[0] > red_cubes) pass = 0;
      if (cubes[1] > green_cubes) pass = 0;
      if (cubes[2] > blue_cubes) pass = 0;
    }

    if (pass == 1) {
      valid_IDs.push_back(current_ID);
    }
  }
  total_IDs = sumVector(valid_IDs);
  cout << "NUMBER:" << total_IDs << "\n";

  //  Close the file
  input_file.close();
  return 0;
}

int get_game_ID(string line) {
  int ID = -1;
  char c;
  int i = 0;
  string number;

  while (c != ':') {
    c = line[i];
    if (isdigit(c)) {
      number += c;
    }
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
  while (getline(raw_input, tmp, ';')) rounds.push_back(tmp);

  return rounds;
}

vector<int> get_round_cubes(string round) {
  vector<int> cubes(3, 0);

  int redpos = round.find("red");
  int greenpos = round.find("green");
  int bluepos = round.find("blue");

  if (redpos != -1) {
    int i = redpos - 2;
    int j = 0;
    while (round[i] != ' ') {
      int number = 0;
      if (j == 0)
        number = ((round[i] - '0') * pow(10, j));
      else
        number = cubes[0] + ((round[i] - '0') * pow(10, j));
      cubes.insert(cubes.begin(), number);
      i--;
      j++;
    };
  }

  if (greenpos != -1) {
    int i = greenpos - 2;
    int j = 0;
    while (round[i] != ' ') {
      int number = 0;
      if (j == 0)
        number = ((round[i] - '0') * pow(10, j));
      else
        number = cubes[1] + ((round[i] - '0') * pow(10, j));
      cubes.insert(cubes.begin() + 1, number);
      i--;
      j++;
    };
  }

  if (bluepos != -1) {
    int i = bluepos - 2;
    int j = 0;
    while (round[i] != ' ') {
      int number = 0;
      if (j == 0)
        number = ((round[i] - '0') * pow(10, j));
      else
        number = cubes[2] + ((round[i] - '0') * pow(10, j));
      cubes.insert(cubes.begin() + 2, number);
      i--;
      j++;
    };
  }

  return cubes;
}

int sumVector(const vector<int>& vec) {
  int sum = 0;

  for (int value : vec) {
    sum += value;
  }

  return sum;
}
