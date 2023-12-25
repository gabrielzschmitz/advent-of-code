/**
 * @file part1.cpp
 * @brief Snowverload - Component Disconnection
 *
 * Given a wiring diagram that shows how components are connected, find the
 * three wires to disconnect to divide the components into two separate groups.
 * Multiply the sizes of these two groups together.
 *
 * Example Input:
 * jqt: rhn xhk nvd
 * rsh: frs pzl lsr
 * xhk: hfx
 * cmg: qnr nvd lhk bvb
 * rhn: xhk bvb hfx
 * bvb: xhk hfx
 * pzl: lsr hfx nvd
 * qnr: nvd
 * ntq: jqt hfx bvb xhk
 * nvd: lhk
 * lsr: lhk
 * rzs: qnr cmg lsr rsh
 * frs: qnr lhk lsr
 *
 * Example Output:
 * 54
 *
 * Find the three wires to disconnect in order to divide the components into two
 * separate groups. What do you get if you multiply the sizes of these two
 * groups together?
 *
 * @author [gabrielzschmitz]
 * @date [25/12/2023]
 */
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <stack>
#include <tuple>
#include <cassert>
using namespace std;

bool debug = false;

typedef pair<string, string> pss;

fstream get_input() {
  string input_file = "input";
  if (debug) input_file = "test";
  fstream f(input_file, ios::in);
  return f;
}

map<string, vector<string>> edges;
vector<string> vertices;
map<string, int> set_sizes;

void absorb_node(string absorbing, string new_parent) {
  vector<string> adj_from_absorbed_node = edges[absorbing];
  for (int i = 0; i < adj_from_absorbed_node.size(); i++) {
    if (adj_from_absorbed_node[i] == new_parent) {
      adj_from_absorbed_node.erase(adj_from_absorbed_node.begin() + i);
      i--;
    }
  }

  for (int i = 0; i < edges[new_parent].size(); i++) {
    if (edges[new_parent][i] == absorbing) {
      edges[new_parent].erase(edges[new_parent].begin() + i);
      i--;
    }
  }

  for (string& s : adj_from_absorbed_node) {
    edges[new_parent].push_back(s);
  }

  edges.erase(absorbing);

  for (auto it = edges.begin(); it != edges.end(); it++) {
    for (string& s : it->second) {
      if (s == absorbing) {
        s = new_parent;
      }
    }
  }

  for (int i = 0; i < vertices.size(); i++) {
    if (vertices[i] == absorbing) {
      vertices.erase(vertices.begin() + i);
      i--;
    }
  }

  set_sizes[new_parent] += set_sizes[absorbing] + 1;
}

bool kargers() {
  const int initial_V = edges.size();
  int V = initial_V;
  while (V > 2) {
    int n, i;
    n = rand() % vertices.size();
    i = rand() % edges[vertices[n]].size();

    if (vertices[n] == edges[vertices[n]][i]) continue;

    absorb_node(edges[vertices[n]][i], vertices[n]);
    V--;
  }

  int cuts = 0;
  long long set_size_total = 1;
  for (auto it = edges.begin(); it != edges.end(); it++) {
    if (it->second.size() > 0) {
      if (debug) cout << "\tSet size: " << set_sizes[it->first] + 1 << endl;
      set_size_total *= set_sizes[it->first] + 1;
    }

    for (string& ed : it->second) {
      if (ed != it->first) cuts++;
    }
  }

  if (debug) {
    cout << "made " << cuts / 2 << " cuts!" << endl;
    cout << "NUMBER: " << set_size_total << endl << endl;
  }
  if (cuts / 2 == 3 && !debug)
    cout << "NUMBER: " << set_size_total << endl << endl;
  return (cuts / 2 == 3);
}

int main() {
  string ln;
  fstream f = get_input();
  string u, v;

  set<string> nodes_to_add;

  while (getline(f, ln)) {
    stringstream ss(ln);

    ss >> u;
    u = u.substr(0, u.length() - 1);
    nodes_to_add.insert(u);
    while (ss >> v) {
      edges[u].push_back(v);
      edges[v].push_back(u);
      nodes_to_add.insert(v);
    }
  }

  vertices.resize(nodes_to_add.size());
  copy(nodes_to_add.begin(), nodes_to_add.end(), vertices.begin());
  nodes_to_add.clear();

  map<string, vector<string>> edges_copy = edges;
  vector<string> vertices_copy = vertices;
  map<string, int> set_sizes_copy = set_sizes;

  while (!kargers()) {
    srand(time(0));

    edges = edges_copy;
    vertices = vertices_copy;
    set_sizes = set_sizes_copy;
  }
}
