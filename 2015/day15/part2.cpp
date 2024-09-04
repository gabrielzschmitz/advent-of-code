/**
 * @file part2.cpp
 * @brief Calorie-Constrained Cookie Optimization
 *
 * Building on the previous cookie recipe optimization, this program now 
 * includes an additional constraint: the final cookie must contain exactly 
 * 500 calories. The goal is to find the combination of ingredients that 
 * achieves the highest possible cookie score while also meeting this calorie 
 * requirement.
 *
 * As before, the recipe must use exactly 100 teaspoons of ingredients, and the 
 * score is determined by multiplying the summed properties of capacity, 
 * durability, flavor, and texture. However, this time, the total score must be 
 * computed only for recipes that result in exactly 500 calories.
 *
 * Example Input:
 * - Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8
 * - Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3
 *
 * Example Output:
 * - Using 40 teaspoons of Butterscotch and 60 teaspoons of Cinnamon yields a 
 *   total calorie count of 500 and a cookie score of 57600000.
 *
 * The program processes the possible combinations and outputs the highest 
 * cookie score achievable under the 500 calorie constraint.
 *
 * Puzzle Answer: [Highest Calorie-Constrained Cookie Score]
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

std::pair<int, int> calculateScoreAndCalories(
  const std::vector<Ingredient>& ingredients, const std::vector<int>& amounts) {
  int capacity = 0, durability = 0, flavor = 0, texture = 0, calories = 0;

  for (size_t i = 0; i < ingredients.size(); ++i) {
    capacity += ingredients[i].capacity * amounts[i];
    durability += ingredients[i].durability * amounts[i];
    flavor += ingredients[i].flavor * amounts[i];
    texture += ingredients[i].texture * amounts[i];
    calories += ingredients[i].calories * amounts[i];
  }

  capacity = std::max(0, capacity);
  durability = std::max(0, durability);
  flavor = std::max(0, flavor);
  texture = std::max(0, texture);

  int score = capacity * durability * flavor * texture;
  return {score, calories};
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
          auto [score, calories] =
            calculateScoreAndCalories(ingredients, amounts);
          if (calories == 500) { best_score = std::max(best_score, score); }
        }
      } else {
        amounts[2] = 100 - amounts[0] - amounts[1];
        auto [score, calories] =
          calculateScoreAndCalories(ingredients, amounts);
        if (calories == 500) { best_score = std::max(best_score, score); }
      }
    }
  }

  int total = best_score;
  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
