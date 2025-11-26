/**
 * @file part2.cpp
 * @brief Room Name Decryption and Target Identification
 *
 * This program decrypts the names of real rooms from an encrypted list using a
 * Caesar cipher rotation. Each letter in a room name is shifted forward through
 * the alphabet by a number of positions equal to the room's sector ID.
 * Dashes in the encrypted name become spaces in the decrypted name.
 *
 * The goal is to find the sector ID of the room where North Pole objects are
 * stored, identified by locating the decrypted room name containing that
 * phrase.
 *
 * Example:
 * Encrypted: qzmt-zixmtkozy-ivhz-343
 * Decrypted: very encrypted name
 *
 * Puzzle Answer: [Sector ID of North Pole objects storage room]
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

char caesarShift(char c, int shift) {
  if (c < 'a' || c > 'z')
    return c;
  return 'a' + (c - 'a' + shift) % 26;
}

std::string decryptName(const std::string &encrypted_name, int sector_id) {
  std::string decrypted;
  for (char c : encrypted_name) {
    if (c == '-')
      decrypted += ' ';
    else if (c >= 'a' && c <= 'z')
      decrypted += caesarShift(c, sector_id);
    else
      decrypted += c;
  }
  return decrypted;
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::int64_t total_sector_id_sum = 0;
  int northpole_sector_id = -1;
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
      total_sector_id_sum += sector_id;

      std::string decrypted = decryptName(encrypted_name, sector_id);

      if (debug) {
        std::cout << "Valid room: " << line << " sector ID: " << sector_id
                  << " decrypted: " << decrypted << std::endl;
      }

      std::string lowered;
      lowered.reserve(decrypted.size());
      for (char c : decrypted)
        lowered += std::tolower(c);

      if (lowered.find("northpole object") != std::string::npos) {
        northpole_sector_id = sector_id;
        if (debug) {
          std::cout << "Found North Pole objects room: sector ID " << sector_id
                    << std::endl;
        }
      }
    } else {
      if (debug)
        std::cout << "Invalid room: " << line << std::endl;
    }
  }

  std::cout << "ANSWER: " << northpole_sector_id << std::endl;

  return 0;
}
