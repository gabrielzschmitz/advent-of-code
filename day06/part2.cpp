/**
 * @file part2.cpp
 * @brief Boat Race - Winning Strategies
 *
 * This program addresses the Boat Races challenge where competitors engage in
 * a single race with toy boats. The objective is to determine the number of
 * ways a competitor can beat the current record distance in this extended race.
 *
 * The challenge provides input data with a single race duration and the current
 * record distance. Competitors must calculate the number of ways they can beat
 * the record in this specific race. The result is obtained by determining the
 * range of optimal button-holding times to maximize the distance traveled.
 *
 * Example Input:
 * Time:      71530
 * Distance:  940200
 *
 * The program outputs the total number of ways to beat the record in the
 * prolonged race, providing valuable insights into the potential strategies
 * for achieving victory. In case of invalid or inaccessible input data, an
 * error message is displayed.
 *
 * Competitors employ strategic button-holding techniques to optimize boat
 * performance, considering the continuous duration of the single race.
 *
 * @author [gabrielzschmitz]
 * @date [06/12/2023]
 */
#include <cctype>
#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>
#include <tuple>
#include <sstream>
#include <algorithm>

using namespace std;

struct Race {
  int time;
  uint64_t distance;
};

void printRaces(const vector<Race>& vec, const string& name) {
  cout << name << ": ";
  for (Race current : vec) {
    cout << "(" << current.time << ", " << current.distance << ") ";
  }
  cout << endl;
}

void printVector(const vector<uint64_t>& vec, const string& name) {
  cout << name << ": ";
  for (uint64_t current : vec) {
    cout << current << ", ";
  }
  cout << endl;
}

vector<Race> inputToStruct(ifstream& input_file) {
  vector<Race> races;
  string time_line, distance_line;

  getline(input_file, time_line);
  getline(input_file, distance_line);

  time_line.erase(0, 5);      // Remove "Time:"
  distance_line.erase(0, 9);  // Remove "Distance:"

  string current_number = "";
  vector<int> times;
  for (char c : time_line) {
    if (isdigit(c)) current_number += c;
  }
  int time = stoi(current_number);
  times.push_back(time);
  current_number.clear();
  vector<uint64_t> distances;
  for (char c : distance_line) {
    if (isdigit(c)) current_number += c;
  }
  uint64_t distance = stoul(current_number);
  distances.push_back(distance);

  // Make a loop to add the times and distances to the races vector
  size_t min_size = min(times.size(), distances.size());
  for (size_t i = 0; i < min_size; ++i) {
    Race currentrace;
    currentrace.time = times[i];
    currentrace.distance = distances[i];
    races.push_back(currentrace);
  }
  return races;
}

uint64_t waysCounter(vector<Race> races) {
  uint64_t ways = 0;
  for (Race current : races) {
    uint64_t count = 0;
    for (uint64_t i = 0; i < current.time; i++) {
      uint64_t math = i * (current.time - i);
      if (math > current.distance) count += 1;
    }
    ways = count;
  }
  return ways;
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  uint64_t total = 1;
  vector<Race> Races;
  string line;

  vector<Race> races = inputToStruct(input_file);
  input_file.close();

  uint64_t ways = waysCounter(races);
  total = ways;

  // Display the extracted information
  printRaces(races, "Races");
  // printVector(ways, "Ways");
  cout << "Total: " << total << endl;

  return 0;
}
