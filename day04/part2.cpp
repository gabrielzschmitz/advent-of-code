/**
 * @file part2.cpp
 * @brief Advanced Scratchcard Scorer
 *
 * This program calculates the total number of scratchcards won in a scratchcard
 * game based on the number of matching numbers on each card. The rules dictate
 * that a winning card results in winning copies of subsequent cards equal to
 * the number of matches. Copies of scratchcards are then scored, and the
 * process repeats until no more cards are won.
 *
 * The input file contains information about each scratchcard game, specifying
 * the winning numbers and the player's numbers. The program processes each
 * game, calculating the total number of scratchcards won, including the
 * original set and all subsequent copies.
 *
 * Example Input:
 * Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
 * Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
 * Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
 * Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
 * Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
 * Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11
 *
 * The program outputs the total number of scratchcards, including originals and
 * copies, won for all scratchcard games combined.
 *
 * @author [gabrielzschmitz]
 * @date [04/12/2023]
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Cards {
  int card;
  int winning_cards[10];
  int my_cards[25];
};

int score_cards(const vector<Cards>& games);

int main(int argc, char* argv[]) {
  string user_input = (argc == 2) ? argv[1] : "input";

  ifstream input_file(user_input);
  if (!input_file.is_open()) {
    cout << "ARQUIVO " << user_input << " INACESSIVEL!" << endl;
    return -1;
  }

  int total = 0;
  string line;
  vector<Cards> games;

  while (getline(input_file, line)) {
    istringstream iss(line);
    Cards game;
    string cardString;
    iss >> cardString >> game.card >>
      cardString;  // Extracting the "Card" and card number
    for (int i = 0; i < 10; i++)
      iss >> game.winning_cards[i];
    iss >> cardString;  // Skipping the "|"
    for (int i = 0; i < 25; i++)
      iss >> game.my_cards[i];
    games.push_back(game);
  }

  total = score_cards(games);

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}

int score_cards(const vector<Cards>& games) {
  int total = 0;
  vector<int> cards(games.size(), 1);

  for (int i = 0; i < games.size(); i++) {
    int wins = 0;
    Cards current_card = games[i];
    for (int win : current_card.winning_cards) {
      for (int my : current_card.my_cards) {
        if (my == win) { wins++; }
      }
    }

    int below = i + 1;
    int until = below + wins;
    for (int j = below; j < until; j++) {
      cards[j] += cards[i];
    }
  }

  for (int i = 0; i < games.size(); i++)
    total += cards[i];
  return total;
}
