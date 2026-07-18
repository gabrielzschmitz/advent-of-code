/**
 * @file part1.cpp
 * @brief Simulating a Two-Factor Authentication Display
 *
 * This program simulates the pixel display used by a two-factor authentication
 * system. The display consists of a 50-column by 6-row screen where every
 * pixel is initially turned off.
 *
 * The input contains a sequence of instructions that manipulate the display:
 *   - rect AxB turns on all pixels in a rectangle of width A and height B in
 *     the top-left corner.
 *   - rotate row y=A by B shifts all pixels in row A to the right by B
 *     positions, wrapping around the screen.
 *   - rotate column x=A by B shifts all pixels in column A downward by B
 *     positions, wrapping around the screen.
 *
 * After executing every instruction, the program determines how many pixels
 * are illuminated on the display.
 *
 * Example Input:
 * rect 3x2
 * rotate column x=1 by 1
 * rotate row y=0 by 4
 * rotate column x=1 by 1
 *
 * Example Output:
 * 6
 *
 * The program processes all display instructions and outputs the total number
 * of lit pixels after the screen has been updated.
 *
 * Puzzle Answer: [Number of Lit Pixels]
 *
 * @author [gabrielzschmitz]
 * @date [18/07/2026]
 */
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

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

  constexpr int WIDTH = 50;
  constexpr int HEIGHT = 6;

  std::uint64_t answer = 0;
  std::array<std::array<bool, WIDTH>, HEIGHT> screen{};

  std::string line;
  while (std::getline(input_file, line)) {
    if (line.find("rect") == 0) {
      int w, h;
      sscanf(line.c_str(), "rect %dx%d", &w, &h);

      for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) screen[y][x] = true;
    }

    else if (line.find("rotate row") == 0) {
      int row, shift;
      sscanf(line.c_str(), "rotate row y=%d by %d", &row, &shift);

      std::array<bool, WIDTH> temp{};

      for (int x = 0; x < WIDTH; x++)
        temp[(x + shift) % WIDTH] = screen[row][x];

      screen[row] = temp;
    }

    else if (line.find("rotate column") == 0) {
      int col, shift;
      sscanf(line.c_str(), "rotate column x=%d by %d", &col, &shift);

      std::array<bool, HEIGHT> temp{};

      for (int y = 0; y < HEIGHT; y++)
        temp[(y + shift) % HEIGHT] = screen[y][col];

      for (int y = 0; y < HEIGHT; y++) screen[y][col] = temp[y];
    }

    if (debug) {
      std::cout << line << '\n';
      for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) std::cout << (screen[y][x] ? '#' : '.');
        std::cout << '\n';
      }
      std::cout << '\n';
    }
  }

  for (const auto& row : screen)
    for (bool pixel : row) answer += pixel;

  std::cout << "ANSWER: " << answer << '\n';

  return 0;
}
