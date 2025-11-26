/**
 * @file part2.cpp
 * @brief Position-Based MD5 Password Decryption
 *
 * After opening the first security door, you encounter a second door that uses
 * a more elaborate password generation method. As before, the password is
 * discovered by hashing the Door ID (your puzzle input) combined with an
 * increasing integer index. However, this mechanism determines not only the
 * character but also **its position** within the eight-character password.
 *
 * A hash is considered "interesting" when its hexadecimal MD5 representation
 * begins with **five leading zeroes**. When such a hash is found:
 *
 * - The **sixth character** of the hash indicates the password position
 *   (a digit from 0 to 7).
 * - The **seventh character** is the value to be placed in that position.
 * - Only the **first valid occurrence** for each position is used.
 * - Hashes that specify invalid positions or positions already filled are
 * ignored.
 *
 * Example (Door ID: "abc"):
 * - Index 3231929 → hash begins with "00000", sixth char = '1', seventh = '5'
 *   → position 1 receives '5':  _5______
 * - Index 5357525 → hash "000004e..." → position 4 receives 'e': _5__e___
 * - After resolving all positions, the password becomes: **05ace8e3**
 *
 * This program continues hashing indices until all eight positions of the
 * password have been filled according to the rules above. The final result is
 * the fully decrypted password for the second security door.
 *
 * Puzzle Answer: [Password]
 *
 * @author [gabrielzschmitz]
 * @date [26/11/2025]
 */
#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/md5.h>
#include <sstream>
#include <string>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test")
    debug = true;
}

std::string md5Hex(const std::string &input) {
  unsigned char digest[MD5_DIGEST_LENGTH];
  MD5((const unsigned char *)input.c_str(), input.length(), digest);

  std::stringstream ss;
  for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
  }
  return ss.str();
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string door_id;
  if (!std::getline(input_file, door_id)) {
    std::cerr << "Failed to read Door ID from input file." << std::endl;
    return 1;
  }

  std::string password(8, '_'); // 8 unknown positions
  int filled = 0;
  int index = 0;

  while (filled < 8) {
    std::string to_hash = door_id + std::to_string(index);
    std::string hash = md5Hex(to_hash);

    if (hash.substr(0, 5) == "00000") {
      char pos_char = hash[5];
      char value = hash[6];

      // position must be digit 0–7
      if (pos_char >= '0' && pos_char <= '7') {
        int pos = pos_char - '0';

        if (password[pos] == '_') {
          password[pos] = value;
          filled++;

          if (debug) {
            std::cout << "Hash: " << hash << " index: " << index
                      << " -> position " << pos << " = " << value << std::endl;
            std::cout << "Password: " << password << std::endl;
          }
        }
      }
    }

    index++;
  }

  std::cout << "ANSWER: " << password << std::endl;

  return 0;
}
