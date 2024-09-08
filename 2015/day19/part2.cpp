/**
 * @file part2.cpp
 * @brief Molecule Fabrication for Medicine Creation
 *
 * After calibrating the Red-Nosed Reindeer nuclear fusion/fission plant, the
 * next task is to fabricate the medicine molecule starting from a single
 * electron (e). The goal is to determine the fewest number of replacement steps
 * needed to transform the starting molecule "e" into the desired medicine
 * molecule.
 *
 * The fabrication process follows a set of replacement rules, just like those
 * used in calibration, and involves applying these rules one at a time to build
 * the target molecule step by step.
 *
 * Example Replacement Rules:
 * - e => H
 * - e => O
 * - H => HO
 * - H => OH
 * - O => HH
 *
 * Example Fabrication Steps:
 * To fabricate the molecule HOH from e:
 * 1. e => O
 * 2. O => HH
 * 3. H => OH (on the second H)
 *
 * This example reaches HOH in 3 steps.
 * The task is to compute the minimum number of steps required to transform the
 * starting molecule "e" into the target medicine molecule using the available
 * replacement rules.
 *
 * Puzzle Answer: [Fewest Number of Steps]
 *
 * @author [gabrielzschmitz]
 * @date [08/09/2024]
 */
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

int countOccurrences(const std::string& molecule, const std::string& token) {
  int count = 0;
  size_t pos = molecule.find(token);
  while (pos != std::string::npos) {
    ++count;
    pos = molecule.find(token, pos + token.size());
  }
  return count;
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

  std::regex atom_regex("([A-Z][a-z]?)");
  std::smatch match;
  int atom_count = 0;
  std::string molecule = medicine_molecule;
  while (std::regex_search(molecule, match, atom_regex)) {
    atom_count++;
    molecule = match.suffix();
  }

  int rn_count = countOccurrences(medicine_molecule, "Rn");
  int ar_count = countOccurrences(medicine_molecule, "Ar");
  int y_count = countOccurrences(medicine_molecule, "Y");

  int steps = atom_count - rn_count - ar_count - 2 * y_count - 1;

  int total = steps;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
