/**
 * @file part1.cpp
 * @brief Boolean Logic Gate Simulator
 *
 * **Puzzle Answer:** [Decimal Output]
 *
 * @author [gabrielzschmitz]
 * @date [24/12/2024]
 */
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

// Function to parse a schematic and convert it to a vector of heights
std::vector<int> parseHeights(const std::vector<std::string>& schematic,
                              bool isLock) {
  size_t width = schematic[0].size();
  std::vector<int> heights(width, 0);

  for (size_t col = 0; col < width; ++col) {
    for (size_t row = 0; row < schematic.size(); ++row) {
      if (isLock) {
        if (schematic[row][col] == '#') {
          heights[col] = row + 1;
          break;
        }
      } else {
        size_t actualRow = schematic.size() - 1 - row;
        if (schematic[actualRow][col] == '#') {
          heights[col] = row + 1;
          break;
        }
      }
    }
  }

  return heights;
}

// Function to check if a key fits a lock
bool doesKeyFitLock(const std::vector<int>& lock, const std::vector<int>& key,
                    int maxHeight) {
  for (size_t i = 0; i < lock.size(); ++i) {
    if (lock[i] + key[i] > maxHeight)
      return false;  // Total height exceeds available space
  }
  return true;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::vector<std::vector<std::string>> lock_schematics;
  std::vector<std::vector<std::string>> key_schematics;

  std::string line;
  std::vector<std::string> current_schematic;
  bool isReadingLocks = true;

  while (std::getline(input_file, line)) {
    if (line.empty()) {
      if (!current_schematic.empty()) {
        if (isReadingLocks) {
          lock_schematics.push_back(current_schematic);
        } else {
          key_schematics.push_back(current_schematic);
        }
        current_schematic.clear();
      }
    } else if (line[0] == '.' && current_schematic.empty() && isReadingLocks) {
      isReadingLocks = false;  // Switch to keys once locks are read
    } else {
      current_schematic.push_back(line);
    }
  }
  if (!current_schematic.empty()) {
    if (isReadingLocks) {
      lock_schematics.push_back(current_schematic);
    } else {
      key_schematics.push_back(current_schematic);
    }
  }

  // Convert schematics to height arrays
  std::vector<std::vector<int>> lock_heights;
  for (const auto& schematic : lock_schematics) {
    lock_heights.push_back(parseHeights(schematic, true));
  }

  std::vector<std::vector<int>> key_heights;
  for (const auto& schematic : key_schematics) {
    key_heights.push_back(parseHeights(schematic, false));
  }

  uint64_t total = 0;
  const int maxHeight = 6;

  // Compare each lock with each key
  for (const auto& lock : lock_heights) {
    for (const auto& key : key_heights) {
      if (doesKeyFitLock(lock, key, maxHeight)) { ++total; }
    }
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
