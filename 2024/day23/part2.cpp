/**
 * @file part2.cpp
 * @brief LAN Party Password Finder
 *
 * This program extends the LAN party network analysis to identify the largest
 * set of fully connected computers (a clique) in the given network map. The
 * goal is to determine the password for the LAN party, which is constructed by
 * sorting the names of the computers in the largest clique alphabetically and
 * joining them with commas.
 *
 * **Challenge Description:**
 * Each computer in the largest set of fully connected computers must have a
 * direct connection to every other computer in the set. The task involves
 * finding this clique in the network graph and generating the password for the
 * LAN party.
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
 * 2. Search the graph to find the largest clique (maximum fully connected
 * subset of computers).
 * 3. Generate the password by sorting the computer names in the largest clique
 * and joining them with commas.
 *
 * **Example Output:**
 * For the given input, the largest set of fully connected computers is:
 * co,de,ka,ta  
 *
 * The password for the LAN party would be:  
 * `co,de,ka,ta`
 *
 * **Tasks:**
 * - Parse the network map to construct the graph.
 * - Implement a search algorithm to find the largest clique in the graph.
 * - Generate and output the password based on the largest clique.
 *
 * **Puzzle Answer:** [Password]
 *
 * @author [gabrielzschmitz]
 * @date [23/12/2024]
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

void bronKerbosch(
  const std::set<std::string>& R, std::set<std::string>& P,
  std::set<std::string>& X,
  const std::unordered_map<std::string, std::unordered_set<std::string>>& graph,
  std::set<std::string>& largest_clique) {
  if (P.empty() && X.empty()) {
    if (R.size() > largest_clique.size()) largest_clique = R;
    return;
  }

  auto P_copy = P;
  for (const auto& v : P_copy) {
    std::set<std::string> new_R = R, new_P, new_X;
    new_R.insert(v);

    for (const auto& neighbor : graph.at(v)) {
      if (P.count(neighbor)) new_P.insert(neighbor);
      if (X.count(neighbor)) new_X.insert(neighbor);
    }

    bronKerbosch(new_R, new_P, new_X, graph, largest_clique);

    P.erase(v);
    X.insert(v);
  }
}

std::set<std::string> findLargestClique(
  const std::unordered_map<std::string, std::unordered_set<std::string>>&
    graph) {
  std::set<std::string> largest_clique;
  std::set<std::string> R, P, X;

  for (const auto& [node, _] : graph)
    P.insert(node);

  bronKerbosch(R, P, X, graph, largest_clique);
  return largest_clique;
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

  std::set<std::string> largest_clique = findLargestClique(graph);

  std::vector<std::string> sorted_clique(largest_clique.begin(),
                                         largest_clique.end());
  std::sort(sorted_clique.begin(), sorted_clique.end());
  std::string password = "";
  for (size_t i = 0; i < sorted_clique.size(); ++i) {
    if (i > 0) password += ",";
    password += sorted_clique[i];
  }

  std::cout << "ANSWER: " << password << std::endl;
  return 0;
}
