/**
 * @brief A benchmarking tool for Advent of Code solutions.
 *
 * This tool measures the execution time of a specific Advent of Code solution.
 * If the solution is not built, it uses the existing build tool to compile it
 * before benchmarking. The tool can run the solution multiple times and compute
 * the total time, average execution time and the mean execution time.
 *
 * @details
 * Usage: ./benchmark <year> <day> <part> [<input|test>] [<runs>]
 *
 * - <year>: A 4-digit number specifying the year (e.g., 2023).
 * - <day>: A 2-digit number specifying the day (e.g., 24).
 * - <part>: The part number, either 1 or 2.
 * - <input|test>: Optional file type, "input" or "test". Defaults to "input".
 * - <runs>: Optional number of runs for benchmarking. Defaults to 1000.
 *
 * To compile this tool, use the following command:
 * 
 * g++ -std=c++11 -o benchmark benchmark.cpp
 * 
 * @example
 * ./benchmark 2023 24 1 test run 10000
 * This command will compile the source file `2023/day24/part1.cpp` if it is not
 * already built, and runs it with `2023/day24/test` as input 10000 times, then
 * show the user the statistics generated during benchmark.
 */
#include <sys/stat.h>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

bool fileExists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

int main(int argc, char* argv[]) {
  if (argc < 4 || argc > 6) {
    std::cerr << "Usage: " << argv[0]
              << " <year> <day> <part> [<input|test>] [<runs>]" << std::endl;
    return 1;
  }

  std::string year = argv[1];
  std::string day = argv[2];
  std::string part = argv[3];
  std::string file_type = (argc >= 5) ? argv[4] : "input";
  int runs = (argc == 6) ? std::stoi(argv[5]) : 1000;

  if (year.size() != 4 || !std::all_of(year.begin(), year.end(), ::isdigit)) {
    std::cerr << "Invalid year. Must be a 4-digit number." << std::endl;
    return 1;
  }
  if (day.size() != 2 || !std::all_of(day.begin(), day.end(), ::isdigit)) {
    std::cerr << "Invalid day. Must be a 2-digit number." << std::endl;
    return 1;
  }
  if (part != "1" && part != "2") {
    std::cerr << "Invalid part. Must be 1 or 2." << std::endl;
    return 1;
  }
  if (file_type != "input" && file_type != "test") {
    std::cerr << "Invalid file type. Must be 'input' or 'test'." << std::endl;
    return 1;
  }
  if (runs <= 0) {
    std::cerr << "Number of runs must be greater than 0." << std::endl;
    return 1;
  }

  std::string day_dir = year + "/day" + day;
  std::string build_dir = day_dir + "/build";
  std::string executable = build_dir + "/part" + part;
  std::string input_file = day_dir + "/" + file_type;

  if (!fileExists(executable)) {
    std::cerr << "Executable not found: " << executable << std::endl;
    std::cerr << "Attempting to build using the build tool..." << std::endl;

    std::ostringstream build_command;
    build_command << "./build " << year << " " << day << " " << part << " "
                  << file_type;
    if (system(build_command.str().c_str()) != 0) {
      std::cerr << "Build failed. Cannot proceed with benchmarking."
                << std::endl;
      return 1;
    }
  }

  if (!fileExists(input_file)) {
    std::cerr << "Input file or test file not found: " << input_file
              << std::endl;
    return 1;
  }

  std::ostringstream run_command;
  run_command << executable << " " << input_file << " > /dev/null 2>&1";

  std::cout << "Benchmarking: " << run_command.str() << "\n" << std::endl;
  std::vector<double> durations;

  for (int i = 0; i < runs; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    if (system(run_command.str().c_str()) != 0) {
      std::cerr << "Error running the application." << std::endl;
      return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    durations.push_back(elapsed.count());
  }

  double total_time = std::accumulate(durations.begin(), durations.end(), 0.0);
  double average_time = total_time / runs;

  std::sort(durations.begin(), durations.end());
  double median_time = (runs % 2 == 0)
                         ? (durations[runs / 2 - 1] + durations[runs / 2]) / 2.0
                         : durations[runs / 2];

  std::cout << "Total execution time (for " << runs << " runs): " << total_time
            << " ms" << std::endl;
  std::cout << "Execution time (average over " << runs
            << " runs): " << average_time << " ms" << std::endl;
  std::cout << "Median execution time: " << median_time << " ms" << std::endl;

  return 0;
}
