/**
 * @file part2.cpp
 * @brief Reindeer Olympics: Scoring System Update
 *
 * This program simulates a race between Santa's reindeer during the Reindeer
 * Olympics, with a new scoring system implemented by Santa. Instead of
 * determining the winner by distance traveled, points are awarded at the end of
 * each second to the reindeer that are currently in the lead. If multiple
 * reindeer are tied for the lead, each receives a point.
 *
 * The program tracks the position of each reindeer throughout the race duration
 * and calculates the points each reindeer accumulates over the race period. The
 * goal is to determine which reindeer has the most points after the race ends.
 *
 * Example Input:
 * - Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
 * - Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds.
 *
 * Example Output:
 * - After 1000 seconds, Dancer has accumulated 689 points, while Comet has 312
 *   points. Dancer wins.
 *
 * Puzzle Answer: [Winning Reindeer's Points]
 *
 * @author [gabrielzschmitz]
 * @date [03/09/2024]
 */
#include <fstream>
#include <iostream>
#include <map>
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

const int TIME = 2503;

std::string getReindeerName(const std::string& input) {
  std::stringstream ss(input);
  std::string name;
  ss >> name;
  return name;
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

std::vector<int> getReindeerDistanceOverTime(
  const std::tuple<int, int, int>& reindeer, int total_time) {
  int speed = std::get<0>(reindeer);
  int fly_time = std::get<1>(reindeer);
  int rest_time = std::get<2>(reindeer);

  int cycle_time = fly_time + rest_time;
  std::vector<int> distances(total_time + 1, 0);

  int current_distance = 0;
  for (int current_time = 1; current_time <= total_time; ++current_time) {
    bool is_moving = (current_time % cycle_time <= fly_time) &&
                     (current_time % cycle_time != 0);
    if (is_moving) current_distance += speed;
    distances[current_time] = current_distance;
  }

  return distances;
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
  std::map<std::string, std::tuple<int, int, int>> reindeers;
  std::map<std::string, std::vector<int>> allTraveledDistances;
  std::map<std::string, int> reindeerPoints;

  while (std::getline(input_file, line)) {
    std::string name = getReindeerName(line);
    auto reindeer_tuple = parseInput(line);
    reindeers[name] = reindeer_tuple;
    allTraveledDistances[name] =
      getReindeerDistanceOverTime(reindeer_tuple, TIME);
    reindeerPoints[name] = 0;
  }

  if (debug)
    for (const auto& reindeer : reindeers)
      std::cout << reindeer.first << " -> " << std::get<0>(reindeer.second)
                << "km/s " << std::get<1>(reindeer.second) << "s "
                << std::get<2>(reindeer.second) << "s" << std::endl;

  for (int current_time = 1; current_time <= TIME; ++current_time) {
    int max_distance = 0;

    for (const auto& reindeer : allTraveledDistances)
      max_distance = std::max(max_distance, reindeer.second[current_time]);

    for (auto& reindeer : reindeerPoints)
      if (allTraveledDistances[reindeer.first][current_time] == max_distance)
        reindeer.second++;
  }

  int max_points = 0;
  for (const auto& reindeer : reindeerPoints)
    max_points = std::max(max_points, reindeer.second);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = max_points;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
