/**
 * @file part2.cpp
 * @brief Camel Cards Challenge - Part Two
 *
 * In this enhanced version of the Camel Cards Challenge, the Elf introduces an
 * additional rule to spice up the game. Now, J cards serve as jokers, acting as
 * wildcards that can assume the role of any card to form the strongest possible
 * hand. However, J cards are the weakest individual cards, even weaker than 2,
 * maintaining the order: A, K, Q, T, 9, 8, 7, 6, 5, 4, 3, 2, J.
 *
 * J cards have the flexibility to emulate any card for determining hand type.
 * For example, QJJQ2 is now considered four of a kind. However, when breaking
 * ties between two hands of the same type, J is always treated as J, not the
 * card it's pretending to be. For instance, JKKK2 is weaker than QQQQ2 because
 * J is weaker than Q.
 *
 * Given a set of hands and their corresponding bid amounts, your task is to
 * determine the rank of each hand using the new joker rule. Calculate the total
 * winnings by multiplying the bid amount with the hand's rank, and provide the
 * final answer representing the sum of bid amounts multiplied by their
 * respective ranks.
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
 * individual cards, considering the joker rule. The bidding process assigns a
 * rank to each hand, and winnings are determined by multiplying the bid amount
 * with the hand's rank. The total winnings result from the sum of these
 * calculations.
 *
 * Example Input:
 * - 32T3K 765
 * - T55J5 684
 * - KK677 28
 * - KTJJT 220
 * - QQQJA 483
 *
 * Example Output:
 * Total : 5905
 *
 * @author [gabrielzschmitz]
 * @date [07/12/2023]
 */
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
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
    case 'A': return 13;
    case 'K': return 12;
    case 'Q': return 11;
    case 'T': return 10;
    case '9': return 9;
    case '8': return 8;
    case '7': return 7;
    case '6': return 6;
    case '5': return 5;
    case '4': return 4;
    case '3': return 3;
    case '2': return 2;
    case 'J': return 1;
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
  int jokerCount = 0;
  auto countChars = [](const string& str, int& jokerCount) {
    unordered_map<char, int> charCounts;
    for (char ch : str) {
      if (ch == 'J') jokerCount += 1;
      if (isalnum(ch)) {
        if (ch != 'J') charCounts[ch]++;
      }
    }
    return charCounts;
  };

  unordered_map<char, int> charCounts = countChars(hand.hand, jokerCount);

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

  maxCount += jokerCount;
  cout << maxCount << ", " << secondMaxCount << endl;

  if (maxCount >= 5) hand.type = FiveOfKind;
  else if (maxCount == 4) hand.type = FourOfKind;
  else if (maxCount == 3 && secondMaxCount == 2) hand.type = FullHouse;
  else if (maxCount == 3 && secondMaxCount == 3) hand.type = ThreeOfAKind;
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
  for (int i = 0; i < hand1.hand.size(); i++)
    if (hand1.hand[i] != hand2.hand[i])
      if (hand1.hand[i] != hand2.hand[i])
        return cardValue(hand1.hand[i]) < cardValue(hand2.hand[i]);

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

  int64_t total = 0;
  vector<CamelCards> camel;

  camel = inputToStruct(input_file);
  input_file.close();

  for (const CamelCards& current : camel) {
    total += (current.bid * current.rank);
    cout << "(" << current.hand << ", " << handTypeToString(current.type)
         << ", " << current.bid << ", " << current.rank << ") ";
    cout << "Total: " << total << endl;
  }

  cout << "Total: " << total << endl;

  return 0;
}
