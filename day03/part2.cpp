/**
 * @file part2.cpp
 * @brief Engine and Gear Ratio Calculator
 *
 * This program reads input data from a file, specified by a command-line
 * argument or defaulting to "input". It processes each line of characters,
 * identifying engines with associated IDs, positions, and neighboring special
 * characters ('*'). The program calculates the gear ratio for each gear,
 * defined as the product of IDs for pairs of engines sharing the same
 * neighboring special character ('*'). The final result is the sum of all these
 * gear ratios.
 *
 * The input file format is expected to contain lines representing a grid of
 * characters. The program identifies engines, where each engine has a unique
 * ID, a position in the grid, and neighboring special characters ('*'). The
 * program considers engines valid if they have exactly two neighboring special
 * characters ('*'). It calculates the gear ratio for each valid gear, defined
 * as the product of IDs for the pair of engines sharing the same neighboring
 * special character ('*').
 *
 * Example Input:
 * 467..114..
 * ...*......
 * ..35..633.
 * ......#...
 * 617*......
 * .....+.58.
 * ..592.....
 * ......755.
 * ...$.*....
 * .664.598..
 *
 * The program outputs the sum of the gear ratios for all valid gears in the
 * engine. If the input file is inaccessible, an error message is displayed.
 *
 * The program utilizes a struct 'Engine' to store information about each
 * engine, including its ID, position, neighboring special characters, and a
 * marked attribute to track processed engines. It uses vectors to store the
 * identified engines and calculates the gear ratios for valid gears.
 *
 * @author [gabrielzschmitz]
 * @date [03/12/2023]
 */
#include <cmath>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

struct Engine {
  vector<tuple<int, int>> especials;
  tuple<int, int> position;
  int ID;
  int marked;
};

vector<tuple<int, int>> check_neighbors(const vector<string>& maps, int ID,
                                        int IDx, int IDy);
vector<Engine> search_engines(const vector<string>& maps);
vector<int> search_gears(const vector<Engine>& engines);

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!\n";
    return -1;
  }

  int total = 0;
  string line;
  vector<string> maps;

  while (getline(input_file, line)) {
    maps.push_back(line);
  }

  vector<Engine> engines = search_engines(maps);
  vector<int> gears = search_gears(engines);

  for (int i : gears) {
    total += i;
  }

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}

vector<tuple<int, int>> check_neighbors(const vector<string>& maps, int ID,
                                        int IDx, int IDy) {
  vector<tuple<int, int>> especials;

  // ABOVE
  if (IDy > 0) {
    for (int i = IDx; i < IDx + to_string(ID).length(); i++) {
      if (maps[IDy - 1][i] == '*') { especials.emplace_back(IDy - 1, i); }
    }
  }

  // BELOW
  if (IDy < maps.size() - 1) {
    for (int i = IDx; i < IDx + to_string(ID).length(); i++) {
      if (maps[IDy + 1][i] == '*') { especials.emplace_back(IDy + 1, i); }
    }
  }

  // LEFT
  if (IDx > 0) {
    int ymin = max(0, IDy - 1);
    int ymax = min(static_cast<int>(maps.size()) - 1, IDy + 1);
    for (int i = ymin; i <= ymax; i++) {
      if (maps[i][IDx - 1] == '*') { especials.emplace_back(i, IDx - 1); }
    }
  }

  // RIGHT
  if (IDx + to_string(ID).length() < maps[0].size()) {
    int ymin = max(0, IDy - 1);
    int ymax = min(static_cast<int>(maps.size()) - 1, IDy + 1);
    for (int i = ymin; i <= ymax; i++) {
      if (maps[i][IDx + to_string(ID).length()] == '*') {
        especials.emplace_back(i, IDx + to_string(ID).length());
      }
    }
  }

  return especials;
}

vector<Engine> search_engines(const vector<string>& maps) {
  vector<Engine> engines;

  for (int i = 0; i < maps.size(); i++) {
    int ID = 0;
    int positionx = 0;
    vector<tuple<int, int>> especials;

    for (int j = 0; j < maps[0].size(); j++) {
      if (isdigit(maps[i][j])) { ID = ID * 10 + (maps[i][j] - '0'); }

      if (j + 1 == maps[0].size() ||
          (!isdigit(maps[i][j + 1]) && isdigit(maps[i][j]))) {
        positionx = j - to_string(ID).length() + 1;
        especials = check_neighbors(maps, ID, positionx, i);

        if (!especials.empty()) {
          engines.push_back({especials, {positionx, i}, ID, -1});
        }

        ID = 0;
      }
    }
  }

  return engines;
}

vector<int> search_gears(const vector<Engine>& engines) {
  vector<int> gears;

  for (const Engine& engine : engines) {
    int engineID = engine.ID;

    for (const tuple<int, int>& neighborPosition : engine.especials) {
      int neighborID = -1;
      int neighborCount = 0;
      int neighborY = get<1>(neighborPosition);
      int neighborX = get<0>(neighborPosition);

      for (const Engine& neighborEngine : engines) {
        if (get<1>(neighborEngine.especials[0]) == neighborY &&
            get<0>(neighborEngine.especials[0]) == neighborX) {
          neighborID = neighborEngine.ID;
          neighborCount += 1;
        }
      }

      if (neighborID != -1 && neighborCount == 2 && engine.marked == -1 &&
          engineID != neighborID) {
        gears.push_back(engineID * neighborID);
        const_cast<Engine&>(engine).marked = 1;  // Mark the engine as processed
      }
    }
  }

  return gears;
}
