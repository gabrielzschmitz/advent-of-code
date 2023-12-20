/**
 * @file part1.cpp
 * @brief Aplenty - Workflow Sorting
 *
 * Given a list of workflows and part ratings, sort the parts through the
 * workflows based on the specified rules. Each part is rated in four
 * categories: x (Extremely cool looking), m (Musical), a (Aerodynamic), and s
 * (Shiny). The workflows contain rules that dictate the conditions for
 * accepting or rejecting a part and where to send it based on those conditions.
 * Calculate the sum of rating numbers for all parts that ultimately get
 * accepted.
 *
 * Example Input:
 * px{a<2006:qkq,m>2090:A,rfg}
 * pv{a>1716:R,A}
 * lnx{m>1548:A,A}
 * rfg{s<537:gd,x>2440:R,A}
 * qs{s>3448:A,lnx}
 * qkq{x<1416:A,crn}
 * crn{x>2662:A,R}
 * in{s<1351:px,qqz}
 * qqz{s>2770:qs,m<1801:hdj,R}
 * gd{a>3333:R,R}
 * hdj{m>838:A,pv}
 *
 * {x=787,m=2655,a=1222,s=2876}
 * {x=1679,m=44,a=2067,s=496}
 * {x=2036,m=264,a=79,s=2244}
 * {x=2461,m=1339,a=466,s=291}
 * {x=2127,m=1623,a=2188,s=1013}
 *
 * Example Output:
 * Sum of Accepted Parts Ratings: 19114
 *
 * Calculate the sum of rating numbers for all parts that ultimately get
 * accepted after going through the workflows.
 *
 * @author [gabrielzschmitz]
 * @date [19/12/2023]
 */
#include <unordered_map>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <tuple>
#include <map>
using namespace std;

bool debug = false;

struct Workflow {
  string ID;
  vector<string> rules;
  string fallback;
};

struct Parts {
  int X;
  int M;
  int A;
  int S;
};

map<char, function<bool(int, int)>> ops = {{'>', greater<int>()},
                                           {'<', less<int>()}};

Workflow parseWorkflowString(const string& input) {
  Workflow workflow;

  size_t start = input.find('{');
  size_t end = input.find('}');

  workflow.ID = input.substr(0, start);

  string rulesAndFallback = input.substr(start + 1, end - start - 1);
  istringstream ruleStream(rulesAndFallback);
  string rule;

  while (getline(ruleStream, rule, ',')) {
    if (rule.find(':') != string::npos) {
      workflow.rules.push_back(rule);
    } else {
      workflow.fallback = rule;
    }
  }

  return workflow;
}

Parts parsePartsString(const string& input) {
  Parts parts;

  string content = input.substr(1, input.size() - 2);

  istringstream tokenStream(content);
  string token;

  while (getline(tokenStream, token, ',')) {
    istringstream keyValueStream(token);
    string key, value;

    getline(keyValueStream, key, '=');
    getline(keyValueStream, value, '=');

    if (key == "x") {
      parts.X = stoi(value);
    } else if (key == "m") {
      parts.M = stoi(value);
    } else if (key == "a") {
      parts.A = stoi(value);
    } else if (key == "s") {
      parts.S = stoi(value);
    }
  }

  return parts;
}

Workflow findWorkflowByID(const vector<Workflow>& workflows, const string& ID) {
  for (const Workflow workflow : workflows)
    if (workflow.ID == ID) return workflow;

  return {};
}

bool processPart(const vector<Workflow>& workflows, const Parts& item,
                 string ID = "in") {
  if (ID == "R")
    return false;
  else if (ID == "A")
    return true;

  Workflow currentWorkflow = findWorkflowByID(workflows, ID);

  for (const string& rule : currentWorkflow.rules) {
    istringstream ruleStream(rule);
    char key, cmp;
    int n;
    string target;
    ruleStream >> key >> cmp;
    ruleStream >> n >> target;
    target = target.substr(1);

    if (debug) cout << key << " " << cmp << " " << n << ": " << target << "\n";
    if ((ops.at(cmp))(item.X, n) && key == 'x') {
      return processPart(workflows, item, target);
    } else if ((ops.at(cmp))(item.M, n) && key == 'm') {
      return processPart(workflows, item, target);
    } else if ((ops.at(cmp))(item.A, n) && key == 'a') {
      return processPart(workflows, item, target);
    } else if ((ops.at(cmp))(item.S, n) && key == 's') {
      return processPart(workflows, item, target);
    }
  }

  return processPart(workflows, item, currentWorkflow.fallback);
}

void readInput(ifstream& input_file, vector<Workflow>& workflows,
               vector<Parts>& parts) {
  string line;
  while (getline(input_file, line)) {
    if (line.empty()) break;
    Workflow current = parseWorkflowString(line);

    if (debug) {
      cout << "ID: " << current.ID << "\n";
      cout << "Rules:\n";
      for (const auto& rule : current.rules) cout << "  " << rule << "\n";
      cout << "Fallback: " << current.fallback << "\n\n";
    }

    workflows.push_back(current);
  }

  while (getline(input_file, line)) {
    Parts current = parsePartsString(line);

    if (debug) {
      cout << "X: " << current.X << "\n";
      cout << "M: " << current.M << "\n";
      cout << "A: " << current.A << "\n";
      cout << "S: " << current.S << "\n\n";
    }

    parts.push_back(current);
  }
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";
  if (debug) user_input = "test";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  int total = 0;

  vector<Workflow> workflows;
  vector<Parts> parts;
  readInput(input_file, workflows, parts);

  for (Parts item : parts) {
    if (processPart(workflows, item))
      total += item.X + item.M + item.A + item.S;
  }

  cout << "NUMBER:" << total << "\n";
  input_file.close();
  return 0;
}
