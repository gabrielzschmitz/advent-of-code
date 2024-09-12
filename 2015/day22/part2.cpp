/**
 * @file part2.cpp
 * @brief Wizard Simulator: Hard Mode Minimum Mana for Victory
 *
 * This program simulates a turn-based battle between a player (wizard) and a
 * boss in a role-playing game under hard mode conditions. The player starts
 * with 50 hit points and 500 mana points, and the goal is to determine the
 * least amount of mana the player can spend to defeat the boss. 
 *
 * In hard mode, the player loses 1 hit point at the start of each turn, before
 * any other effects apply. If this reduces the player's hit points to 0 or
 * below, the player loses. All other rules and mechanics of the game remain the
 * same as in normal mode.
 *
 * The player and the boss take turns attacking, with the player going first.
 * The player can cast one of several spells each turn, and effects from spells
 * may alter the gameplay. The player's damage is not reduced by the boss's
 * armor since the boss effectively has zero armor due to magic damage.
 *
 * The spells available to the player are:
 * - Magic Missile: Costs 53 mana, does 4 damage.
 * - Drain: Costs 73 mana, does 2 damage and heals the player for 2 hit points.
 * - Shield: Costs 113 mana, increases player's armor by 7 for 6 turns.
 * - Poison: Costs 173 mana, deals 3 damage to the boss each turn for 6 turns.
 * - Recharge: Costs 229 mana, restores 101 mana to the player each turn for 5
 *   turns.
 *
 * The player must manage their mana and health while casting spells and
 * handling effects. Each turn, the player must choose a spell to cast or lose
 * if they cannot afford any spell. The boss's stats are provided in the puzzle
 * input.
 *
 * The program calculates the minimum mana spent by the player to win the battle
 * under hard mode conditions, where the player loses 1 hit point at the start
 * of each turn.
 *
 * Example Input: Boss stats (hit points, damage, armor) are given in the puzzle
 * input.
 *
 * Example Battle:
 * - Player loses 1 hit point at the start of their turn.
 * - Player casts a spell and its effects apply.
 * - Boss attacks with damage reduced by the player's armor.
 * - Spells and their effects modify the game state and may last multiple turns.
 * 
 * The program outputs the minimum amount of mana spent where the player still
 * wins under hard mode rules.
 *
 * Puzzle Answer: [Minimum Mana Spent]
 *
 * @author [gabrielzschmitz]
 * @date [11/09/2024]
 */
#include <algorithm>
#include <climits>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');
  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);
  if (last_part == "test") debug = true;
}

struct Stat {
  int perm = 0;
  int temp = 0;
  int time_left = 0;
};

enum Effects {
  Shield,
  Poison,
  Recharge,
  None,
};

struct Entity {
  Stat hp;
  Stat damage;
  Stat armour;
  Stat mana;
  std::unordered_set<Effects> effects;
};

struct Spell {
  std::string name;
  int cost;
  Effects effect;
  std::function<void(Entity&, Entity&)> function;
};

std::vector<Spell> spells{
  {"Magic Missile", 53, Effects::None,
   [](Entity& e1, Entity& e2) {
     e1.mana.perm -= 53;
     e2.hp.perm -= 4;
   }},
  {"Drain", 73, Effects::None,
   [](Entity& e1, Entity& e2) {
     e1.mana.perm -= 73;
     e1.hp.perm += 2;
     e2.hp.perm -= 2;
   }},
  {"Shield", 113, Effects::Shield,
   [](Entity& e1, Entity& e2) {
     e1.mana.perm -= 113;
     e1.armour.temp = 7;
     e1.armour.time_left = 6;
     e1.effects.insert(Effects::Shield);
   }},
  {"Poison", 173, Effects::Poison,
   [](Entity& e1, Entity& e2) {
     e1.mana.perm -= 173;
     e2.hp.temp = -3;
     e2.hp.time_left = 6;
     e2.effects.insert(Effects::Poison);
   }},
  {"Recharge", 229, Effects::Recharge,
   [](Entity& e1, Entity& e2) {
     e1.mana.perm -= 229;
     e1.mana.temp = 101;
     e1.mana.time_left = 5;
     e1.effects.insert(Effects::Recharge);
   }},
  // {"None" , 0, [](Entity& e1, Entity& e2) { }},
};

int min_mana_spent = std::numeric_limits<int>::max();

void resolve_effects(Entity& e) {
  e.hp.perm += e.hp.temp;
  e.mana.perm += e.mana.temp;
  if (e.hp.time_left > 0) e.hp.time_left--;
  if (e.damage.time_left > 0) e.damage.time_left--;
  if (e.armour.time_left > 0) e.armour.time_left--;
  if (e.mana.time_left > 0) e.mana.time_left--;
  if (e.hp.time_left == 0) { e.effects.erase(Effects::Poison); }
  if (e.armour.time_left == 0) { e.effects.erase(Effects::Shield); }
  if (e.mana.time_left == 0) { e.effects.erase(Effects::Recharge); }
}

void reset_timed_out_effects(Entity& e) {
  if (e.hp.time_left == 0) { e.hp.temp = 0; }
  if (e.damage.time_left == 0) { e.damage.temp = 0; }
  if (e.armour.time_left == 0) { e.armour.temp = 0; }
  if (e.mana.time_left == 0) { e.mana.temp = 0; }
}

bool should_return(const Entity& player_in, const Entity& boss_in,
                   int mana_spent) {
  if (boss_in.hp.perm <= 0) {
    min_mana_spent = std::min(mana_spent, min_mana_spent);
    return true;
  }
  if (player_in.hp.perm <= 0) { return true; }
  return false;
}

void fight(const Entity& player_in, const Entity& boss_in, bool player_turn,
           int mana_spent, int level) {
  if (mana_spent >= min_mana_spent) return;
  if (should_return(player_in, boss_in, mana_spent)) return;
  if (player_turn) {
    for (const auto& spell : spells) {
      if (player_in.mana.perm < spell.cost) continue;
      auto player = player_in;
      auto boss = boss_in;
      player.hp.perm--;
      if (should_return(player_in, boss_in, mana_spent)) return;
      resolve_effects(player);
      resolve_effects(boss);
      if (player.effects.count(spell.effect)) continue;
      if (boss.effects.count(spell.effect)) continue;
      if (should_return(player_in, boss_in, mana_spent)) return;
      spell.function(player, boss);
      reset_timed_out_effects(player);
      reset_timed_out_effects(boss);
      fight(player, boss, false, mana_spent + spell.cost, level + 1);
    }
  } else {
    auto player = player_in;
    auto boss = boss_in;
    resolve_effects(player);
    resolve_effects(boss);
    if (should_return(player_in, boss_in, mana_spent)) return;
    player.hp.perm -=
      std::max(1, boss.damage.perm - player.armour.perm - player.armour.temp);
    reset_timed_out_effects(boss);
    reset_timed_out_effects(player);
    fight(player, boss, true, mana_spent, level + 1);
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

  Entity enemy;
  std::string line;
  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    std::string label;
    int value;
    if (line.find("Hit Points:") != std::string::npos) {
      iss.ignore(11);
      iss >> value;
      enemy.hp.perm = value;
    } else if (line.find("Damage:") != std::string::npos) {
      iss.ignore(8);
      iss >> value;
      enemy.damage.perm = value;
    }
  }

  Entity player;
  player.hp.perm = 50;
  player.mana.perm = 500;

  if (debug)
    std::cout << "boss stats: " << enemy.hp.perm << "; " << enemy.damage.perm
              << std::endl;

  fight(player, enemy, true, 0, 1);

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  int total = min_mana_spent;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
