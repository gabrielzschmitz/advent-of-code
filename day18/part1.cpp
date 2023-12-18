/**
 * @file part1.cpp
 * @brief Lavaduct Lagoon - Calculate Lava Capacity
 *
 * Determine the total cubic meters of lava the lagoon can hold based on the dig
 * plan provided by the Elves. The dig plan specifies the directions (U, D, L,
 * R) and distances to dig, resulting in a looped trench. The lagoon's capacity
 * is calculated by first creating the edge of the trench and then digging out
 * the interior to make it one meter deep. The edge of the trench is also
 * painted with RGB hexadecimal color codes.
 *
 * Example Input:
 * R 6 (#70c710)
 * D 5 (#0dc571)
 * L 2 (#5713f0)
 * D 2 (#d2c081)
 * R 2 (#59c680)
 * D 2 (#411b91)
 * L 5 (#8ceee2)
 * U 2 (#caa173)
 * L 1 (#1b58a2)
 * U 2 (#caa171)
 * R 2 (#7807d2)
 * U 3 (#a77fa3)
 * L 2 (#015232)
 * U 2 (#7a21e3)
 *
 * Example Output:
 * Lagoon Lava Capacity: 62 cubic meters
 *
 * Analyze the provided dig plan to calculate the total cubic meters of lava the
 * lagoon can hold.
 *
 * @author [gabrielzschmitz]
 * @date [18/12/2023]
 */
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
using namespace std;

bool debug = false;

void followDigPlain(vector<tuple<int, int>>& digPlain, int& boundaryPoints,
                    ifstream& input_file) {
  unordered_map<char, pair<int, int>> directions = {
      {'U', {-1, 0}}, {'D', {1, 0}}, {'L', {0, -1}}, {'R', {0, 1}}};

  string line;
  while (getline(input_file, line)) {
    istringstream iss(line);
    string direction, steps, _;
    iss >> direction >> steps >> _;

    int directionRotation, directionCoordenates;
    char directionChar = direction[0];
    tie(directionRotation, directionCoordenates) = directions[directionChar];
    int numberSteps = stoi(steps);

    boundaryPoints += numberSteps;

    int rotation, coordenates;
    tie(rotation, coordenates) = digPlain.back();
    digPlain.push_back({rotation + directionRotation * numberSteps,
                        coordenates + directionCoordenates * numberSteps});
  }
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";
  if (debug) user_input = "test";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  int total = 0;

  int boundaryPoints = 0;
  vector<tuple<int, int>> digPlain = {{0, 0}};

  followDigPlain(digPlain, boundaryPoints, input_file);
  if (debug)
    for (tuple<int, int> i : digPlain)
      cout << "(" << get<0>(i) << ", " << get<1>(i) << ") ";

  /* Shoelace formula */
  int shoelaceArea = 0;
  for (int i = 0; i < digPlain.size(); ++i) {
    shoelaceArea += get<0>(digPlain[i]) *
                    (get<1>(digPlain[i - 1]) - get<1>(digPlain[(i + 1)]));
  }
  shoelaceArea = abs(shoelaceArea) / 2;

  /* Pick's theorem */
  int interiorPoints = shoelaceArea - boundaryPoints / 2 + 1;

  if (debug) {
    cout << "\n\n";
    cout << "(i, b, A): ";
    cout << "(" << interiorPoints << ", " << boundaryPoints << ", "
         << shoelaceArea << ")" << endl;
  }

  total = interiorPoints + boundaryPoints;
  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
