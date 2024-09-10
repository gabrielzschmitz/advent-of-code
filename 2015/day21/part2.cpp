/**
 * @file part2.cpp
 * @brief RPG Battle Simulator: Maximum Cost Defeat
 *
 * This program simulates a turn-based RPG battle between the player and a boss.
 * The goal in this part is to determine the most amount of gold the player can
 * spend on equipment and still lose the fight. 
 *
 * The player starts with 100 hit points and must purchase exactly one weapon, 
 * while armor and rings are optional. Each item adds to the player's damage 
 * or armor stats, which directly affect combat performance.
 *
 * During combat, the player and boss take turns attacking. The player's attack 
 * damage is determined by their total damage stat minus the boss's armor, 
 * and vice versa. Each attack reduces hit points, with the first to reach 
 * 0 losing the fight.
 *
 * The task is to determine the most amount of gold the player can spend on 
 * items and still be defeated by the boss, whose stats are provided in the
 * input.
 *
 * Example Input:
 * Player:
 * - 100 hit points
 * 
 * Boss:
 * - [Hit points, Damage, Armor values provided in the puzzle input]
 *
 * Shop:
 * Weapons, Armor, Rings with respective costs, damage, and armor modifiers 
 * (detailed in the problem description).
 *
 * Example Battle:
 * - Player deals damage = player_damage - boss_armor
 * - Boss deals damage = boss_damage - player_armor
 * 
 * Both continue attacking until one side's hit points reach 0.
 *
 * The program outputs the maximum amount of gold spent where the player still
 * loses.
 *
 * Puzzle Answer: [Maximum Gold Spent]
 *
 * @author [gabrielzschmitz]
 * @date [10/09/2024]
 */
#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

struct Item {
  std::string name;
  int cost;
  int damage;
  int armor;
};

struct Character {
  int hit_points;
  int damage;
  int armor;
};

bool simulate_battle(Character player, Character boss) {
  while (true) {
    int player_damage_dealt = std::max(1, player.damage - boss.armor);
    boss.hit_points -= player_damage_dealt;
    if (boss.hit_points <= 0) { return true; }

    int boss_damage_dealt = std::max(1, boss.damage - player.armor);
    player.hit_points -= boss_damage_dealt;
    if (player.hit_points <= 0) { return false; }
  }
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  // Shop items
  std::vector<Item> weapons = {{"Dagger", 8, 4, 0},
                               {"Shortsword", 10, 5, 0},
                               {"Warhammer", 25, 6, 0},
                               {"Longsword", 40, 7, 0},
                               {"Greataxe", 74, 8, 0}};
  std::vector<Item> armor = {
    {"None", 0, 0, 0},        {"Leather", 13, 0, 1},
    {"Chainmail", 31, 0, 2},  {"Splintmail", 53, 0, 3},
    {"Bandedmail", 75, 0, 4}, {"Platemail", 102, 0, 5}};
  std::vector<Item> rings = {
    {"None1", 0, 0, 0},       {"None2", 0, 0, 0},
    {"Damage +1", 25, 1, 0},  {"Damage +2", 50, 2, 0},
    {"Damage +3", 100, 3, 0}, {"Defense +1", 20, 0, 1},
    {"Defense +2", 40, 0, 2}, {"Defense +3", 80, 0, 3}};

  Character boss;
  std::string line;
  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    std::string label;
    int value;
    if (line.find("Hit Points:") != std::string::npos) {
      iss.ignore(11);
      iss >> value;
      boss.hit_points = value;
    } else if (line.find("Damage:") != std::string::npos) {
      iss.ignore(8);
      iss >> value;
      boss.damage = value;
    } else if (line.find("Armor:") != std::string::npos) {
      iss.ignore(7);
      iss >> value;
      boss.armor = value;
    }
  }

  if (debug)
    std::cout << "boss stats: " << boss.hit_points << "; " << boss.damage
              << "; " << boss.armor << std::endl;

  int max_cost = 0;

  // Iterate over all possible combinations of 1 weapon, 0-1 armor, 0-2 rings
  for (const auto& weapon : weapons) {
    for (const auto& arm : armor) {
      for (size_t i = 0; i < rings.size(); ++i) {
        for (size_t j = i + 1; j < rings.size(); ++j) {
          int total_cost =
            weapon.cost + arm.cost + rings[i].cost + rings[j].cost;
          int total_damage = weapon.damage + rings[i].damage + rings[j].damage;
          int total_armor = arm.armor + rings[i].armor + rings[j].armor;

          Character player = {100, total_damage, total_armor};

          if (!simulate_battle(player, boss))
            max_cost = std::max(max_cost, total_cost);
        }
      }
    }
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = max_cost;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
