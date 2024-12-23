/**
 * @file part1.cpp
 * @brief LAN Party Connection Finder
 *
 * This program analyzes a local network map to identify sets of inter-connected
 * computers as part of a quest to locate a LAN party. The network map provides
 * a list of connections between computers, where each connection is
 * bidirectional and represented as a pair of computer names.
 *
 * The primary goal is to find all sets of three computers where each computer
 * in the set is directly connected to the other two computers. Furthermore,
 * since the Chief Historian might be at the LAN party and is likely using a
 * computer whose name starts with 't', the program filters these sets to
 * include only those that have at least one computer whose name starts with
 * 't'.
 *
 * **Example Input:**
 * kh-tc  
 * qp-kh  
 * de-cg  
 * ka-co  
 * yn-aq  
 * qp-ub  
 * cg-tb  
 * ...
 *
 * **Example Process:**
 * 1. Parse the input to build a bidirectional graph of computer connections.
 * 2. Identify all sets of three computers where each is connected to the other
 * two.
 * 3. Filter these sets to include only those containing a computer whose name
 * starts with 't'.
 *
 * **Example Output:**
 * For the given input, 7 valid sets of three inter-connected computers are
 * found:
 * co,de,ta  
 * co,ka,ta  
 * de,ka,ta  
 * qp,td,wh  
 * tb,vc,wq  
 * tc,td,wh  
 * td,wh,yn  
 *
 * **Tasks:**
 * - Parse the network map to construct the graph.
 * - Implement a search algorithm to identify cliques of size 3.
 * - Apply the filtering criteria to count sets with a 't'-prefixed computer.
 *
 * **Puzzle Answer:** [Number of Sets]
 *
 * @author [gabrielzschmitz]
 * @date [23/12/2024]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

void addConnection(
  std::unordered_map<std::string, std::unordered_set<std::string>>& graph,
  const std::string& a, const std::string& b) {
  graph[a].insert(b);
  graph[b].insert(a);
}

std::set<std::set<std::string>> findCliques(
  const std::unordered_map<std::string, std::unordered_set<std::string>>&
    graph) {
  std::set<std::set<std::string>> unique_cliques;

  for (const auto& [node, neighbors] : graph)
    for (const auto& neighbor1 : neighbors)
      for (const auto& neighbor2 : neighbors)
        if (neighbor1 < neighbor2 && graph.at(neighbor1).count(neighbor2))
          unique_cliques.insert({node, neighbor1, neighbor2});

  return unique_cliques;
}

uint64_t countCliquesWithT(const std::set<std::set<std::string>>& cliques) {
  uint64_t count = 0;

  for (const auto& clique : cliques) {
    for (const auto& computer : clique) {
      if (!computer.empty() && computer[0] == 't') {
        count++;
        break;
      }
    }
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

  std::unordered_map<std::string, std::unordered_set<std::string>> graph;
  std::string line;
  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    std::string a, b;
    if (std::getline(iss, a, '-') && std::getline(iss, b))
      addConnection(graph, a, b);
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  auto cliques = findCliques(graph);

  uint64_t total = countCliquesWithT(cliques);

  if (debug) {
    for (const auto& clique : cliques) {
      for (const auto& computer : clique)
        std::cout << computer << " ";
      std::cout << std::endl;
    }
  }

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
