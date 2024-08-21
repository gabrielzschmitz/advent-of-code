/**
 * @file part1.cpp
 * @brief Parabolic Reflector Dish - Calculate Total Load on North Support Beams
 *
 * Analyze a massive parabolic reflector dish with mirrors and a control
 * platform. Tilt the platform in a specific direction to make rounded rocks
 * roll north. Calculate the total load on the north support beams caused by the
 * rolled rocks.
 *
 * Example Input:
 * O....#....
 * O.OO#....#
 * .....##...
 * OO.#O....O
 * .O.....O#.
 * O.#..O.#.#
 * ..O..#O..O
 * .......O..
 * #....###..
 * #OO..#....
 *
 * Example Output:
 * Total Load on North Support Beams: 136
 *
 * Tilt the platform to make rounded rocks roll north and calculate the load
 * on the north support beams based on the number of rows each rock traverses.
 * Sum up the total load to obtain the final result.
 *
 * @author [gabrielzschmitz]
 * @date [14/12/2023]
 */
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

bool debug = false;

int processColumn(const vector<string>& dish, int column) {
  int n = dish.size();
  int answer = 0;
  int row = 0;

  while (row < n) {
    while (row < n && dish[row][column] == '#')
      row += 1;

    int start = row;
    int count = 0;
    while (row < n && dish[row][column] != '#') {
      if (dish[row][column] == 'O') count += 1;
      row += 1;
    }

    if (debug)
      cout << "Column " << column << " Range from " << start << " to " << row
           << "; Count: " << count << endl;

    for (int i = start; i < start + count; i++)
      answer += n - i;
  }
  if (debug) cout << "Values from Column " << column << " " << answer << endl;
  return answer;
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
  vector<string> dish;
  while (getline(input_file, line)) {
    dish.push_back(line);
  }
  if (debug)
    for (string i : dish)
      cout << i << endl;

  int m = dish[0].size();

  for (int column = 0; column < m; column++)
    total += processColumn(dish, column);

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
