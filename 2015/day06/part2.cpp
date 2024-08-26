/**
 * @file part2.cpp
 * @brief Santa's Light Grid Brightness Control
 *
 * This program manages a 1000x1000 grid of lights where each light's brightness
 * can be adjusted individually. Initially, all lights have a brightness of
 * zero. The instructions provided by Santa dictate how to modify the
 * brightness:
 * - "turn on" increases the brightness of the specified lights by 1.
 * - "turn off" decreases the brightness of the specified lights by 1, with a
 *   minimum brightness of zero.
 * - "toggle" increases the brightness of the specified lights by 2.
 *
 * The task is to calculate the total brightness of all lights after processing
 * all the instructions.
 *
 * Example Instructions:
 * - "turn on 0,0 through 0,0" would increase the brightness of a single light
 *   by 1.
 * - "toggle 0,0 through 999,999" would increase the brightness of all lights in
 *   the grid by 2,000,000.
 *
 * The program processes the list of instructions and outputs the total
 * brightness of the grid after all instructions have been executed.
 *
 * Puzzle Answer: [Total Brightness]
 *
 * @author [gabrielzschmitz]
 * @date [26/08/2024]
 */
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>

bool debug = false;
const int MAX_GRID = 1000;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

int getLightsOn(const int grid[MAX_GRID][MAX_GRID]) {
  int total = 0;
  for (int i = 0; i < MAX_GRID; i++)
    for (int j = 0; j < MAX_GRID; j++)
      total += grid[i][j];
  return total;
}

void toggle(int grid[MAX_GRID][MAX_GRID], int x1, int y1, int x2, int y2) {
  for (int i = y1; i <= y2; i++)
    for (int j = x1; j <= x2; j++)
      grid[i][j] += 2;
  if (debug)
    std::cout << "Toggle lights from (" << x1 << "," << y1 << ") to (" << x2
              << "," << y2 << ")\n";
}

void turnOff(int grid[MAX_GRID][MAX_GRID], int x1, int y1, int x2, int y2) {
  for (int i = y1; i <= y2; i++)
    for (int j = x1; j <= x2; j++)
      if (grid[i][j] > 0) grid[i][j] -= 1;
  if (debug)
    std::cout << "Turn off lights from (" << x1 << "," << y1 << ") to (" << x2
              << "," << y2 << ")\n";
}

void turnOn(int grid[MAX_GRID][MAX_GRID], int x1, int y1, int x2, int y2) {
  for (int i = y1; i <= y2; i++)
    for (int j = x1; j <= x2; j++)
      grid[i][j] += 1;
  if (debug)
    std::cout << "Turn on lights from (" << x1 << "," << y1 << ") to (" << x2
              << "," << y2 << ")\n";
}

void parseAndExecute(int grid[MAX_GRID][MAX_GRID], const std::string& command) {
  std::map<std::string,
           std::function<void(int[MAX_GRID][MAX_GRID], int, int, int, int)>>
    commandMap = {
      {"toggle", toggle}, {"turn off", turnOff}, {"turn on", turnOn}};

  std::string action;
  int x1, y1, x2, y2;

  if (command.find("toggle") == 0) action = "toggle";
  else if (command.find("turn off") == 0) action = "turn off";
  else if (command.find("turn on") == 0) action = "turn on";

  std::string coordinates = command.substr(action.length());
  std::stringstream ss(coordinates);
  char ignoreChar;  // To ignore the commas and "through"

  ss >> x1 >> ignoreChar >> y1 >> std::ws;
  ss.ignore(7);
  ss >> x2 >> ignoreChar >> y2;

  commandMap[action](grid, x1, y1, x2, y2);
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  int grid[MAX_GRID][MAX_GRID];
  std::memset(grid, 0, sizeof(grid));

  std::string line;
  while (std::getline(input_file, line))
    parseAndExecute(grid, line);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = getLightsOn(grid);
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
