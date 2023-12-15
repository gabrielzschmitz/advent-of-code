/*
 * @file part2.cpp
 * @brief Parabolic Reflector Dish - Calculate Total Load on North Support Beams
 * After Spin Cycles
 *
 * Spin the platform through multiple cycles to move the rounded rocks to the
 * edges. Calculate the total load on the north support beams after 1000000000
 * cycles.
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
 * Total Load on North Support Beams After 1000000000 Cycles: 64
 *
 * Run the spin cycle for 1000000000 cycles, and calculate the load on the north
 * support beams based on the final position of the rounded rocks after the
 * specified number of cycles. Sum up the total load to obtain the final result.
 *
 * @author [gabrielzschmitz]
 * @date [14/12/2023]
 */
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <unordered_map>
using namespace std;

bool debug = false;

struct Dish {
  string map;
  int width = 0;
  int height = 0;

  char& get(int x, int y) { return map[y * width + x]; }
};

struct DishHash {
  size_t operator()(const Dish& dish) const { return hash<string>()(dish.map); }
};

struct DishLoad {
  size_t idx;
  size_t load;
};

enum Direction { North = 0, West, South, East };

bool operator==(const Dish& a, const Dish& b) { return a.map == b.map; }

using Cache = unordered_map<Dish, DishLoad, DishHash>;

Dish loadInput(const string& file) {
  Dish result = {};
  ifstream fs(file);
  if (!fs.is_open()) {
    cout << "ARQUIVO " << file << " INACESSIVEL!" << endl;
    return result;
  }
  string line;

  while (getline(fs, line)) {
    result.map += line;
    result.width = static_cast<int>(line.size());
    result.height++;
  }

  return result;
}

void tilt(Dish& dish, Direction dir) {
  if (dir == North) {
    for (int y = 0; y < dish.height; ++y) {
      for (int x = 0; x < dish.width; ++x) {
        if (dish.get(x, y) == 'O') {
          int startY = y - 1;
          int newY = startY;
          while (newY >= 0 && dish.get(x, newY) == '.') {
            newY--;
          }

          if (newY != startY) {  // we've moved
            dish.get(x, y) = '.';
            dish.get(x, newY + 1) = 'O';
          }
        }
      }
    }
  } else if (dir == West) {
    for (int x = 0; x < dish.width; ++x) {
      for (int y = 0; y < dish.height; ++y) {
        if (dish.get(x, y) == 'O') {
          int startX = x - 1;
          int newX = startX;
          while (newX >= 0 && dish.get(newX, y) == '.') {
            newX--;
          }

          if (newX != startX) {  // we've moved
            dish.get(x, y) = '.';
            dish.get(newX + 1, y) = 'O';
          }
        }
      }
    }
  } else if (dir == South) {
    for (int y = dish.height - 1; y >= 0; --y) {
      for (int x = 0; x < dish.width; ++x) {
        if (dish.get(x, y) == 'O') {
          int startY = y + 1;
          int newY = startY;
          while (newY < dish.height && dish.get(x, newY) == '.') {
            newY++;
          }

          if (newY != startY) {  // we've moved
            dish.get(x, y) = '.';
            dish.get(x, newY - 1) = 'O';
          }
        }
      }
    }
  } else if (dir == East) {
    for (int x = dish.width - 1; x >= 0; --x) {
      for (int y = 0; y < dish.height; ++y) {
        if (dish.get(x, y) == 'O') {
          int startX = x + 1;
          int newX = startX;
          while (newX < dish.width && dish.get(newX, y) == '.') {
            newX++;
          }

          if (newX != startX) {  // we've moved
            dish.get(x, y) = '.';
            dish.get(newX - 1, y) = 'O';
          }
        }
      }
    }
  }
}

size_t calculateLoad(Dish& dish) {
  size_t sum = 0;
  for (int y = 0; y < dish.height; ++y) {
    for (int x = 0; x < dish.width; ++x) {
      if (dish.get(x, y) == 'O') {
        sum += dish.height - y;
      }
    }
  }
  return sum;
}

size_t getIndex(size_t a, size_t b) {
  return (((4000000000 - a) % (b - a)) + a) - 1;
}

size_t summary(Dish dish) {
  Cache cache;

  size_t i = 0;
  while (true) {
    tilt(dish, Direction(i % 4));
    if (cache.count(dish)) {
      break;  // oh we got a cycle!
    }
    cache[dish] = {i, calculateLoad(dish)};
    i++;
  }

  size_t idx = getIndex(cache[dish].idx, i);
  auto it = find_if(
      cache.begin(), cache.end(),
      [&](const pair<Dish, DishLoad>& c) { return c.second.idx == idx; });
  return it->second.load;
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";
  if (debug) {
    user_input = "test";
    Dish testValues = loadInput(user_input);
    cout << "part2: " << summary(testValues) << endl;
    return 0;
  }

  Dish actualValues = loadInput(user_input);
  cout << "part2: " << summary(actualValues) << endl;

  return 0;
}
