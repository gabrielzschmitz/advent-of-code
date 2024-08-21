/**
 * @file part1.cpp
 * @brief The Floor Will Be Lava - Maximize Energized Tiles
 *
 * Analyze a flat, two-dimensional square grid containing mirrors, splitters,
 * and empty space. Simulate the behavior of a beam of light starting from any
 * edge tile and heading away from that edge. Find the initial beam
 * configuration that energizes the largest number of tiles.
 *
 * Example Input:
 * .|...\....
 * |.-.\.....
 * .....|-...
 * ........|.
 * ..........
 * .........\
 * ..../.\\..
 * .-.-/..|..
 * .|....-|.\
 * ..//.|....
 *
 * Example Output:
 * Maximized Energized Tiles: 51
 *
 * Analyze the contraption with different initial beam configurations and find
 * the one that energizes the largest number of tiles. Report the number of
 * energized tiles in that configuration.
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
  vector<tuple<int, int, int, int>> a = {{0, -1, 0, 1}};
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
      for (auto direction :
           (ch == '|' ? vector<tuple<int, int>>{{1, 0}, {-1, 0}}
                      : vector<tuple<int, int>>{{0, 1}, {0, -1}})) {
        int new_dr, new_dc;
        tie(new_dr, new_dc) = direction;
        auto current_state = make_tuple(r, c, new_dr, new_dc);
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

  int total = reachablePoints(0, -1, 0, 1, grid);
  cout << "NUMBER:" << total << "\n";

  return 0;
}
