/**
 * @file part1.cpp
 * @brief Room Validation and Sector ID Summation
 *
 * This program processes a list of encrypted room names, each consisting of
 * lowercase letters separated by dashes, followed by a sector ID and a
 * checksum.
 *
 * The task is to identify real rooms by validating the checksum against the
 * encrypted name. A room is considered real if its checksum matches the five
 * most common letters in the name (excluding dashes), ordered by frequency
 * and alphabetically to break ties.
 *
 * For example:
 * - "aaaaa-bbb-z-y-x-123[abxyz]" is a real room.
 * - "a-b-c-d-e-f-g-h-987[abcde]" is a real room.
 * - "not-a-real-room-404[oarel]" is a real room.
 * - "totally-real-room-200[decoy]" is a decoy.
 *
 * The program sums the sector IDs of all real rooms from the input.
 *
 * Puzzle Answer: [Sum of sector IDs of real rooms]
 *
 * @author [gabrielzschmitz]
 * @date [26/11/2025]
 */
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test")
    debug = true;
}

bool cmp(const std::pair<char, int> &a, const std::pair<char, int> &b) {
  if (a.second != b.second)
    return a.second > b.second;
  return a.first < b.first;
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::int64_t total = 0;
  std::string line;

  while (std::getline(input_file, line)) {
    size_t last_dash = line.find_last_of('-');
    if (last_dash == std::string::npos)
      continue;

    std::string id_checksum = line.substr(last_dash + 1);

    size_t bracket_pos = id_checksum.find('[');
    if (bracket_pos == std::string::npos)
      continue;

    std::string sector_id_str = id_checksum.substr(0, bracket_pos);
    int sector_id = std::stoi(sector_id_str);

    std::string checksum = id_checksum.substr(bracket_pos + 1, 5);

    std::string encrypted_name = line.substr(0, last_dash);

    std::map<char, int> freq;
    for (char c : encrypted_name)
      if (c >= 'a' && c <= 'z')
        freq[c]++;

    std::vector<std::pair<char, int>> freq_vec(freq.begin(), freq.end());

    std::sort(freq_vec.begin(), freq_vec.end(), cmp);

    std::string computed_checksum;
    for (size_t i = 0; i < 5 && i < freq_vec.size(); i++)
      computed_checksum += freq_vec[i].first;

    if (computed_checksum == checksum) {
      total += sector_id;
      if (debug)
        std::cout << "Valid room: " << line << " sector ID: " << sector_id
                  << std::endl;
    } else {
      if (debug)
        std::cout << "Invalid room: " << line << std::endl;
    }
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
