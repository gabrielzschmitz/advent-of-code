/**
 * @file part2.cpp
 * @brief Expanded Seed to Location Mapper
 *
 * This program extends the functionality of the Seed to Location Mapper,
 * incorporating additional features from an almanac that includes seed
 * ranges and conversion maps for categories like soil, fertilizer, water,
 * light, temperature, humidity, and location.
 *
 * The almanac specifies seed ranges using pairs of values. Each pair consists
 * of the starting seed number and the length of the range. The program processes
 * these seed ranges to consider a total of 27 seed numbers instead of just the
 * individual seed numbers.
 *
 * It explores conversion maps with three numbers each: the destination range
 * start, the source range start, and the range length. The program traverses
 * these conversion maps to determine the lowest location number for each initial
 * seed, taking into account intermediate categories such as soil, fertilizer, etc.
 *
 * Example Input:
 * seeds: 79 14 55 13
 * seed-to-soil map:
 * 50 98 2
 * 52 50 48
 * soil-to-fertilizer map:
 * 0 15 37
 * 37 52 2
 * 39 0 15
 * fertilizer-to-water map:
 * 49 53 8
 * 0 11 42
 * 42 0 7
 * water-to-light map:
 * 88 18 7
 * 18 25 70
 * light-to-temperature map:
 * 45 77 23
 * 81 45 19
 * 68 64 13
 * temperature-to-humidity map:
 * 0 69 1
 * 1 0 69
 * humidity-to-location map:
 * 60 56 37
 * 56 93 4
 *
 * The program outputs the lowest location number corresponding to any
 * of the initial seed numbers within the specified seed ranges.
 * If the input file is inaccessible or the almanac format is incorrect,
 * an error message is displayed.
 *
 * The program utilizes seed ranges and conversion maps to map seed numbers to
 * location numbers, considering intermediate categories, and finds the lowest
 * corresponding location within the specified seed ranges.
 *
 * @author [gabrielzschmitz]
 * @date [05/12/2023]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
using namespace std;

struct Input {
  vector<pair<uint32_t, uint32_t>> seed_ranges;
  vector<tuple<uint32_t, uint32_t, uint32_t>> seed_to_soil;
  vector<tuple<uint32_t, uint32_t, uint32_t>> soil_to_fertilizer;
  vector<tuple<uint32_t, uint32_t, uint32_t>> fertilizer_to_water;
  vector<tuple<uint32_t, uint32_t, uint32_t>> water_to_light;
  vector<tuple<uint32_t, uint32_t, uint32_t>> light_to_temperature;
  vector<tuple<uint32_t, uint32_t, uint32_t>> temperature_to_humidity;
  vector<tuple<uint32_t, uint32_t, uint32_t>> humidity_to_location;
};

void printVector(const vector<uint32_t>& vec, const string& name) {
  cout << name << ": ";
  for (uint32_t val : vec) {
    cout << val << " ";
  }
  cout << endl;
  cout << endl;
}

void printTupleVector(const vector<tuple<uint32_t, uint32_t, uint32_t>>& vec,
                      const string& name) {
  cout << name << ":\n";
  for (auto& tpl : vec) {
    cout << get<0>(tpl) << " " << get<1>(tpl) << " " << get<2>(tpl) << "\n";
  }
  cout << endl;
}

int main() {
  ifstream input_file("input");  // Replace with the actual input file name
  if (!input_file.is_open()) {
    cout << "Failed to open the input file!" << endl;
    return -1;
  }

  Input input;

  string line;
  int section = -1;
  input.seed_ranges = {
    make_pair(304740406, 53203352),   make_pair(1080760686, 52608146),
    make_pair(1670978447, 367043978), make_pair(1445830299, 58442414),
    make_pair(4012995194, 104364808), make_pair(4123691336, 167638723),
    make_pair(2284615844, 178205532), make_pair(3164519436, 564398605),
    make_pair(90744016, 147784453),   make_pair(577905361, 122056749)};
  while (getline(input_file, line)) {
    if (line.empty()) {
      section = -1;  // Reset section when encountering an empty line
      continue;
    }

    istringstream iss(line);
    uint32_t dest, src_start, length;

    if (line.find("seeds:") != string::npos) {
      section = 0;  // Seeds section
    } else if (line.find("seed-to-soil map:") != string::npos) {
      section = 1;  // Seed-to-soil map section
    } else if (line.find("soil-to-fertilizer map:") != string::npos) {
      section = 2;  // Soil-to-fertilizer map section
    } else if (line.find("fertilizer-to-water map:") != string::npos) {
      section = 3;  // Fertilizer-to-water map section
    } else if (line.find("water-to-light map:") != string::npos) {
      section = 4;  // Water-to-light map section
    } else if (line.find("light-to-temperature map:") != string::npos) {
      section = 5;  // Light-to-temperature map section
    } else if (line.find("temperature-to-humidity map:") != string::npos) {
      section = 6;  // Temperature-to-humidity map section
    } else if (line.find("humidity-to-location map:") != string::npos) {
      section = 7;  // Humidity-to-location map section
    } else {
      iss >> dest >> src_start >> length;

      switch (section) {
        case 1:
          input.seed_to_soil.push_back(make_tuple(dest, src_start, length));
          break;
        case 2:
          input.soil_to_fertilizer.push_back(
            make_tuple(dest, src_start, length));
          break;
        case 3:
          input.fertilizer_to_water.push_back(
            make_tuple(dest, src_start, length));
          break;
        case 4:
          input.water_to_light.push_back(make_tuple(dest, src_start, length));
          break;
        case 5:
          input.light_to_temperature.push_back(
            make_tuple(dest, src_start, length));
          break;
        case 6:
          input.temperature_to_humidity.push_back(
            make_tuple(dest, src_start, length));
          break;
        case 7:
          input.humidity_to_location.push_back(
            make_tuple(dest, src_start, length));
          break;
      }
    }
  }

  input_file.close();

  // Find the lowest location number that corresponds to any of the initial seed
  // numbers
  uint32_t min_location = UINT32_MAX;

  // Iterate through seed ranges
  for (auto& seed_range : input.seed_ranges) {
    uint32_t seed_start = seed_range.first;
    uint32_t seed_end = seed_start + seed_range.second;

    // Iterate through seeds in the range
    for (uint32_t seed = seed_start; seed < seed_end; ++seed) {
      uint32_t soil = seed;

      // Convert seed to soil
      for (auto& mapping : input.seed_to_soil) {
        if (seed >= get<1>(mapping) &&
            seed < get<1>(mapping) + get<2>(mapping)) {
          soil = get<0>(mapping) + (seed - get<1>(mapping));
          break;
        }
      }

      // Convert soil to fertilizer
      for (auto& mapping : input.soil_to_fertilizer) {
        if (soil >= get<1>(mapping) &&
            soil < get<1>(mapping) + get<2>(mapping)) {
          soil = get<0>(mapping) + (soil - get<1>(mapping));
          break;
        }
      }

      // Convert fertilizer to water
      for (auto& mapping : input.fertilizer_to_water) {
        if (soil >= get<1>(mapping) &&
            soil < get<1>(mapping) + get<2>(mapping)) {
          soil = get<0>(mapping) + (soil - get<1>(mapping));
          break;
        }
      }

      // Convert water to light
      for (auto& mapping : input.water_to_light) {
        if (soil >= get<1>(mapping) &&
            soil < get<1>(mapping) + get<2>(mapping)) {
          soil = get<0>(mapping) + (soil - get<1>(mapping));
          break;
        }
      }

      // Convert light to temperature
      for (auto& mapping : input.light_to_temperature) {
        if (soil >= get<1>(mapping) &&
            soil < get<1>(mapping) + get<2>(mapping)) {
          soil = get<0>(mapping) + (soil - get<1>(mapping));
          break;
        }
      }

      // Convert temperature to humidity
      for (auto& mapping : input.temperature_to_humidity) {
        if (soil >= get<1>(mapping) &&
            soil < get<1>(mapping) + get<2>(mapping)) {
          soil = get<0>(mapping) + (soil - get<1>(mapping));
          break;
        }
      }

      // Convert humidity to location
      for (auto& mapping : input.humidity_to_location) {
        if (soil >= get<1>(mapping) &&
            soil < get<1>(mapping) + get<2>(mapping)) {
          uint32_t location = get<0>(mapping) + (soil - get<1>(mapping));
          min_location = min(min_location, location);
          break;
        }
      }
    }
  }

  // Print all vectors
  printTupleVector(input.seed_to_soil, "Seed-to-soil map");
  printTupleVector(input.soil_to_fertilizer, "Soil-to-fertilizer map");
  printTupleVector(input.fertilizer_to_water, "Fertilizer-to-water map");
  printTupleVector(input.water_to_light, "Water-to-light map");
  printTupleVector(input.light_to_temperature, "Light-to-temperature map");
  printTupleVector(input.temperature_to_humidity,
                   "Temperature-to-humidity map");
  printTupleVector(input.humidity_to_location, "Humidity-to-location map");
  cout << "The lowest location number that corresponds to any of the initial "
          "seed numbers is: "
       << min_location << endl;

  return 0;
}
