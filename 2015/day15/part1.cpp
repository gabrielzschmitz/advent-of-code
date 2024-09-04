/**
 * @file part1.cpp
 * @brief Perfecting the Milk-Dunking Cookie Recipe
 *
 * This program assists in creating the optimal milk-dunking cookie recipe by
 * determining the best combination of ingredients to achieve the highest
 * possible cookie score. The recipe must use exactly 100 teaspoons of
 * ingredients, and the program calculates the total score based on the
 * properties of the ingredients: capacity, durability, flavor, and texture.
 *
 * Each ingredient contributes a certain amount to these properties, and the
 * total score is calculated by summing the individual property scores and
 * multiplying them together. If any property has a negative total, it is
 * treated as zero in the multiplication. The goal is to find the combination of
 * ingredients that yields the highest possible cookie score.
 *
 * Example Input:
 * - Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8
 * - Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3
 *
 * Example Output:
 * - Using 44 teaspoons of Butterscotch and 56 teaspoons of Cinnamon yields a 
 *   total cookie score of 62842880.
 *
 * Puzzle Answer: [Highest Cookie Score]
 *
 * @author [gabrielzschmitz]
 * @date [04/09/2024]
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string& input, bool& debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
    (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test") debug = true;
}

struct Ingredient {
  int capacity;
  int durability;
  int flavor;
  int texture;
  int calories;
};

std::vector<Ingredient> parseInput(std::ifstream& input_file) {
  std::vector<Ingredient> ingredients;
  std::string line;

  while (std::getline(input_file, line)) {
    Ingredient ingredient;
    sscanf(line.c_str(),
           "%*s capacity %d, durability %d, flavor %d, texture %d, calories %d",
           &ingredient.capacity, &ingredient.durability, &ingredient.flavor,
           &ingredient.texture, &ingredient.calories);
    ingredients.push_back(ingredient);
  }

  return ingredients;
}

int calculateScore(const std::vector<Ingredient>& ingredients,
                   const std::vector<int>& amounts) {
  int capacity = 0, durability = 0, flavor = 0, texture = 0;

  for (size_t i = 0; i < ingredients.size(); ++i) {
    capacity += ingredients[i].capacity * amounts[i];
    durability += ingredients[i].durability * amounts[i];
    flavor += ingredients[i].flavor * amounts[i];
    texture += ingredients[i].texture * amounts[i];
  }

  capacity = std::max(0, capacity);
  durability = std::max(0, durability);
  flavor = std::max(0, flavor);
  texture = std::max(0, texture);

  return capacity * durability * flavor * texture;
}

int main(int argc, char* argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  if (input_file.bad()) {
    std::cerr << "Error reading the file!" << std::endl;
    return 1;
  }

  std::vector<Ingredient> ingredients = parseInput(input_file);

  int best_score = 0;

  int num_ingredients = ingredients.size();
  std::vector<int> amounts(num_ingredients);

  for (amounts[0] = 0; amounts[0] <= 100; ++amounts[0]) {
    for (amounts[1] = 0; amounts[1] <= 100 - amounts[0]; ++amounts[1]) {
      if (num_ingredients > 2) {
        for (amounts[2] = 0; amounts[2] <= 100 - amounts[0] - amounts[1];
             ++amounts[2]) {
          amounts[3] = 100 - amounts[0] - amounts[1] - amounts[2];
          int score = calculateScore(ingredients, amounts);
          best_score = std::max(best_score, score);
        }
      } else {
        amounts[2] = 100 - amounts[0] - amounts[1];
        int score = calculateScore(ingredients, amounts);
        best_score = std::max(best_score, score);
      }
    }
  }

  int total = best_score;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
