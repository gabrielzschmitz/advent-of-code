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

bool checkDoubleSkip(const std::string& str) {
  for (size_t i = 0; i < str.size() - 2; ++i) {
    if (str[i] == str[i + 2]) return true;
  }
  return false;
}

bool checkPairTwice(const std::string& str) {
  for (size_t i = 0; i < str.size() - 1; ++i) {
    std::string pair = str.substr(i, 2);
    for (size_t j = i + 2; j < str.size() - 1; ++j) {
      if (pair == str.substr(j, 2)) return true;
    }
  }
  return false;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  int nice = 0;
  int bad = 0;
  std::string line;

  while (std::getline(input_file, line)) {
    if (!(checkDoubleSkip(line) && checkPairTwice(line))) ++bad;
    else ++nice;
  }

  std::cout << "Part 2: " << nice << std::endl;

  if (debug) {
    std::cout << "Total Nice: " << nice << ", Total Bad: " << bad << std::endl;
  }

  return 0;
}
