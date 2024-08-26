/**
 * @file part1.cpp
 * @brief Holiday Light Grid Control
 *
 * This program helps control a 1000x1000 grid of holiday lights based on
 * Santa's instructions. The grid contains one million lights, all initially
 * turned off, and each light can be either on or off. The instructions dictate
 * whether to turn on, turn off, or toggle specific rectangular regions of the
 * grid.
 *
 * The task is to process the instructions and determine the final number of
 * lights that are turned on.
 *
 * Example Instructions:
 * - "turn on 0,0 through 999,999" would turn on all lights in the entire grid.
 * - "toggle 0,0 through 999,0" would toggle the first row of 1000 lights,
 *   turning off the ones that were on and turning on the ones that were off.
 * - "turn off 499,499 through 500,500" would turn off the four lights in the
 *   middle of the grid.
 *
 * The program processes the list of instructions and outputs the total number
 * of lights that are lit after all instructions have been executed.
 *
 * Puzzle Answer: [Number of Lit Lights]
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

int getLightsOn(const bool grid[MAX_GRID][MAX_GRID]) {
  int total = 0;
  for (int i = 0; i < MAX_GRID; i++)
    for (int j = 0; j < MAX_GRID; j++)
      if (grid[i][j] == true) total++;
  return total;
}

void toggle(bool grid[MAX_GRID][MAX_GRID], int x1, int y1, int x2, int y2) {
  for (int i = y1; i <= y2; i++)
    for (int j = x1; j <= x2; j++)
      grid[i][j] = !grid[i][j];
  if (debug)
    std::cout << "Toggle lights from (" << x1 << "," << y1 << ") to (" << x2
              << "," << y2 << ")\n";
}

void turnOff(bool grid[MAX_GRID][MAX_GRID], int x1, int y1, int x2, int y2) {
  for (int i = y1; i <= y2; i++)
    for (int j = x1; j <= x2; j++)
      grid[i][j] = false;
  if (debug)
    std::cout << "Turn off lights from (" << x1 << "," << y1 << ") to (" << x2
              << "," << y2 << ")\n";
}

void turnOn(bool grid[MAX_GRID][MAX_GRID], int x1, int y1, int x2, int y2) {
  for (int i = y1; i <= y2; i++)
    for (int j = x1; j <= x2; j++)
      grid[i][j] = true;
  if (debug)
    std::cout << "Turn on lights from (" << x1 << "," << y1 << ") to (" << x2
              << "," << y2 << ")\n";
}

void parseAndExecute(bool grid[MAX_GRID][MAX_GRID],
                     const std::string& command) {
  std::map<std::string,
           std::function<void(bool[MAX_GRID][MAX_GRID], int, int, int, int)>>
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

  bool grid[MAX_GRID][MAX_GRID];
  std::memset(grid, false, sizeof(grid));

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
