/**
 * @file part1.cpp
 * @brief Reindeer Olympics Race Simulation
 *
 * This program simulates a race between Santa's reindeer during the Reindeer
 * Olympics. Each reindeer can either be flying at a constant speed or resting
 * to recover energy. The goal is to determine which reindeer travels the
 * farthest after a set duration.
 *
 * The program uses the following input parameters for each reindeer:
 * - Flight speed in kilometers per second.
 * - Duration in seconds that the reindeer can maintain its flight speed before
 *   needing to rest.
 * - Rest time in seconds required before the reindeer can resume flying.
 *
 * The task is to calculate the distance traveled by each reindeer after a fixed
 * race time, and determine which reindeer traveled the farthest.
 *
 * Example Input:
 * - Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
 * - Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds.
 *
 * Example Output:
 * - After 1000 seconds, Comet travels 1120 km, and Dancer travels 1056 km.
 *   Comet wins.
 *
 * Puzzle Answer: [Winning Reindeer's Distance]
 *
 * @author [gabrielzschmitz]
 * @date [03/09/2024]
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

std::tuple<int, int, int> parseInput(std::string input) {
  int speed, fly_time, rest_time;
  std::stringstream ss(input);
  std::string temp;

  // Skip the first part "Reindeer can fly"
  ss >> temp >> temp >> temp;
  // Extract the speed
  ss >> speed;
  // Skip "km/s for"
  ss >> temp >> temp;
  // Extract the fly_time
  ss >> fly_time;
  // Skip "seconds, but then must rest for"
  ss >> temp >> temp >> temp >> temp >> temp >> temp;
  // Extract the rest_time
  ss >> rest_time;

  std::tuple<int, int, int> current =
    std::make_tuple(speed, fly_time, rest_time);

  return current;
}

int getReindeerDistance(const std::tuple<int, int, int>& reindeer,
                        int total_time) {
  int speed = std::get<0>(reindeer);
  int fly_time = std::get<1>(reindeer);
  int rest_time = std::get<2>(reindeer);

  int cycle_time = fly_time + rest_time;
  int full_cycles = total_time / cycle_time;
  int remaining_time = total_time % cycle_time;

  int distance = full_cycles * (speed * fly_time);
  if (remaining_time >= fly_time) distance += speed * fly_time;
  else distance += speed * remaining_time;

  return distance;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string line;
  std::vector<std::tuple<int, int, int>> reindeers;
  while (std::getline(input_file, line)) {
    std::tuple<int, int, int> current = parseInput(line);
    reindeers.push_back(current);
  }
  if (debug)
    for (const auto& reindeer : reindeers)
      std::cout << std::get<0>(reindeer) << "km/s " << std::get<1>(reindeer)
                << "s " << std::get<2>(reindeer) << "s" << std::endl;

  int max_distance = 0;
  for (const auto& reindeer : reindeers)
    max_distance = std::max(getReindeerDistance(reindeer, 2503), max_distance);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = max_distance;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
