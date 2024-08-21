/**
 * @brief A build tool for the Advent of Code challenge.
 * 
 * This utility program is designed to build and optionally run a C++ source
 * file specific to the Advent of Code challenge. It compiles the source file
 * located in a directory structure based on the provided year, day, and part.
 * The program also optionally executes the resulting binary with a specified
 * input or test file if the appropriate flag is set.
 * 
 * @details
 * Usage: ./build <year> <day> <part> [<input|test>] [--run]
 * 
 * - <year>: A 4-digit number specifying the year (e.g., 2023).
 * - <day>: A 2-digit number specifying the day (e.g., 24).
 * - <part>: The part number, either 1 or 2.
 * - <input|test>: Optional file type, "input" or "test". Defaults to "input".
 * - [--run]: Optional flag to run the compiled executable. If provided, 
 *   the executable will be executed with the specified input or test file.
 * 
 * The program performs input validation, creates necessary directories,
 * compiles the source file using g++, and optionally runs the resulting
 * executable. To compile this build tool itself, use the following command:
 * 
 * g++ -std=c++11 -o build build.cpp
 * 
 * @example
 * ./build_run 2023 24 1 test --run
 * This command compiles the source file `2023/day24/part1.cpp`, places the
 * executable in `2023/day24/build/part1`, and runs it with `2023/day24/test` as
 * input.
 */
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

// Function to check if a directory exists and create it if it does not
bool createDirectory(const std::string& path) {
  struct stat info;
  if (stat(path.c_str(), &info) != 0) {
    if (mkdir(path.c_str(), 0755) != 0) {
      std::cerr << "Error creating directory: " << path << std::endl;
      return false;
    }
  } else if (!(info.st_mode & S_IFDIR)) {
    std::cerr << "Path exists but is not a directory: " << path << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  // Check for correct number of arguments
  if (argc < 4 || argc > 6) {
    std::cerr << "Usage: " << argv[0]
              << " <year> <day> <part> [<input|test>] [--run]" << std::endl;
    return 1;
  }

  std::string year = argv[1];
  std::string day = argv[2];
  std::string part = argv[3];
  std::string file_type = "input";
  bool run = false;

  // Check for optional arguments
  if (argc >= 5) {
    if (std::string(argv[4]) == "--run") {
      run = true;
    } else {
      file_type = argv[4];
    }
  }
  if (argc == 6 && std::string(argv[5]) == "--run") { run = true; }

  // Validate inputs
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

  // Set directories and files
  std::string day_dir = year + "/day" + day;
  std::string build_dir = day_dir + "/build";
  std::string source_file = day_dir + "/part" + part + ".cpp";
  std::string output_file = build_dir + "/part" + part;

  // Create build directory if it does not exist
  if (!createDirectory(build_dir)) { return 1; }

  // Compile the source file
  std::string compile_command = "g++ " + source_file + " -o " + output_file;
  if (system(compile_command.c_str()) != 0) {
    std::cerr << "Build failed." << std::endl;
    return 1;
  }

  // Run the application with the specified input or test file, if run flag is set
  if (run) {
    std::string input_file = day_dir + "/" + file_type;
    std::ifstream file(input_file);
    if (!file.good()) {
      std::cerr << "Input file or test file not found: " << input_file
                << std::endl;
      return 1;
    }

    std::string run_command = output_file + " " + input_file;
    if (system(run_command.c_str()) != 0) {
      std::cerr << "Error running the application." << std::endl;
      return 1;
    }
  }

  return 0;
}
