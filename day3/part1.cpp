/**
 * @file part1.cpp
 * @brief Engine Calculator
 *
 * This program addresses the "Gear Ratios" challenge, where you encounter an
 * Elf engineer at a gondola lift station facing issues with the gondola lift's
 * engine. The task is to identify the missing engine part by summing up all the
 * part numbers adjacent to symbols in the engine schematic.
 *
 * The engine schematic is represented by a grid of characters. Each digit
 * adjacent to a symbol (excluding periods) is considered a part number. The
 * program calculates the sum of all part numbers for the identified engines,
 * ensuring that only valid engines with at least one neighboring special
 * character contribute to the total.
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
 * The program outputs the sum of part numbers for valid engines.
 * In case the input file is inaccessible, an error message is displayed.
 *
 * The program utilizes a struct 'Engine' to store information about each
 * engine, including its ID, position, and neighboring special characters. It
 * uses vectors to store the identified engines and calculates the total sum of
 * part numbers for valid engines.
 *
 * @author [gabrielzschmitz]
 * @date [03/12/2023]
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <tuple>
using namespace std;

struct Engine {
  vector<tuple<int, int>> especials;
  tuple<int, int> position;
  int ID;
};

vector<tuple<int, int>> check_neighbors(const vector<string>& maps, int ID,
                                        int IDx, int IDy);
vector<Engine> search_engines(const vector<string>& maps);

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

  for (const Engine& engine : engines) {
    total += engine.ID;
  }

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}

vector<tuple<int, int>> check_neighbors(const vector<string>& maps, int ID,
                                        int IDx, int IDy) {
  vector<tuple<int, int>> especials;
  int IDsize = to_string(ID).length();

  // ABOVE
  if (IDy > 0) {
    for (int i = IDx; i < IDx + IDsize; i++) {
      if (!isdigit(maps[IDy - 1][i]) && maps[IDy - 1][i] != '.') {
        especials.emplace_back(IDy - 1, i);
      }
    }
  }

  // BELOW
  if (IDy < maps.size() - 1) {
    for (int i = IDx; i < IDx + IDsize; i++) {
      if (!isdigit(maps[IDy + 1][i]) && maps[IDy + 1][i] != '.') {
        especials.emplace_back(IDy + 1, i);
      }
    }
  }

  // LEFT
  if (IDx > 0) {
    for (int i = max(0, IDy - 1);
         i <= min(static_cast<int>(maps.size()) - 1, IDy + 1); i++) {
      if (!isdigit(maps[i][IDx - 1]) && maps[i][IDx - 1] != '.') {
        especials.emplace_back(i, IDx - 1);
      }
    }
  }

  // RIGHT
  if (IDx + IDsize < maps[0].size()) {
    for (int i = max(0, IDy - 1);
         i <= min(static_cast<int>(maps.size()) - 1, IDy + 1); i++) {
      if (!isdigit(maps[i][IDx + IDsize]) && maps[i][IDx + IDsize] != '.') {
        especials.emplace_back(i, IDx + IDsize);
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
      if (isdigit(maps[i][j])) {
        ID = ID * 10 + (maps[i][j] - '0');
      }

      if (j + 1 == maps[0].size() ||
          (!isdigit(maps[i][j + 1]) && isdigit(maps[i][j]))) {
        positionx = j - to_string(ID).length() + 1;
        especials = check_neighbors(maps, ID, positionx, i);

        if (!especials.empty()) {
          engines.push_back({especials, {positionx, i}, ID});
        }

        ID = 0;
      }
    }
  }

  return engines;
}
