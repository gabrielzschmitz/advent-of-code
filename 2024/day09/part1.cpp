/**
 * @file part1.cpp
 * @brief Disk Fragmenter and Filesystem Compaction
 *
 * This program helps compact a disk by rearranging file blocks to remove gaps
 * between them, as specified in the disk map. The disk map uses a dense format
 * to represent alternating lengths of files and free space. The goal is to
 * calculate the final checksum of the filesystem after all file blocks have
 * been compacted.
 *
 * **Challenge Description:**
 * - The disk map is a single string of digits where:
 *   - Odd-indexed digits represent the length of a file in blocks.
 *   - Even-indexed digits represent the length of free space in blocks.
 * - Files are assigned unique IDs based on their order before rearrangement, 
 *   starting from ID 0.
 * - Compacting involves moving file blocks one at a time to the leftmost free 
 *   space until all gaps are removed.
 *
 * **Task:**
 * 1. Parse the disk map into individual blocks represented by file IDs or free 
 *    space (denoted by `.`).
 * 2. Rearrange the blocks to eliminate all gaps, moving file blocks leftward.
 * 3. Calculate the checksum as the sum of products of block positions and their 
 *    corresponding file IDs.
 *
 * **Example Input:**
 * ```
 * 2333133121414131402
 * ```
 *
 * **Example Output:**
 * ```
 * Filesystem Checksum: 1928
 * ```
 *
 * **Steps to Solve:**
 * 1. Parse the dense disk map into a list of blocks, distinguishing file IDs
 * and free spaces.
 * 2. Simulate the compaction process by repeatedly moving file blocks to the 
 *    leftmost available free space.
 * 3. Calculate the checksum by iterating through the final arrangement of
 * blocks:
 *    - For each block, multiply its position by its file ID and add it to the 
 *      checksum.
 *    - Ignore free space blocks (`.`) in the calculation.
 * 4. Output the final checksum.
 *
 * **Constraints:**
 * - The input is a single, very long string.
 * - The process stops once all file blocks are contiguous.
 *
 * **Puzzle Answer:** [Filesystem Checksum]
 *
 * @author [gabrielzschmitz]
 * @date [01/01/2025]
 */
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
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
  std::getline(input_file, line);
  input_file.close();

  std::string disk = line;

  std::vector<int> new_disk;
  for (size_t i = 0; i < disk.size(); ++i) {
    int length = disk[i] - '0';
    if (i % 2 == 0)
      for (int j = 0; j < length; ++j)
        new_disk.push_back(i / 2);
    else
      for (int j = 0; j < length; ++j)
        new_disk.push_back(-1);
  }

  if (debug) {
    for (int block : new_disk)
      std::cout << block << " ";
    std::cout << std::endl;
  }

  for (size_t i = 0; i < new_disk.size(); ++i) {
    if (new_disk[i] == -1) {
      for (size_t j = new_disk.size() - 1; j > i; --j) {
        if (new_disk[j] != -1) {
          std::swap(new_disk[i], new_disk[j]);
          break;
        }
      }
    }
  }

  int64_t total = 0;
  for (size_t i = 0; i < new_disk.size(); ++i)
    if (new_disk[i] != -1) total += i * new_disk[i];

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
