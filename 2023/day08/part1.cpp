/**
 * @file part1.cpp
 * @brief Haunted Wasteland Navigation
 *
 * As you traverse Desert Island on camelback, a sudden sandstorm approaches,
 * and the Elf mysteriously disappears. Left with a pouch of labeled "maps," you
 * find instructions and a network of labeled nodes. The objective is to
 * navigate this network using left/right instructions to escape the haunted
 * wasteland. The starting point is node AAA, and the goal is to reach node ZZZ
 * following the provided instructions.
 *
 * The network is defined by nodes, and each node contains left and right
 * elements. The left/right instructions guide the camel to the next element
 * based on the current instruction. If the instructions run out, repeat the
 * sequence as needed.
 *
 * Example Input:
 * RL
 * AAA = (BBB, CCC)
 * BBB = (DDD, EEE)
 * CCC = (ZZZ, GGG)
 * DDD = (DDD, DDD)
 * EEE = (EEE, EEE)
 * GGG = (GGG, GGG)
 * ZZZ = (ZZZ, ZZZ)
 *
 * Starting at AAA, follow the left/right instructions to reach ZZZ. For
 * example, RL leads from AAA to CCC, then L to ZZZ. If the instructions run
 * out, repeat the sequence as necessary.
 *
 * Your task is to determine the number of steps required to reach ZZZ starting
 * from AAA using the given left/right instructions.
 *
 * Example Output:
 * Total: 6
 *
 * @author [gabrielzschmitz]
 * @date [08/12/2023]
 */
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

struct NetworkNode {
  string element;
  string left;
  string right;
};

// void printHands(const vector<CamelCards>& vec, const string& name) {
//   cout << name << " (32T3K, OnePair, 765, 1):\n";
//   for (CamelCards current : vec) {
//     cout << "(" << current.hand << ", " << handTypeToString(current.type)
//          << ", " << current.bid << ", " << current.rank << ") " << endl;
//   }
//   cout << endl;
// }

void printVector(const vector<uint64_t>& vec, const string& name) {
  cout << name << ": ";
  for (uint64_t current : vec) {
    cout << current << ", ";
  }
  cout << endl;
}

uint64_t countSteps(vector<NetworkNode>& network, const string& commands) {
  uint64_t steps = 0;
  NetworkNode current;

  // Find the starting point ("AAA")
  for (int i = 0; i < network.size(); i++) {
    current = network[i];
    if (current.element == "AAA") break;
  }

  int commandPos = 0;
  while (current.element != "ZZZ") {
    if (commandPos == 277) commandPos = 0;
    steps += 1;
    if (commands[commandPos] == 'L') {
      cout << "LEFT ";
      string left = current.left;
      for (int i = 0; i < network.size(); i++) {
        current = network[i];
        if (current.element == left) break;
      }
      commandPos += 1;
    } else if (commands[commandPos] == 'R') {
      cout << "RIGHT ";
      string right = current.right;
      cout << right << endl;
      for (int i = 0; i < network.size(); i++) {
        current = network[i];
        if (current.element == right) break;
      }
      commandPos += 1;
    }
  }
  return steps;
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  uint64_t total = 0;
  string line;
  string commands;
  vector<NetworkNode> network = {};

  getline(input_file, line);
  commands = line;
  getline(input_file, line);
  cout << commands << endl;

  // Read the rest of the lines representing relationships
  while (getline(input_file, line)) {
    NetworkNode current;

    istringstream iss(line);
    iss >> current.element;
    iss.ignore();                      // ignore the ' ' character
    iss.ignore();                      // ignore the '=' character
    iss.ignore();                      // ignore the ' ' character
    iss.ignore();                      // ignore the '(' character
    getline(iss, current.left, ',');   // Read the right part until ')'
    iss.ignore();                      // ignore the ',' character
    getline(iss, current.right, ')');  // Read the right part until ')'

    network.push_back(current);
  }
  input_file.close();

  for (const NetworkNode& i : network)
    cout << i.element << " (" << i.left << ", " << i.right << ") " << endl;

  total = countSteps(network, commands);

  cout << "Total: " << total << endl;

  return 0;
}
