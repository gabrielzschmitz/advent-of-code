/**
 * @file part1.cpp
 * @brief Lens Library - Sum of Results from HASH Algorithm on Initialization
 * Steps
 *
 * Run the HASH algorithm on each step in the initialization sequence and
 * calculate the sum of the results. The HASH algorithm turns any string of
 * characters into a single number in the range 0 to 255 based on specific
 * steps.
 *
 * Example Input:
 * rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7
 *
 * Example Output:
 * Sum of Results: 1320
 *
 * Parse the initialization sequence, run the HASH algorithm on each step, and
 * sum up the results to obtain the final result.
 *
 * @author [gabrielzschmitz]
 * @date [15/12/2023]
 */
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

  while (getline(iss, token, ',')) {
    result.push_back(token);
  }

  return result;
}

int HASH(string sequence) {
  int current = 0;
  for (char c : sequence) {
    current += c;
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

  for (string part : initsequence) {
    int current = HASH(part);
    total += current;
  }

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
