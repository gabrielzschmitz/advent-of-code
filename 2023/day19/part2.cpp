/**
 * @file part2.cpp
 * @brief Aplenty - Distinct Combinations Calculation
 *
 * Given a list of workflows and possible ratings for four categories (x, m, a,
 * s), determine the number of distinct combinations of ratings that will be
 * accepted by the workflows. Each workflow contains rules specifying conditions
 * for accepting or rejecting a part based on its ratings. Calculate the total
 * number of distinct combinations of ratings that will be accepted.
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
 * Total Distinct Combinations Accepted: 167409079868000
 *
 * Calculate the total number of distinct combinations of ratings that will be
 * accepted by the workflows.
 *
 * @author [gabrielzschmitz]
 * @date [19/12/2023]
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>
using namespace std;

bool debug = false;

vector<string> parse(const string& line, regex reg, int index) {
  vector<string> result(sregex_token_iterator(line.begin(), line.end(), reg),
                        sregex_token_iterator());
  return result;
}

struct rule_t {
  char left;
  char op;
  int right;
  string result;
};

struct workflow_t {
  string name;
  vector<rule_t> rules;
  string end;
};

using workflow_map_t = map<string, workflow_t>;
using rating_map_t = map<char, int>;

struct workflows_and_parts_t {
  workflow_map_t workflows;
  vector<rating_map_t> ratings;
};

workflows_and_parts_t load_input(const string& file) {
  workflows_and_parts_t ret;
  ifstream fs(file);
  string line;
  bool parsing_parts = false;

  while (getline(fs, line)) {
    if (line.empty()) {
      parsing_parts = true;
      continue;
    }

    if (parsing_parts) {
      auto strs = parse(line, regex("[\\w]+|[=]"), 0);
      rating_map_t parts;
      for (int i = 0; i < strs.size(); i += 3) {
        parts[strs[i][0]] = stoi(strs[i + 2]);
      }
      ret.ratings.push_back(parts);
    } else {
      workflow_t workflow;
      auto strs = parse(line, regex("[\\w]+|[<]|[>]"), 1);
      workflow.name = strs[0];
      for (int i = 1; i < strs.size() - 1; i += 4) {
        rule_t rule;
        rule.left = strs[i][0];
        rule.op = strs[i + 1][0];
        rule.left = strs[i][0];
        rule.right = stoi(strs[i + 2]);
        rule.result = strs[i + 3];
        workflow.rules.push_back(rule);
      }
      workflow.end = strs.back();
      ret.workflows[workflow.name] = workflow;
    }
  }

  return ret;
}

bool op(int left, char op, int right) {
  return op == '<' ? (left < right) : (left > right);
}

void sort_thru(const workflow_map_t& workflows, const rating_map_t& parts,
               const string& name, int idx, size_t& sum) {
  if (name == "A") {
    sum += parts.at('x') + parts.at('m') + parts.at('a') + parts.at('s');
    return;
  } else if (name == "R") {
    return;
  }

  if (idx == workflows.at(name).rules.size()) {
    sort_thru(workflows, parts, workflows.at(name).end, 0, sum);
  } else {
    const rule_t& rule = workflows.at(name).rules[idx];
    if (op(parts.at(rule.left), rule.op, rule.right)) {
      sort_thru(workflows, parts, rule.result, 0, sum);
    } else {
      sort_thru(workflows, parts, name, idx + 1, sum);
    }
  }
}

size_t part1(const workflows_and_parts_t& info) {
  size_t sum = 0;
  for (auto& rating_map : info.ratings) {
    sort_thru(info.workflows, rating_map, "in", 0, sum);
  }
  return sum;
}

struct interval_t {
  size_t range() const { return b - a + 1; }
  void intersect(const interval_t& other) {
    a = max(a, other.a);
    b = min(b, other.b);
  };
  int a, b;
};

using xmas_map_t = map<char, interval_t>;

void sort_thru_all(const xmas_map_t& xmas_map, const workflow_map_t& workflows,
                   const string& name, int idx, size_t& sum) {
  if (name == "A") {
    sum += xmas_map.at('x').range() * xmas_map.at('m').range() *
           xmas_map.at('a').range() * xmas_map.at('s').range();
    return;
  } else if (name == "R") {
    return;
  }

  if (idx == workflows.at(name).rules.size()) {
    sort_thru_all(xmas_map, workflows, workflows.at(name).end, 0, sum);
  } else {
    auto& rule = workflows.at(name).rules[idx];
    bool less_op = rule.op == '<';

    xmas_map_t map0 = xmas_map;
    map0[rule.left].intersect(less_op ? interval_t{1, rule.right - 1}
                                      : interval_t{rule.right + 1, 4000});
    sort_thru_all(map0, workflows, rule.result, 0, sum);

    xmas_map_t map1 = xmas_map;
    map1[rule.left].intersect(less_op ? interval_t{rule.right, 4000}
                                      : interval_t{1, rule.right});
    sort_thru_all(map1, workflows, name, idx + 1, sum);
  }
}

size_t summary(const workflows_and_parts_t& info) {
  size_t sum = 0;

  xmas_map_t xmas_map = {{'x', interval_t{1, 4000}},
                         {'m', interval_t{1, 4000}},
                         {'a', interval_t{1, 4000}},
                         {'s', interval_t{1, 4000}}};
  sort_thru_all(xmas_map, info.workflows, "in", 0, sum);

  return sum;
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";
  if (debug) {
    user_input = "test";
    auto testValues = load_input(user_input);
    cout << "part2: " << summary(testValues) << endl;
    return 0;
  }

  auto actualValues = load_input(user_input);

  cout << "part2: " << summary(actualValues) << endl;

  return 0;
}
