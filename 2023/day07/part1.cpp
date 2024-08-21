/**
 * @file part1.cpp
 * @brief Camel Cards Challenge
 *
 * Your adventure begins with a one-way airship ride to Desert Island, where you
 * encounter an Elf seeking help. The sand-filtering machines are broken due to
 * a lack of parts, and you embark on a camel-riding journey to fix them.
 *
 * During the trip, the Elf introduces you to Camel Cards, a camel-friendly
 * poker game. Hands consist of five cards labeled A, K, Q, J, T, 9, 8, 7, 6, 5,
 * 4, 3, or 2. The goal is to order hands based on strength, with specific hand
 * types ranked higher than others.
 *
 * Hand Types (from strongest to weakest):
 * - Five of a kind (e.g., AAAAA)
 * - Four of a kind (e.g., AA8AA)
 * - Full house (e.g., 23332)
 * - Three of a kind (e.g., TTT98)
 * - Two pair (e.g., 23432)
 * - One pair (e.g., A23A4)
 * - High card (e.g., 23456)
 *
 * Hands with the same type are further ordered based on the strength of
 * individual cards. The bidding process assigns a rank to each hand, and
 * winnings are determined by multiplying the bid amount with the hand's rank.
 * The total winnings result from the sum of these calculations.
 *
 * Your task is to determine the rank of each hand in the given set and
 * calculate the total winnings. Provide the final answer representing the sum
 * of bid amounts multiplied by their respective ranks.
 *
 * @author [gabrielzschmitz]
 * @date [07/12/2023]
 */
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

enum HandType {
  FiveOfKind,
  FourOfKind,
  FullHouse,
  ThreeOfAKind,
  TwoPair,
  OnePair,
  HighCard
};

struct CamelCards {
  string hand;
  HandType type;
  int bid;
  int rank;
};

string handTypeToString(HandType type) {
  switch (type) {
    case FiveOfKind: return "FiveOfKind";
    case FourOfKind: return "FourOfKind";
    case FullHouse: return "FullHouse";
    case ThreeOfAKind: return "ThreeOfAKind";
    case TwoPair: return "TwoPair";
    case OnePair: return "OnePair";
    case HighCard: return "HighCard";
    default: return "Unknown";
  }
}

int cardValue(char card) {
  switch (card) {
    case 'A': return 14;
    case 'K': return 13;
    case 'Q': return 12;
    case 'J': return 11;
    case 'T': return 10;
    case '9': return 9;
    case '8': return 8;
    case '7': return 7;
    case '6': return 6;
    case '5': return 5;
    case '4': return 4;
    case '3': return 3;
    case '2': return 2;
  }
  return 0;
}

void printHands(const vector<CamelCards>& vec, const string& name) {
  cout << name << " (32T3K, OnePair, 765, 1):\n";
  for (CamelCards current : vec) {
    cout << "(" << current.hand << ", " << handTypeToString(current.type)
         << ", " << current.bid << ", " << current.rank << ") " << endl;
  }
  cout << endl;
}

void printVector(const vector<uint64_t>& vec, const string& name) {
  cout << name << ": ";
  for (uint64_t current : vec) {
    cout << current << ", ";
  }
  cout << endl;
}

// Function to determine the type of hand
void determineHandType(CamelCards& hand) {
  // Function to count the occurrences of each character in a string
  auto countChars = [](const string& str) {
    unordered_map<char, int> charCounts;
    for (char ch : str) {
      if (isalnum(ch)) { charCounts[ch]++; }
    }
    return charCounts;
  };

  unordered_map<char, int> charCounts = countChars(hand.hand);

  // Determine the hand type based on counts
  int maxCount = 0;
  int secondMaxCount = 0;
  for (const auto& pair : charCounts) {
    if (pair.second > maxCount) {
      secondMaxCount = maxCount;
      maxCount = pair.second;
    } else if (pair.second > secondMaxCount) {
      secondMaxCount = pair.second;
    }
  }

  if (maxCount == 5) hand.type = FiveOfKind;
  else if (maxCount == 4) hand.type = FourOfKind;
  else if (maxCount == 3 && secondMaxCount == 2) hand.type = FullHouse;
  else if (maxCount == 3 && secondMaxCount == 1) hand.type = ThreeOfAKind;
  else if (maxCount == 2 && secondMaxCount == 2) hand.type = TwoPair;
  else if (maxCount == 2 && secondMaxCount == 1) hand.type = OnePair;
  else hand.type = HighCard;
}

// Comparison function for sorting CamelCards based on strength
bool compareHands(const CamelCards& hand1, const CamelCards& hand2) {
  // Compare types
  if (hand1.type != hand2.type) {
    return hand1.type > hand2.type;  // Higher type is stronger
  }

  // Compare cards
  for (int i = 0; i < hand1.hand.size() + 1; ++i) {
    if (hand1.hand[i] != hand2.hand[i]) {
      if (cardValue(hand1.hand[i]) > cardValue(hand2.hand[i])) return false;
      else return true;
    }
  }

  // If all cards are equal, the hands are considered equal
  return false;
}

vector<CamelCards> inputToStruct(ifstream& input_file) {
  vector<CamelCards> camel;
  string line;
  while (getline(input_file, line)) {
    int currentBid = 0;
    char currentHand[100];
    if (sscanf(line.c_str(), "%s %d", currentHand, &currentBid) == 2) {
      CamelCards hand;
      hand.hand = currentHand;
      hand.bid = currentBid;
      hand.type = HighCard;
      hand.rank = 0;
      camel.push_back(hand);
    } else cerr << "Error parsing line: " << line << endl;
  }

  for (CamelCards& current : camel) {
    determineHandType(current);
  }

  // Sort hands based on comparison function
  sort(camel.begin(), camel.end(), compareHands);

  // Assign ranks
  for (int i = 0; i < camel.size(); ++i) {
    camel[i].rank = i + 1;  // Ranks start from 1
  }
  return camel;
}

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  uint64_t total = 0;
  vector<CamelCards> camel;

  camel = inputToStruct(input_file);
  input_file.close();

  for (CamelCards current : camel) {
    total = total + (current.bid * current.rank);
    cout << "(" << current.hand << ", " << handTypeToString(current.type)
         << ", " << current.bid << ", " << current.rank << ") " << total
         << endl;
  }
  // Display the extracted information
  // printHands(camel, "CamelCards");

  cout << "Total: " << total << endl;

  return 0;
}
