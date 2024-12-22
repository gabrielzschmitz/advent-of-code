/**
 * @file part2.cpp
 * @brief Monkey Exchange Market - Optimal Sequence for Maximum Bananas
 *
 * In this extended challenge, we refine the simulation of secret numbers and
 * their derived prices to maximize the bananas obtained through strategic
 * sales. Each buyer generates a sequence of secret numbers, and the prices are
 * determined as the ones digit of these secret numbers.
 *
 * The task is to identify a single sequence of four consecutive price changes
 * that, when tracked by the monkey negotiator, results in the maximum bananas
 * collected across all buyers. The monkey will sell to a buyer the first time
 * this sequence appears in their price changes.
 *
 * **Key Rules:**
 * - Prices are derived as the ones digit of secret numbers.
 * - Changes in price are calculated as the difference between consecutive
 *   prices.
 * - The monkey looks for a specific sequence of four changes to trigger a sale.
 * - Only one sequence of four changes can be given to the monkey for all
 *   buyers.
 * - If the sequence does not occur in a buyer's price changes, the monkey skips
 *   that buyer.
 *
 * **Example Input:**
 * 1
 * 2
 * 3
 * 2024
 *
 * **Example Output:**
 * Sequence: -2, 1, -1, 3
 * Total Bananas: 23
 *
 * **Tasks:**
 * 1. Parse the initial secret numbers for each buyer.
 * 2. Simulate the sequence of 2000 secret numbers per buyer.
 * 3. Extract the prices and calculate their changes.
 * 4. Identify the optimal sequence of four changes to maximize bananas
 * collected.
 * 5. Output the total number of bananas obtained.
 *
 * Puzzle Answer: [Most Bananas Collected]
 *
 * @author [gabrielzschmitz]
 * @date [22/12/2024]
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

namespace std {
template <>
struct hash<std::tuple<uint64_t, uint64_t, uint64_t, uint64_t>> {
  size_t operator()(
    const std::tuple<uint64_t, uint64_t, uint64_t, uint64_t>& t) const {
    size_t h1 = std::hash<uint64_t>{}(std::get<0>(t));
    size_t h2 = std::hash<uint64_t>{}(std::get<1>(t));
    size_t h3 = std::hash<uint64_t>{}(std::get<2>(t));
    size_t h4 = std::hash<uint64_t>{}(std::get<3>(t));

    // Combine the individual hashes using a common hashing pattern
    return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
  }
};
}  // namespace std

std::vector<uint64_t> uint64_ts(const std::string& s) {
  std::vector<uint64_t> result;
  std::regex r("-?\\d+");
  auto begin = std::sregex_iterator(s.begin(), s.end(), r);
  auto end = std::sregex_iterator();
  for (auto it = begin; it != end; ++it) {
    result.push_back(std::stoi(it->str()));
  }
  return result;
}

uint64_t mix(uint64_t x, uint64_t y) {
  return x ^ y;
}

uint64_t prune(uint64_t x) {
  return x % 16777216;
}

std::vector<uint64_t> prices(uint64_t x) {
  std::vector<uint64_t> ans = {x};
  for (uint64_t i = 0; i < 2000; ++i) {
    x = prune(mix(x, 64 * x));
    x = prune(mix(x, x / 32));
    x = prune(mix(x, x * 2048));
    ans.push_back(x);
  }
  return ans;
}

std::vector<uint64_t> changes(const std::vector<uint64_t>& P) {
  std::vector<uint64_t> result;
  for (size_t i = 1; i < P.size(); ++i) {
    result.push_back(P[i] - P[i - 1]);
  }
  return result;
}

std::unordered_map<std::tuple<uint64_t, uint64_t, uint64_t, uint64_t>, uint64_t>
getScores(const std::vector<uint64_t>& P, const std::vector<uint64_t>& C) {
  std::unordered_map<std::tuple<uint64_t, uint64_t, uint64_t, uint64_t>,
                     uint64_t>
    ANS;
  for (size_t i = 0; i + 3 < C.size(); ++i) {
    auto pattern = std::make_tuple(C[i], C[i + 1], C[i + 2], C[i + 3]);
    if (ANS.find(pattern) == ANS.end()) { ANS[pattern] = P[i + 4]; }
  }
  return ANS;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::string D;
  std::string line;
  while (std::getline(input_file, line))
    D += line + "\n";

  std::unordered_map<std::tuple<uint64_t, uint64_t, uint64_t, uint64_t>,
                     uint64_t>
    SCORE;

  std::istringstream stream(D);
  while (std::getline(stream, line)) {
    uint64_t secret = std::stoi(line);
    std::vector<uint64_t> P = prices(secret);
    for (auto& p : P)
      p %= 10;
    std::vector<uint64_t> C = changes(P);
    auto S = getScores(P, C);
    for (const auto& [k, v] : S) {
      if (SCORE.find(k) == SCORE.end()) SCORE[k] = v;
      else SCORE[k] += v;
    }
  }

  uint64_t total = 0;
  for (const auto& [k, v] : SCORE)
    total = std::max(total, v);

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
