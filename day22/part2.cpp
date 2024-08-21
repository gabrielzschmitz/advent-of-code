/**
 * @file part2.cpp
 * @brief Sand Slabs - Determine Chain Reaction Impact of Disintegrating Bricks
 *
 * Given a snapshot of bricks with their positions represented as two x,y,z
 * coordinates, determine for each brick how many other bricks would fall if
 * that brick were disintegrated. Calculate the sum of the number of other
 * bricks that would fall for each brick. Find the brick that, when
 * disintegrated, causes the maximum impact in terms of bricks falling, and
 * return the sum for that brick.
 *
 * Example Input:
 * 1,0,1~1,2,1
 * 0,0,2~2,0,2
 * 0,2,3~2,2,3
 * 0,0,4~0,2,4
 * 2,0,5~2,2,5
 * 0,1,6~2,1,6
 * 1,1,8~1,1,9
 *
 * Example Output:
 * Sum of Bricks Falling for Optimal Disintegration: 7
 *
 * Determine the impact of disintegrating each brick and find the brick that,
 * when disintegrated, causes the maximum impact in terms of bricks falling.
 *
 * @author [gabrielzschmitz]
 * @date [22/12/2023]
 */
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
bool debug = false;

struct Brick {
  int x1, y1, z1, x2, y2, z2;
};

bool overlaps(const Brick& a, const Brick& b) {
  return max(a.x1, b.x1) <= min(a.x2, b.x2) &&
         max(a.y1, b.y1) <= min(a.y2, b.y2);
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";
  if (debug) user_input = "test";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  vector<Brick> bricks;

  int x1, y1, z1, x2, y2, z2;
  char delimiter;

  while (input_file >> x1 >> delimiter >> y1 >> delimiter >> z1 >> delimiter >>
         x2 >> delimiter >> y2 >> delimiter >> z2) {
    bricks.push_back({x1, y1, z1, x2, y2, z2});
  }
  if (debug)
    for (Brick i : bricks)
      cout << "(" << i.x1 << ", " << i.y1 << ", " << i.z1 << ") (" << i.x2
           << ", " << i.y2 << ", " << i.z2 << ") " << endl;

  sort(bricks.begin(), bricks.end(),
       [](const Brick& a, const Brick& b) { return a.z1 < b.z1; });

  for (size_t index = 0; index < bricks.size(); ++index) {
    int maxZ = 1;

    for (size_t checkIndex = 0; checkIndex < index; ++checkIndex) {
      const Brick& check = bricks[checkIndex];

      if (overlaps(bricks[index], check)) maxZ = max(maxZ, check.z2 + 1);
    }

    bricks[index].z2 -= bricks[index].z1 - maxZ;
    bricks[index].z1 = maxZ;
  }

  sort(bricks.begin(), bricks.end(),
       [](const Brick& a, const Brick& b) { return a.z1 < b.z1; });

  unordered_map<size_t, unordered_set<size_t>> kSupportsV;
  unordered_map<size_t, unordered_set<size_t>> vSupportsK;

  for (size_t j = 0; j < bricks.size(); ++j) {
    for (size_t i = 0; i < j; ++i) {
      const Brick& upper = bricks[j];
      const Brick& lower = bricks[i];

      if (overlaps(lower, upper) && upper.z1 == lower.z2 + 1) {
        kSupportsV[i].insert(j);
        vSupportsK[j].insert(i);
      }
    }
  }

  int total = 0;

  for (int i = 0; i < bricks.size(); ++i) {
    deque<int> queue;

    for (int j : kSupportsV[i])
      if (vSupportsK[j].size() == 1) { queue.push_back(j); }

    set<int> falling = {i};
    for (int q : queue)
      falling.insert(q);

    while (!queue.empty()) {
      int j = queue.front();
      queue.pop_front();

      for (int k : kSupportsV[j]) {
        if (falling.find(k) == falling.end()) {
          bool isSubset = true;
          for (int vk : vSupportsK[k]) {
            if (falling.find(vk) == falling.end()) {
              isSubset = false;
              break;
            }
          }
          if (isSubset) {
            queue.push_back(k);
            falling.insert(k);
          }
          isSubset = true;
        }
      }
    }
    total += falling.size() - 1;
  }

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
