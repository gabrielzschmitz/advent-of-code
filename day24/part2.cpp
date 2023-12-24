/**
 * @file part2.cpp
 * @brief Never Tell Me The Odds - Hailstones Perfect Collision Position
 * Calculation
 *
 * Given the initial positions and velocities of hailstones, find the exact
 * position and velocity for a rock at time 0 so that it perfectly collides with
 * every hailstone. Return the sum of the X, Y, and Z coordinates of that
 * initial position.
 *
 * Example Input:
 * 19, 13, 30 @ -2,  1, -2
 * 18, 19, 22 @ -1, -1, -2
 * 20, 25, 34 @ -2, -2, -4
 * 12, 31, 28 @ -1, -2, -1
 * 20, 19, 15 @  1, -5, -3
 *
 * Example Output:
 * Count of hailstones' paths intersections within the specified test area:
 * 47
 *
 * Determine the exact position and velocity for the rock at time 0 so that it
 * perfectly collides with every hailstone.
 *
 * @author [gabrielzschmitz]
 * @date [24/12/2023]
 */
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

bool debug = false;

struct Hail {
  Eigen::Vector3d pos;
  Eigen::Vector3d vel;
};

std::vector<Hail> getHails(std::ifstream &inf) {
  std::vector<Hail> hails;
  std::string line;
  while (std::getline(inf, line)) {
    std::string posStr{line.substr(0, line.find('@') - 1)};
    std::string velStr(line.substr(line.find('@') + 2));
    Eigen::Vector3d pos;
    Eigen::Vector3d vel;

    for (int i = 0; i < 3; ++i) {
      pos[i] = std::stod(posStr.substr(0, posStr.find(',')));
      vel[i] = std::stod(velStr.substr(0, velStr.find(',')));

      posStr = posStr.substr(posStr.find(',') + 1);
      velStr = velStr.substr(velStr.find(',') + 1);
    }

    Hail hail{pos, vel};
    hails.push_back(hail);
  }

  return hails;
}

Eigen::Matrix3d getCoeffMat(const Eigen::Vector3d &v) {
  Eigen::Matrix3d coeffMat;
  coeffMat << 0, -v[2], v[1], v[2], 0, -v[0], -v[1], v[0], 0;
  return coeffMat;
}

int main(int argc, char *argv[]) {
  std::string user_input = (argc == 2) ? argv[1] : "input";
  if (debug) user_input = "test";

  std::ifstream input_file(user_input);
  if (!input_file.is_open()) {
    std::cout << "ARQUIVO " << user_input << " INACESSIVEL!" << std::endl;
    return -1;
  }

  int64_t total = 0;
  std::vector<Hail> hails{getHails(input_file)};

  // Numerical stability issues meant I had to take a majority vote
  std::unordered_map<long, int> freqs;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, hails.size() - 1);

  for (int i = 0; i < 100; ++i) {
    int p0{distr(gen)};
    int p1{distr(gen)};
    int p2{distr(gen)};
    int p3{distr(gen)};

    if (p0 == p1 || p0 == p2 || p0 == p3 || p1 == p2 || p1 == p3 || p2 == p3) {
      continue;
    }
    Eigen::MatrixXd equations(6, 6);
    Eigen::VectorXd rhs(6);

    rhs.segment<3>(0) =
        hails[p0].pos.cross(hails[p0].vel) - hails[p1].pos.cross(hails[p1].vel);
    rhs.segment<3>(3) =
        hails[p2].pos.cross(hails[p2].vel) - hails[p3].pos.cross(hails[p3].vel);

    equations.block<3, 3>(0, 0) = getCoeffMat(hails[p1].vel - hails[p0].vel);
    equations.block<3, 3>(3, 0) = getCoeffMat(hails[p3].vel - hails[p2].vel);
    equations.block<3, 3>(0, 3) = getCoeffMat(hails[p0].pos - hails[p1].pos);
    equations.block<3, 3>(3, 3) = getCoeffMat(hails[p2].pos - hails[p3].pos);

    Eigen::VectorXd result = equations.inverse() * rhs;

    long sum{0};
    for (int i = 0; i < 3; ++i) {
      sum += std::roundl(result[i]);
    }
    freqs[sum]++;
  }

  int maxFreq{0};
  long sum{0};
  for (auto it = freqs.begin(); it != freqs.end(); ++it) {
    const auto &entry = *it;
    const long k = entry.first;
    const int v = entry.second;
    if (v > maxFreq) {
      maxFreq = v;
      sum = k;
    }
  }

  total = sum;
  std::cout << "NUMBER:" << total << "\n";

  input_file.close();
  return 0;
}
