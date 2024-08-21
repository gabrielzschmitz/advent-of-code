/**
 * @file part2.cpp
 * @brief Lens Library - Focusing Power of Resulting Lens Configuration
 *
 * Follow the initialization sequence (HASHMAP procedure) and configure the
 * lenses in the boxes based on the given steps. Calculate the focusing power of
 * the resulting lens configuration by summing up the individual focusing powers
 * of each lens.
 *
 * Example Input:
 * rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7
 *
 * Example Output:
 * Focusing Power of Resulting Configuration: 145
 *
 * Follow the steps, update the lens configuration, and calculate the focusing
 * power.
 *
 * @author [gabrielzschmitz]
 * @date [15/12/2023]
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

bool debug = false;

vector<string> extractElements(const string& inputString) {
  vector<string> result;
  istringstream iss(inputString);
  string token;
  while (getline(iss, token, ','))
    result.push_back(token);

  return result;
}

string removeCharacter(const string& original, char toRemove) {
  string result;
  for (char ch : original) {
    if (ch == toRemove) break;
    if (ch != toRemove) result.push_back(ch);
  }

  return result;
}

int HASH(string sequence) {
  int current = 0;
  for (char ch : sequence) {
    current += ch;
    current *= 17;
    current %= 256;
  }

  return current;
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

  string input;
  getline(input_file, input);
  vector<string> initsequence = extractElements(input);
  if (debug)
    for (string i : initsequence)
      cout << i << endl;

  vector<vector<pair<string, int>>> boxes(256);
  for (string part : initsequence) {
    size_t labelIndex, valueIndex;

    if ((labelIndex = part.find("-")) != string::npos) {
      string label = part.substr(0, labelIndex);
      size_t box = HASH(label);

      auto lens = find_if(
        boxes[box].begin(), boxes[box].end(),
        [label](const pair<string, int>& x) { return x.first == label; });

      if (lens != boxes[box].end()) {
        size_t idx = distance(boxes[box].begin(), lens);
        boxes[box].erase(boxes[box].begin() + idx);
      }
    }

    if ((valueIndex = part.find("=")) != string::npos) {
      string label = part.substr(0, valueIndex);
      size_t box = HASH(label);
      int focal_len = stoi(part.substr(valueIndex + 1));

      auto lens = find_if(
        boxes[box].begin(), boxes[box].end(),
        [label](const pair<string, int>& x) { return x.first == label; });

      if (lens != boxes[box].end()) {
        size_t idx = distance(boxes[box].begin(), lens);
        boxes[box][idx] = {label, focal_len};
      } else {
        boxes[box].push_back({label, focal_len});
      }
    }
  }
  for (size_t i = 0; i < boxes.size(); ++i) {
    int power = 0;
    int j = 0;
    for (const auto& box : boxes[i]) {
      j += 1;
      if (debug)
        cout << "Box " << i << ": " << j << " slot " << box.first << " = "
             << box.second << endl;

      power += (i + 1) * j * box.second;
    }
    total += power;
  }

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
