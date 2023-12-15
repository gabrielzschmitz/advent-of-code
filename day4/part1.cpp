/**
 * @file part1.cpp
 * @brief Scratchcard Game Scorer
 *
 * This program processes scratchcard game scenarios stored in a file,
 * which is specified by a command-line argument or defaults to "input".
 * Each scratchcard game consists of two lists of numbers separated by a
 * vertical bar (|): the list of winning numbers and the list of player's
 * numbers. The program calculates the score for each game based on the matching
 * numbers, applying a doubling effect for each subsequent match. The total
 * score for all games is then outputted.
 *
 * The input file format is expected to contain lines representing scratchcard
 * games, with winning numbers and the player's numbers listed. The program
 * calculates the score for each game and outputs the total score for all games.
 *
 * Example Input:
 * Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
 * Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
 * Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
 * Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
 * Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
 * Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11
 *
 * The program outputs the total score for all scratchcard games combined.
 * If the input file is inaccessible, an error message is displayed.
 *
 * The program utilizes a struct 'Cards' to store information about each game,
 * including the card number, winning cards, and the player's cards. It uses a
 * vector to store information about each game and a scoring function to
 * calculate the total score.
 *
 * @author [gabrielzschmitz]
 * @date [04/12/2023]
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
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
  string cardstring;

  while (getline(input_file, line)) {
    istringstream iss(line);
    Cards game;
    string cardString;
    iss >> cardString >> game.card >>
        cardString;  // Extracting the "Card" and card number
    for (int i = 0; i < 10; i++) {
      iss >> game.winning_cards[i];
    }
    iss >> cardString;  // Skipping the "|"
    for (int i = 0; i < 25; i++) {
      iss >> game.my_cards[i];
    }
    games.push_back(game);
  }

  total = score_cards(games);

  cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}

int score_cards(const vector<Cards>& games) {
  int total = 0;

  for (Cards i : games) {
    vector<int> wins = {};
    for (int win : i.winning_cards) {
      for (int my : i.my_cards) {
        if (my == win) {
          wins.push_back(my);
        }
      }
    }

    int N = wins.size();
    int round = 0;
    int found = 1;
    for (int i = 0; i < N; i++) {
      if (i == 0)
        round += 1;
      else if (i == N - 1)
        round += 1;
      else {
        found = found * 2;
        round += found;
      }
    }

    total += round;
  }

  return total;
}
