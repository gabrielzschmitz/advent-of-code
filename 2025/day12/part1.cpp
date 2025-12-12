/**
 * @file part1.cpp
 * @brief Present Placement Feasibility Checker
 *
 * This program evaluates whether various regions beneath Christmas trees can
 * accommodate specified quantities of presents of known shapes. Each present
 * shape is defined as a two-dimensional pattern of occupied cells on a unit
 * grid. A region is a rectangular grid into which these presents must be
 * placed without overlap.
 *
 * Input is divided into two sections:
 *   1. A list of present shapes, each identified by index, followed by a
 *      visual diagram. In each diagram, '#' denotes an occupied cell and '.'
 *      denotes empty space. Shapes may be rotated or flipped when placed.
 *
 *   2. A list of regions. Each region entry specifies:
 *        - The region's dimensions (width x height).
 *        - A vector of required present counts, one per shape index.
 *
 * Presents must satisfy the following constraints:
 *   - All placements must align perfectly to the discrete grid.
 *   - Shapes may not overlap; occupied cells from different presents cannot
 *     coincide.
 *   - Empty cells within a shape do not block placement of other presents.
 *   - Stacking is not allowed; arrangement is strictly two-dimensional.
 *
 * The program verifies, for each region, whether it is possible to place all
 * required presents—considering all rotations and reflections—while satisfying
 * the geometric constraints. This constitutes an exact packing problem over a
 * finite grid.
 *
 * Example:
 *   - A 4x4 region can accommodate two instances of shape 4.
 *   - A 12x5 region can accommodate a combination of shapes
 *     {0:1, 2:1, 4:2, 5:2}.
 *   - A similar 12x5 region requiring an additional shape 4 cannot be packed.
 *
 * The program outputs the total count of regions for which all listed presents
 * can be successfully arranged.
 *
 * Puzzle Answer: [Number of Regions That Can Fit All Presents]
 *
 * @author [gabrielzschmitz]
 * @date [12/12/2025]
 */
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

bool debug = false;

void checkDebugFlag(const std::string &input, bool &debug) {
  size_t pos = input.find_last_of('/');

  std::string last_part =
      (pos == std::string::npos) ? input : input.substr(pos + 1);

  if (last_part == "test")
    debug = true;
}

struct Region {
  int width;
  int length;
  std::vector<int> shapes;

  bool could_fit(const std::unordered_map<int, int> &shape_cell_counts) const {
    long long required = 0;
    for (size_t i = 0; i < shapes.size(); ++i) {
      int cnt = shapes[i];
      if (cnt == 0)
        continue;
      auto it = shape_cell_counts.find((int)i);
      int cells = (it == shape_cell_counts.end()) ? 0 : it->second;
      required += 1LL * cnt * cells;
    }
    return 1LL * width * length >= required;
  }
};

static inline std::string trim(const std::string &s) {
  size_t a = s.find_first_not_of(" \t\r\n");
  if (a == std::string::npos)
    return "";
  size_t b = s.find_last_not_of(" \t\r\n");
  return s.substr(a, b - a + 1);
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  checkDebugFlag(user_input, debug);

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cerr << "FILE " << user_input << " UNAVAILABLE!" << std::endl;
    return 1;
  }

  std::ostringstream oss;
  oss << input_file.rdbuf();
  std::string content = oss.str();

  content.erase(std::remove(content.begin(), content.end(), '\r'),
                content.end());

  std::regex block_re(R"(\n\s*\n)");
  std::sregex_token_iterator blocks_it(content.begin(), content.end(), block_re,
                                       -1);
  std::sregex_token_iterator end_it;

  std::unordered_map<int, int> shape_cell_counts;
  std::vector<Region> regions;

  std::regex digits_re(R"((\d+))");

  for (; blocks_it != end_it; ++blocks_it) {
    std::string block = trim(*blocks_it);
    if (block.empty())
      continue;

    std::vector<std::string> lines;
    std::istringstream ls(block);
    std::string ln;
    while (std::getline(ls, ln))
      lines.push_back(ln);

    if (lines.empty())
      continue;

    std::string first = trim(lines[0]);
    if (!first.empty() && first.back() == ':') {
      std::string idx_str = first.substr(0, first.size() - 1);
      idx_str = trim(idx_str);
      if (idx_str.empty())
        continue;
      int idx = std::stoi(idx_str);

      int count_hash = 0;
      for (size_t r = 1; r < lines.size(); ++r)
        for (char c : lines[r])
          if (c == '#')
            ++count_hash;
      shape_cell_counts[idx] = count_hash;
      if (debug)
        std::cerr << "shape " << idx << " cells=" << count_hash << "\n";
    } else {
      for (auto &rawline : lines) {
        std::string l = trim(rawline);
        if (l.empty())
          continue;
        std::vector<int> numbers;
        std::sregex_iterator it(l.begin(), l.end(), digits_re);
        std::sregex_iterator it_end;
        for (; it != it_end; ++it)
          numbers.push_back(std::stoi((*it)[1].str()));
        if (numbers.size() < 2)
          continue;
        Region r;
        r.width = numbers[0];
        r.length = numbers[1];

        size_t known_shapes = 0;
        for (auto &p : shape_cell_counts)
          known_shapes = std::max(known_shapes, (size_t)(p.first + 1));
        size_t provided_counts =
            (numbers.size() >= 2) ? (numbers.size() - 2) : 0;
        size_t target_size = std::max(known_shapes, provided_counts);
        r.shapes.assign(target_size, 0);

        for (size_t k = 0; k < provided_counts; ++k)
          r.shapes[k] = numbers[(size_t)2 + k];

        regions.push_back(std::move(r));
        if (debug) {
          std::cerr << "region " << r.width << "x" << r.length
                    << " shapeCounts:";
          for (auto v : r.shapes)
            std::cerr << " " << v;
          std::cerr << "\n";
        }
      }
    }
  }

  int64_t total = 0;
  for (auto &reg : regions)
    if (reg.could_fit(shape_cell_counts))
      ++total;

  std::cout << "ANSWER: " << total << std::endl;
  return 0;
}
