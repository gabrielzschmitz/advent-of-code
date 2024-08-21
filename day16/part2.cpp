/**
 * @file part2.cpp
 * @brief Lens Library - Sum of Results from HASH Algorithm on Initialization
 * Steps
 *
 * Run the HASH algorithm on each step in the initialization sequence and
 * calculate the sum of the results. The HASH algorithm turns any string of
 * characters into a single number in the range 0 to 255 based on specific
 * steps.
 *
 * Example Input:
 * rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7
 *
 * Example Output:
 * Sum of Results: 1320
 *
 * Parse the initialization sequence, run the HASH algorithm on each step, and
 * sum up the results to obtain the final result.
 *
 * @author [gabrielzschmitz]
 * @date [16/12/2023]
 */
#include <deque>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

bool debug = false;

int reachablePoints(int r, int c, int dr, int dc, const vector<string>& grid) {
  vector<tuple<int, int, int, int>> a = {{r, c, dr, dc}};
  set<tuple<int, int, int, int>> seen;
  deque<tuple<int, int, int, int>> q(a.begin(), a.end());

  while (!q.empty()) {
    int r, c, dr, dc;
    tie(r, c, dr, dc) = q.front();
    q.pop_front();

    r += dr;
    c += dc;

    if (r < 0 || r >= static_cast<int>(grid.size()) || c < 0 ||
        c >= static_cast<int>(grid[0].size())) {
      continue;
    }

    char ch = grid[r][c];

    if (ch == '.' || (ch == '-' && dc != 0) || (ch == '|' && dr != 0)) {
      auto current_state = make_tuple(r, c, dr, dc);
      if (seen.find(current_state) == seen.end()) {
        seen.insert(current_state);
        q.push_back(current_state);
      }
    } else if (ch == '/') {
      tie(dr, dc) = make_tuple(-dc, -dr);
      auto current_state = make_tuple(r, c, dr, dc);
      if (seen.find(current_state) == seen.end()) {
        seen.insert(current_state);
        q.push_back(current_state);
      }
    } else if (ch == '\\') {
      tie(dr, dc) = make_tuple(dc, dr);
      auto current_state = make_tuple(r, c, dr, dc);
      if (seen.find(current_state) == seen.end()) {
        seen.insert(current_state);
        q.push_back(current_state);
      }
    } else {
      for (const auto& new_direction :
           (ch == '|' ? std::vector<std::tuple<int, int>>{{1, 0}, {-1, 0}}
                      : std::vector<std::tuple<int, int>>{{0, 1}, {0, -1}})) {
        int new_dr, new_dc;
        std::tie(new_dr, new_dc) = new_direction;

        auto current_state = std::make_tuple(r, c, new_dr, new_dc);
        if (seen.find(current_state) == seen.end()) {
          seen.insert(current_state);
          q.push_back(current_state);
        }
      }
    }
  }

  set<tuple<int, int>> coords;
  for (const auto& state : seen) {
    int r, c, _, __;
    tie(r, c, _, __) = state;
    coords.insert(make_tuple(r, c));
  }

  return coords.size();
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";
  if (debug) user_input = "test";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  vector<string> grid;
  string line;
  while (getline(input_file, line)) {
    grid.push_back(line);
  }

  int total = 0;

  for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
    total = max(total, reachablePoints(r, -1, 0, 1, grid));
    total = max(
      total, reachablePoints(r, static_cast<int>(grid[0].size()), 0, -1, grid));
  }

  for (int c = 0; c < static_cast<int>(grid[0].size()); ++c) {
    total = max(total, reachablePoints(-1, c, 1, 0, grid));
    total = max(total,
                reachablePoints(static_cast<int>(grid.size()), c, -1, 0, grid));
  }

  cout << "NUMBER:" << total << "\n";

  return 0;
}
