/**
 * @file part2.cpp
 * @brief Santa's Longest Route Calculation
 *
 * This program helps Santa determine the longest route to visit all the given
 * locations during his single-night journey to deliver presents. After
 * successfully delivering all presents the previous year using the shortest
 * route, Santa now wants to show off by taking the longest possible route.
 *
 * The task is to calculate the longest possible distance Santa can travel to
 * visit all locations exactly once and return to the starting point if
 * necessary. Santa can start and end his journey at any two different
 * locations.
 *
 * Example Input:
 * London to Dublin = 464
 * London to Belfast = 518
 * Dublin to Belfast = 141
 *
 * Possible Routes:
 * Dublin -> London -> Belfast = 982
 * London -> Dublin -> Belfast = 605
 * London -> Belfast -> Dublin = 659
 * Dublin -> Belfast -> London = 659
 * Belfast -> Dublin -> London = 605
 * Belfast -> London -> Dublin = 982
 *
 * The longest of these routes is Dublin -> London -> Belfast = 982.
 *
 * The program processes the distances and outputs the longest route Santa can
 * take.
 *
 * Puzzle Answer: [Longest Distance]
 *
 * @author [gabrielzschmitz]
 * @date [29/08/2024]
 */
#include <limits.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

void parseInput(std::ifstream& input_file,
                std::map<std::string, std::map<std::string, int>>& distances) {
  std::string line;
  while (std::getline(input_file, line)) {
    size_t pos1 = line.find(" to ");
    size_t pos2 = line.find(" = ");

    std::string loc1 = line.substr(0, pos1);
    std::string loc2 = line.substr(pos1 + 4, pos2 - pos1 - 4);
    int distance = std::stoi(line.substr(pos2 + 3));

    distances[loc1][loc2] = distance;
    distances[loc2][loc1] = distance;
  }
}

int calculateRouteDistance(
  const std::vector<std::string>& route,
  const std::map<std::string, std::map<std::string, int>>& distances) {
  int totalDistance = 0;
  for (size_t i = 0; i < route.size() - 1; ++i)
    totalDistance += distances.at(route[i]).at(route[i + 1]);
  return totalDistance;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::map<std::string, std::map<std::string, int>> distances;
  parseInput(input_file, distances);

  std::vector<std::string> locations;
  for (const auto& entry : distances)
    locations.push_back(entry.first);

  int minDistance = INT_MIN;

  std::sort(locations.begin(), locations.end());
  do {
    int currentDistance = calculateRouteDistance(locations, distances);
    minDistance = std::max(minDistance, currentDistance);
  } while (std::next_permutation(locations.begin(), locations.end()));

  int total = minDistance;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
