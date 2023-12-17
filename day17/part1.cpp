/**
 * @file part1.cpp
 * @brief Clumsy Crucible - Minimize Heat Loss
 *
 * Determine the least heat loss incurred by directing a top-heavy crucible from
 * the lava pool (starting point) to the machine parts factory (destination) on
 * Gear Island. The crucible can move at most three blocks in a single direction
 * before turning 90 degrees left or right. Minimize heat loss by choosing a
 * route that doesn't require the crucible to go in a straight line for too
 * long.
 *
 * Example Input:
 * 2413432311323
 * 3215453535623
 * 3255245654254
 * 3446585845452
 * 4546657867536
 * 1438598798454
 * 4457876987766
 * 3637877979653
 * 4654967986887
 * 4564679986453
 * 1224686865563
 * 2546548887735
 * 4322674655533
 *
 * Example Output:
 * Least Heat Loss: 102
 *
 * Analyze the given map to find the least heat loss incurred by directing the
 * crucible from the lava pool to the machine parts factory. The crucible can
 * move at most three blocks in a single direction before turning 90 degrees
 * left or right.
 *
 * @author [gabrielzschmitz]
 * @date [17/12/2023]
 */
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
using namespace std;

bool debug = false;

struct State {
  int heatLoss, row, column, directionRow, directionColumn, steps;

  bool operator>(const State& other) const { return heatLoss > other.heatLoss; }
};

int minHeatLoss(const vector<vector<int>>& heatMap) {
  int rows = heatMap.size();
  int columns = heatMap[0].size();

  set<tuple<int, int, int, int, int>> seen;
  priority_queue<State, vector<State>, greater<State>> priorityQueue;
  priorityQueue.push({0, 0, 0, 0, 0, 0});

  while (!priorityQueue.empty()) {
    State current = priorityQueue.top();
    priorityQueue.pop();

    int currentHeatLoss = current.heatLoss;
    int currentRow = current.row;
    int currentColumn = current.column;
    int currentDirectionRow = current.directionRow;
    int currentDirectionColumn = current.directionColumn;
    int currentStep = current.steps;

    if (currentRow == rows - 1 && currentColumn == columns - 1) {
      return currentHeatLoss;
    }

    if (seen.count({currentRow, currentColumn, currentDirectionRow,
                    currentDirectionColumn, currentStep})) {
      continue;
    }

    seen.insert({currentRow, currentColumn, currentDirectionRow,
                 currentDirectionColumn, currentStep});

    if (currentStep < 3 &&
        (currentDirectionRow != 0 || currentDirectionColumn != 0)) {
      int nRows = currentRow + currentDirectionRow;
      int nColumns = currentColumn + currentDirectionColumn;
      if (0 <= nRows && nRows < rows && 0 <= nColumns && nColumns < columns) {
        priorityQueue.push({currentHeatLoss + heatMap[nRows][nColumns], nRows,
                            nColumns, currentDirectionRow,
                            currentDirectionColumn, currentStep + 1});
      }
    }

    for (const auto& direction :
         vector<pair<int, int>>{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}) {
      int nDirectionRows = direction.first;
      int nDirectionColumns = direction.second;
      if ((nDirectionRows != currentDirectionRow ||
           nDirectionColumns != currentDirectionColumn) &&
          (nDirectionRows != -currentDirectionRow ||
           nDirectionColumns != -currentDirectionColumn)) {
        int nRows = currentRow + nDirectionRows;
        int nColumns = currentColumn + nDirectionColumns;
        if (0 <= nRows && nRows < rows && 0 <= nColumns && nColumns < columns) {
          priorityQueue.push({currentHeatLoss + heatMap[nRows][nColumns], nRows,
                              nColumns, nDirectionRows, nDirectionColumns, 1});
        }
      }
    }
  }

  return -1;
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

  string line;
  vector<vector<int>> heatMap;
  while (getline(input_file, line)) {
    vector<int> row;
    for (char i : line) row.push_back(i - '0');
    heatMap.push_back(row);
  }
  if (debug) {
    for (vector<int> i : heatMap) {
      for (int j : i) cout << j << ", ";
      cout << endl;
    }
  }

  total = minHeatLoss(heatMap);

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
