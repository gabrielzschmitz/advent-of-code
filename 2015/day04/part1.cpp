/**
 * @file part1.cpp
 * @brief Santa's AdventCoin Mining
 *
 * This program assists Santa in mining AdventCoins, a special type of currency
 * similar to bitcoins, by finding the lowest positive number that, when
 * combined with a secret key, produces an MD5 hash starting with at least five
 * zeroes in hexadecimal format.
 *
 * The input to the MD5 hash function consists of a secret key followed by a
 * number in decimal. The goal is to determine the smallest number that, when
 * appended to the secret key, generates an MD5 hash starting with five zeroes.
 *
 * Example Inputs:
 * - Secret key: abcdef, Result: 609043 (MD5 hash starts with 000001dbbfa...)
 * - Secret key: pqrstuv, Result: 1048970 (MD5 hash starts with 000006136ef...)
 *
 * Puzzle Input:
 * - Secret key: ckczppom
 *
 * The program processes the secret key and finds the lowest number that
 * satisfies the hash condition, outputting that number.
 *
 * Puzzle Answer: [Lowest Positive Number]
 *
 * @author [gabrielzschmitz]
 * @date [24/08/2024]
 */
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

uint32_t leftRotate(uint32_t x, uint32_t c) {
  return (x << c) | (x >> (32 - c));
}

std::string md5(const std::string& input) {
  // s specifies the per-round shift amounts
  static const uint32_t s[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

  // Use binary integer part of the sines of integers as constants:
  // uint32_t K[64];
  // for (int i = 0; i < 64; ++i)
  //   K[i] = static_cast<uint32_t>(floor((1ULL << 32) * fabs(sin(i + 1))));

  static const uint32_t K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
    0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
    0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
    0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
    0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
    0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

  // Initialize variables:
  uint32_t a0 = 0x67452301;  // A
  uint32_t b0 = 0xefcdab89;  // B
  uint32_t c0 = 0x98badcfe;  // C
  uint32_t d0 = 0x10325476;  // D

  // Pre-processing: adding a single 1 bit and padding with zeros
  std::vector<uint8_t> msg(input.begin(), input.end());
  // Capture the original message length in bytes
  size_t original_len = msg.size();
  // Append the '1' bit (0x80 in hexadecimal represents the bit '10000000')
  msg.push_back(0x80);

  // Padding with zeros until message length is 448 mod 512 (56 bytes mod 64)
  while (msg.size() % 64 != 56)
    msg.push_back(0);

  // Append the original length in bits mod 2^64
  uint64_t bits_len = original_len * 8;  // Calculate the length in bits
  msg.push_back(bits_len & 0xff);
  msg.push_back((bits_len >> 8) & 0xff);
  msg.push_back((bits_len >> 16) & 0xff);
  msg.push_back((bits_len >> 24) & 0xff);
  msg.push_back((bits_len >> 32) & 0xff);
  msg.push_back((bits_len >> 40) & 0xff);
  msg.push_back((bits_len >> 48) & 0xff);
  msg.push_back((bits_len >> 56) & 0xff);

  // Process the message in successive 512-bit chunks:
  for (size_t offset = 0; offset < msg.size(); offset += 64) {
    // Break chunk into sixteen 32-bit words M[j], 0 ≤ j ≤ 15
    uint32_t M[16];
    for (int i = 0; i < 16; ++i) {
      M[i] = (msg[offset + i * 4 + 0] << 0) | (msg[offset + i * 4 + 1] << 8) |
             (msg[offset + i * 4 + 2] << 16) | (msg[offset + i * 4 + 3] << 24);
    }

    // Initialize hash value for this chunk:
    uint32_t A = a0;
    uint32_t B = b0;
    uint32_t C = c0;
    uint32_t D = d0;

    // Main loop:
    for (int i = 0; i < 64; ++i) {
      uint32_t F, g;
      if (i < 16) {
        F = (B & C) | ((~B) & D);
        g = i;
      } else if (i < 32) {
        F = (D & B) | ((~D) & C);
        g = (5 * i + 1) % 16;
      } else if (i < 48) {
        F = B ^ C ^ D;
        g = (3 * i + 5) % 16;
      } else {
        F = C ^ (B | (~D));
        g = (7 * i) % 16;
      }
      F = F + A + K[i] + M[g];
      A = D;
      D = C;
      C = B;
      B = B + leftRotate(F, s[i]);
    }

    // Add this chunk's hash to result so far:
    a0 += A;
    b0 += B;
    c0 += C;
    d0 += D;
  }

  // Convert a0, b0, c0, d0 to little-endian before appending
  a0 = (a0 & 0xFF000000) >> 24 | (a0 & 0x00FF0000) >> 8 |
       (a0 & 0x0000FF00) << 8 | (a0 & 0x000000FF) << 24;
  b0 = (b0 & 0xFF000000) >> 24 | (b0 & 0x00FF0000) >> 8 |
       (b0 & 0x0000FF00) << 8 | (b0 & 0x000000FF) << 24;
  c0 = (c0 & 0xFF000000) >> 24 | (c0 & 0x00FF0000) >> 8 |
       (c0 & 0x0000FF00) << 8 | (c0 & 0x000000FF) << 24;
  d0 = (d0 & 0xFF000000) >> 24 | (d0 & 0x00FF0000) >> 8 |
       (d0 & 0x0000FF00) << 8 | (d0 & 0x000000FF) << 24;

  // Produce the final hash value (digest) as a hex string:
  std::ostringstream result;
  result << std::hex << std::setfill('0') << std::setw(8) << a0 << std::setw(8)
         << b0 << std::setw(8) << c0 << std::setw(8) << d0;

  return result.str();
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string base_input;
  std::getline(input_file, base_input);
  int number_to_append = 0;

  if (debug) std::cout << "input: " << base_input << std::endl;

  while (true) {
    std::string input = base_input + std::to_string(number_to_append);
    std::string hash = md5(input);

    if (debug)
      std::cout << "num: " << number_to_append << " md5: " << hash << std::endl;

    if (hash.substr(0, 5) == "00000") break;

    ++number_to_append;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = number_to_append;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
