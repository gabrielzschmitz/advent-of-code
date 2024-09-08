/**
 * @file part1.cpp
 * @brief Molecule Replacement for Medicine Calibration
 *
 * This program is designed to help Santa calibrate the Red-Nosed Reindeer
 * nuclear fusion/fission plant. The goal is to determine how many distinct
 * molecules can be generated in one step by performing a series of
 * replacements on an initial medicine molecule.
 *
 * The machine supports a set of replacement rules, where a substring in the
 * molecule can be replaced by a specified alternative. The program processes
 * these replacements to find all possible distinct molecules that can be
 * generated from one replacement on the input molecule.
 *
 * Example Replacement Rules:
 * - H => HO
 * - H => OH
 * - O => HH
 *
 * Example Input:
 * Starting molecule: HOH
 *
 * Possible outputs from one replacement:
 * - HOOH (via H => HO on the first H)
 * - HOHO (via H => HO on the second H)
 * - OHOH (via H => OH on the first H)
 * - HOOH (via H => OH on the second H)
 * - HHHH (via O => HH)
 *
 * In this case, there are 4 distinct molecules after one replacement.
 *
 * The task is to count how many distinct molecules can be generated from one
 * replacement on the provided input molecule using the set of given replacement
 * rules.
 *
 * Puzzle Answer: [Number of Distinct Molecules]
 *
 * @author [gabrielzschmitz]
 * @date [08/09/2024]
 */
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
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

  std::vector<std::pair<std::string, std::string>> replacements;
  std::string medicine_molecule;
  std::string line;

  while (std::getline(input_file, line)) {
    if (line.empty()) {
      continue;
    } else if (line.find("=>") != std::string::npos) {
      size_t arrow_pos = line.find(" => ");
      std::string from = line.substr(0, arrow_pos);
      std::string to = line.substr(arrow_pos + 4);
      replacements.push_back({from, to});
    } else {
      medicine_molecule = line;
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::set<std::string> distinct_molecules;

  for (const auto& [from, to] : replacements) {
    size_t pos = 0;

    while ((pos = medicine_molecule.find(from, pos)) != std::string::npos) {
      std::string new_molecule = medicine_molecule.substr(0, pos) + to +
                                 medicine_molecule.substr(pos + from.size());
      distinct_molecules.insert(new_molecule);
      pos += from.size();
    }
  }

  int total = distinct_molecules.size();
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
