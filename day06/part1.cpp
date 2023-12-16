/**
 * @file part1.cpp
 * @brief Boat Races - Winning Strategies
 *
 * This program tackles the Boat Races challenge where competitors participate
 * in races with toy boats. The goal is to determine the number of ways a
 * competitor can beat the current record distance in each race by strategically
 * holding down a button to charge the boat.
 *
 * Each race has a fixed duration, and the boat's speed increases for each
 * millisecond the button is held at the start. Time spent holding the button
 * counts against the total race time, and boats move only when the button is
 * released. The task is to find the optimal holding time to maximize the
 * distance traveled.
 *
 * The challenge provides input data with race durations and the current record
 * distances. Competitors need to calculate the number of ways they can beat
 * the record in each race. The final result is obtained by multiplying these
 * values together.
 *
 * Example Input:
 * Time:      7  15   30
 * Distance:  9  40  200
 *
 * The program outputs the product of the ways to beat the record in each race,
 * providing insight into the margin of error for winning the grand prize.
 * If the input data is invalid or inaccessible, an error message is displayed.
 *
 * Competitors employ strategic button-holding techniques to optimize boat
 * performance and increase the likelihood of winning each race.
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
  int distance;
};

void printRaces(const vector<Race>& vec, const string& name) {
  cout << name << ": ";
  for (Race current : vec) {
    cout << "(" << current.time << ", " << current.distance << ") ";
  }
  cout << endl;
}

void printVector(const vector<int>& vec, const string& name) {
  cout << name << ": ";
  for (int current : vec) {
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
  char last;
  for (char c : time_line) {
    if (isdigit(c)) current_number += c;
    if (!isdigit(c) && isdigit(last)) {
      int time = stoi(current_number);
      current_number.clear();
      times.push_back(time);
    }
    last = c;
  }
  if (!current_number.empty()) {
    int time = stoi(current_number);
    times.push_back(time);
  }
  current_number.clear();
  vector<int> distances;
  last = ' ';
  for (char c : distance_line) {
    if (isdigit(c)) current_number += c;
    if (!isdigit(c) && isdigit(last)) {
      int distance = stoi(current_number);
      current_number.clear();
      distances.push_back(distance);
    }
    last = c;
  }
  // Handle the last number outside the loop
  if (!current_number.empty()) {
    int distance = stoi(current_number);
    distances.push_back(distance);
  }

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

vector<int> waysCounter(vector<Race> races) {
  vector<int> ways;
  for (Race current : races) {
    int count = 0;
    for (int i = 0; i < current.time; i++) {
      int math = i * (current.time - i);
      if (math > current.distance) count += 1;
    }
    ways.push_back(count);
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

  int total = 1;
  vector<Race> Races;
  string line;

  vector<Race> races = inputToStruct(input_file);
  input_file.close();

  vector<int> ways = waysCounter(races);
  for (int i : ways) total = total * i;

  // Display the extracted information
  printRaces(races, "Races");
  printVector(ways, "Ways");
  cout << "Total: " << total << endl;

  return 0;
}
