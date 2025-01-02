/**
 * @file part2.cpp
 * @brief Disk Fragmenter with Whole File Compacting
 *
 * This program helps compact a disk using a different method than in part 1.
 * Rather than moving individual file blocks to eliminate gaps, the amphipod now
 * wants to move whole files to the leftmost available span of free space that
 * can accommodate each file. The files are moved in order of their file ID
 * number, starting with the file with the highest ID.
 *
 * **Challenge Description:**
 * - The disk map is a single string of digits where:
 *   - Odd-indexed digits represent the length of a file in blocks.
 *   - Even-indexed digits represent the length of free space in blocks.
 * - Files are assigned unique IDs based on their order before rearrangement, 
 *   starting from ID 0.
 * - Instead of moving file blocks individually, the amphipod now moves entire 
 *   files, attempting to place each file into the leftmost available space that 
 *   can accommodate it.
 * - Files are moved in order of their ID number, starting with the file that 
 *   has the highest ID.
 * 
 * **Task:**
 * 1. Parse the disk map into individual blocks represented by file IDs or free 
 *    space (denoted by `.`).
 * 2. For each file (in order of decreasing file ID), attempt to move it to the
 * leftmost available free space that is large enough to accommodate the file.
 * 3. Calculate the checksum by iterating through the final arrangement of
 * blocks:
 *    - For each block, multiply its position by its file ID and add it to the 
 *      checksum.
 *    - Ignore free space blocks (`.`) in the calculation.
 * 4. Output the final checksum.
 *
 * **Example Input:**
 * ```
 * 2333133121414131402
 * ```
 *
 * **Example Output:**
 * ```
 * Filesystem Checksum: 2858
 * ```
 *
 * **Steps to Solve:**
 * 1. Parse the dense disk map into a list of blocks, distinguishing file IDs
 * and free spaces.
 * 2. For each file (starting from the one with the highest ID), find the
 * leftmost span of free space that is large enough to fit the file and move the
 * file there.
 * 3. Calculate the checksum by iterating through the final arrangement of
 * blocks:
 *    - For each block, multiply its position by its file ID and add it to the 
 *      checksum.
 *    - Ignore free space blocks (`.`) in the calculation.
 * 4. Output the final checksum.
 *
 * **Constraints:**
 * - The input is a single, very long string.
 * - The process stops once all files have been compacted, or if no file can be
 *   moved.
 *
 * **Puzzle Answer:** [Filesystem Checksum]
 *
 * @author [gabrielzschmitz]
 * @date [01/01/2025]
 */
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

  uint64_t total = 0;

  std::string content;
  std::getline(input_file, content);
  input_file.close();

  bool isSpace = false;
  std::vector<std::vector<int64_t>> spaces;
  std::vector<std::vector<int64_t>> files;
  int64_t maxLen = 0;
  int64_t curID = 0;

  for (char ch : content) {
    int64_t cnt = ch - '0';
    if (isSpace) spaces.push_back({maxLen, cnt});
    else {
      files.push_back({maxLen, cnt});
      curID++;
    }

    maxLen += cnt;
    isSpace = !isSpace;
  }

  for (int64_t i = files.size() - 1; i >= 0; --i) {
    for (size_t j = 0; j < spaces.size(); ++j) {
      if (spaces[j][0] < files[i][0] && spaces[j][1] >= files[i][1]) {
        spaces[j][1] -= files[i][1];
        files[i][0] = spaces[j][0];
        spaces[j][0] += files[i][1];
        break;
      }
    }
  }

  std::vector<int64_t> uncompressed(maxLen, -1);

  for (size_t i = 0; i < files.size(); ++i)
    for (int64_t j = 0; j < files[i][1]; ++j)
      uncompressed[files[i][0] + j] = i;

  for (size_t i = 0; i < uncompressed.size(); ++i)
    if (uncompressed[i] != -1) total += i * uncompressed[i];

  std::cout << "ANSWER: " << total << std::endl;

  return 0;
}
