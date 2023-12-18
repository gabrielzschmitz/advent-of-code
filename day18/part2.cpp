/**
 * @file part2.cpp
 * @brief Lavaduct Lagoon - Extract Correct Instructions
 *
 * Extract the correct instructions from the hexadecimal color codes in the dig
 * plan. Each hexadecimal code is six digits long, where the first five digits
 * encode the distance in meters as a five-digit hexadecimal number, and the
 * last digit encodes the direction to dig (0 means R, 1 means D, 2 means L, and
 * 3 means U). Convert the hexadecimal codes into the true instructions to fix
 * the bug in the dig plan.
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
 * Lagoon Lava Capacity: 952408144115 cubic meters
 *
 * Extract the correct instructions from the hexadecimal color codes and
 * calculate the total cubic meters of lava the lagoon can hold when following
 * the new dig plan.
 *
 * @author [gabrielzschmitz]
 * @date [18/12/2023]
 */
#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
using namespace std;

bool debug = false;

void followDigPlain(vector<tuple<int64_t, int64_t>>& digPlain,
                    int64_t& boundaryPoints, ifstream& input_file) {
  unordered_map<char, pair<int, int>> directions = {
      {'U', {-1, 0}}, {'D', {1, 0}}, {'L', {0, -1}}, {'R', {0, 1}}};

  string line;
  while (getline(input_file, line)) {
    string _, __;
    string hex;
    istringstream iss(line);
    iss >> _ >> __ >> hex;
    hex = hex.substr(2, hex.length() - 3);

    char directionChar = "RDLU"[stoi(hex.substr(hex.length() - 1))];
    pair<int, int> direction = directions[directionChar];
    int64_t numberSteps = stoi(hex.substr(0, hex.length() - 1), nullptr, 16);

    boundaryPoints += numberSteps;

    int64_t rotation, coordenates;
    tie(rotation, coordenates) = digPlain.back();
    digPlain.push_back({rotation + direction.first * numberSteps,
                        coordenates + direction.second * numberSteps});
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

  int64_t total = 0;

  vector<tuple<int64_t, int64_t>> digPlain = {{0, 0}};
  int64_t boundaryPoints = 0;
  followDigPlain(digPlain, boundaryPoints, input_file);

  if (debug)
    for (tuple<int64_t, int64_t> i : digPlain)
      cout << "(" << get<0>(i) << ", " << get<1>(i) << ") ";

  /* Shoelace formula */
  int64_t shoelaceArea = 0;
  for (int64_t i = 1; i < digPlain.size(); ++i)
    shoelaceArea +=
        get<0>(digPlain[i]) *
        (get<1>(digPlain[i - 1]) - get<1>(digPlain[(i + 1) % digPlain.size()]));
  shoelaceArea = abs(shoelaceArea) / 2;

  /* Pick's theorem */
  int64_t interiorPoints = shoelaceArea - boundaryPoints / 2 + 1;

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
