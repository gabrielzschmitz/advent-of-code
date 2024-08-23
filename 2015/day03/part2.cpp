/**
 * @file part2.cpp
 * @brief Santa and Robo-Santa's Joint Delivery
 *
 * This program extends Santa's delivery system by introducing Robo-Santa, a robot 
 * version of Santa designed to speed up the present delivery process. Both Santa 
 * and Robo-Santa start at the same location and take turns moving according to 
 * the directions provided by an elf at the North Pole.
 *
 * The directions are given as a string of characters where:
 * - '^' indicates moving north.
 * - 'v' indicates moving south.
 * - '>' indicates moving east.
 * - '<' indicates moving west.
 *
 * The task is to determine how many unique houses receive at least one present 
 * when both Santa and Robo-Santa are delivering presents. They start at the same 
 * location and alternate moves, delivering presents based on the shared directions.
 *
 * Example Scenarios:
 * - '^v' delivers presents to 3 houses: Santa goes north, and Robo-Santa goes south.
 * - '^>v<' delivers presents to 3 houses, with Santa and Robo-Santa ending up back 
 *   where they started.
 * - '^v^v^v^v^v' delivers presents to 11 houses, as Santa and Robo-Santa move 
 *   in opposite directions.
 *
 * The program processes the direction string and outputs the total number of unique 
 * houses that receive at least one present from Santa and Robo-Santa.
 *
 * Puzzle Answer: [Total Number of Houses Receiving at Least One Present]
 *
 * @author [gabrielzschmitz]
 * @date [23/08/2024]
 */
#include <fstream>
#include <iostream>
#include <set>
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

  std::set<std::pair<int, int>> santa_visited_houses;
  std::set<std::pair<int, int>> robo_visited_houses;
  int santa_x = 0, santa_y = 0;
  int robo_x = 0, robo_y = 0;
  santa_visited_houses.insert({santa_x, santa_y});
  robo_visited_houses.insert({robo_x, robo_y});

  char ch;
  bool is_santa = true;
  while (input_file.get(ch)) {
    if (is_santa) switch (ch) {
        case '^': santa_y++; break;
        case 'v': santa_y--; break;
        case '>': santa_x++; break;
        case '<': santa_x--; break;
        default: break;
      }
    else switch (ch) {
        case '^': robo_y++; break;
        case 'v': robo_y--; break;
        case '>': robo_x++; break;
        case '<': robo_x--; break;
        default: break;
      }
    is_santa = !is_santa;
    santa_visited_houses.insert({santa_x, santa_y});
    robo_visited_houses.insert({robo_x, robo_y});

    if (debug) {
      std::cout << "Santa moved to: (" << santa_x << ", " << santa_y << ")"
                << std::endl;
      std::cout << "Robo moved to: (" << robo_x << ", " << robo_y << ")"
                << std::endl;
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::set<std::pair<int, int>> combined_visited_houses = santa_visited_houses;
  combined_visited_houses.insert(robo_visited_houses.begin(),
                                 robo_visited_houses.end());

  int total = combined_visited_houses.size();
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
