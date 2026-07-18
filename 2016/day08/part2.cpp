/**
 * @file part2.cpp
 * @brief Decoding the Two-Factor Authentication Display
 *
 * This program simulates the same 50×6 pixel display used by the two-factor
 * authentication system and executes the full sequence of display
 * instructions.
 *
 * The display begins with all pixels turned off. The input consists of three
 * possible operations:
 *   - rect AxB turns on all pixels in a rectangle of width A and height B in
 *     the top-left corner.
 *   - rotate row y=A by B shifts row A to the right by B positions, wrapping
 *     around the screen.
 *   - rotate column x=A by B shifts column A downward by B positions,
 *     wrapping around the screen.
 *
 * After all instructions have been applied, the resulting pixel pattern forms
 * a sequence of capital letters. Each character is rendered using a fixed 5×6
 * pixel font.
 *
 * The program simulates every instruction, renders the final screen, and
 * outputs the code displayed by the screen.
 *
 * Puzzle Answer: [Displayed Authentication Code]
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

  std::cout << "DISPLAY:\n";
  for (const auto& row : screen) {
    for (bool pixel : row) std::cout << (pixel ? '#' : '.');
    std::cout << '\n';
  }

  return 0;
}
