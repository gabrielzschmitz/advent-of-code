#include <curl/curl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

const std::string BASE_URL = "https://adventofcode.com";
const std::string SESSION_FILE = "./aoc/aoc.session";
const std::string ARGS_FILE = "./aoc/aoc_args.tmp";
std::string AOC_SESSION;

// ANSI color codes
constexpr const char* COLOR_RESET = "\033[0m";
constexpr const char* COLOR_WHITE = "\033[1;37m";
constexpr const char* COLOR_BLUE = "\033[1;34m";
constexpr const char* COLOR_GREEN = "\033[1;32m";
constexpr const char* COLOR_YELLOW = "\033[1;33m";
constexpr const char* COLOR_RED = "\033[1;31m";
constexpr const char* STYLE_BOLD = "\033[1m";

std::string current_timestamp() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::ostringstream ss;

  ss << COLOR_WHITE << "["
     << std::put_time(std::localtime(&time), "%Y-%m-%d|%H:%M:%S") << "]"
     << COLOR_RESET;
  return ss.str();
}

void log_info(const std::string& msg) {
  std::cout << current_timestamp() << " " << COLOR_BLUE << STYLE_BOLD
            << "[INFO]" << COLOR_RESET << " " << msg << std::endl;
}

void log_success(const std::string& msg) {
  std::cout << current_timestamp() << " " << COLOR_GREEN << STYLE_BOLD
            << "[SUCCESS]" << COLOR_RESET << " " << msg << std::endl;
}

void log_warn(const std::string& msg) {
  std::cout << current_timestamp() << " " << COLOR_YELLOW << STYLE_BOLD
            << "[WARNING]" << COLOR_RESET << " " << msg << std::endl;
}

void log_error(const std::string& msg) {
  std::cerr << current_timestamp() << " " << COLOR_RED << STYLE_BOLD
            << "[ERROR]" << COLOR_RESET << " " << msg << std::endl;
}

bool file_exists(const std::string& filename) {
  std::ifstream f(filename);
  return f.good();
}

std::string read_file(const std::string& filename) {
  std::ifstream file(filename);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

size_t write_data(void* ptr, size_t size, size_t nmemb, void* userdata) {
  std::ofstream* stream = static_cast<std::ofstream*>(userdata);
  size_t total_size = size * nmemb;
  stream->write(static_cast<char*>(ptr), total_size);
  return total_size;
}

std::string curl_get(const std::string& url) {
  CURL* curl = curl_easy_init();
  std::stringstream response;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    std::string cookie = "session=" + AOC_SESSION;
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
    curl_easy_setopt(
      curl, CURLOPT_WRITEFUNCTION,
      +[](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
        ((std::stringstream*)userp)->write((char*)contents, size * nmemb);
        return size * nmemb;
      });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl");
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  return response.str();
}

void get_input(const std::string& year, const std::string& day_str) {
  log_info("Fetching input for year " + year + ", day " + day_str);

  std::string padded_day = day_str.length() < 2 ? "0" + day_str : day_str;
  std::string url = BASE_URL + "/" + year + "/day/" +
                    std::to_string(std::stoi(day_str)) + "/input";
  std::string dir_path = "./" + year + "/day" + padded_day;
  std::string file_path = dir_path + "/input";

  CURL* curl = curl_easy_init();
  std::ofstream out(file_path);
  if (curl && out) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    std::string cookie = "session=" + AOC_SESSION;
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl");
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    log_success("Puzzle input saved to " + file_path);
  } else {
    log_error("Failed to fetch puzzle input.");
  }
}

void submit_answer(const std::string& year, const std::string& day_str,
                   const std::string& part, const std::string& answer) {
  log_info("Submitting answer for year " + year + ", day " + day_str +
           ", part " + part);
  log_info("Answer: " + answer);

  CURL* curl = curl_easy_init();
  std::stringstream response;
  if (curl) {
    std::string padded_day = day_str.length() < 2 ? "0" + day_str : day_str;
    std::string url = BASE_URL + "/" + year + "/day/" +
                      std::to_string(std::stoi(day_str)) + "/answer";
    std::string post_fields = "level=" + part + "&answer=" + answer;
    std::string cookie = "session=" + AOC_SESSION;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    curl_easy_setopt(
      curl, CURLOPT_WRITEFUNCTION,
      +[](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
        ((std::stringstream*)userp)->write((char*)contents, size * nmemb);
        return size * nmemb;
      });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl");
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::regex article_regex("<article.*?>(.*?)</article>",
                             std::regex::ECMAScript);
    std::smatch match;
    std::string html = response.str();
    if (std::regex_search(html, match, article_regex)) {
      std::string message =
        std::regex_replace(match[1].str(), std::regex("<[^>]*>"), "");
      if (message.find("That's the right answer") != std::string::npos)
        log_success(message);
      else if (message.find("That's not the right answer") != std::string::npos)
        log_error(message);
      else { log_warn("Server response:\n" + message); }
    } else {
      log_error("Failed to parse server response.");
    }
  }
}

void get_prompt(const std::string& year, const std::string& day_str) {
  log_info("Fetching prompt for year " + year + ", day " + day_str);

  // Zero-pad if needed
  std::string padded_day = day_str.length() < 2 ? "0" + day_str : day_str;

  std::string url =
    BASE_URL + "/" + year + "/day/" + std::to_string(std::stoi(day_str));
  std::string html = curl_get(url);

  html = std::regex_replace(html, std::regex("\n"), " ");
  std::regex article_regex("<article.*?>(.*?)</article>",
                           std::regex::ECMAScript);
  std::smatch match;

  if (!std::regex_search(html, match, article_regex)) {
    log_error("Failed to fetch prompt.");
    return;
  }

  std::string article_html = "<article>" + match[1].str() + "</article>";

  std::string dir_path = "./" + year + "/day" + padded_day;
  std::string filename = dir_path + "/prompt.md";

  // Try html2text
  if (system("command -v html2text > /dev/null") == 0) {
    std::string command = "html2text > \"" + filename + "\"";
    FILE* pipe = popen(command.c_str(), "w");
    if (pipe) {
      fwrite(article_html.c_str(), 1, article_html.size(), pipe);
      pclose(pipe);
      log_success("Problem prompt saved to " + filename);
    }
  }
  // Try pandoc
  else if (system("command -v pandoc > /dev/null") == 0) {
    std::string command = "pandoc -f html -t markdown -o \"" + filename + "\"";
    FILE* pipe = popen(command.c_str(), "w");
    if (pipe) {
      fwrite(article_html.c_str(), 1, article_html.size(), pipe);
      pclose(pipe);
      log_success("Problem prompt saved to " + filename);
    }
  } else {
    log_error("Install 'html2text' or 'pandoc' to convert prompt to Markdown.");
  }
}

void show_help() {
  std::cout << "Usage:\n"
            << "  ./aoc input\n"
            << "  ./aoc prompt\n"
            << "  ./aoc submit\n"
            << "Ensure './aoc.session' contains your session cookie.\n"
            << "And './aoc_args.temp' contains the problem to submit.\n";
}

int main(int argc, char* argv[]) {
  if (!file_exists(SESSION_FILE)) {
    log_error("Session file '" + SESSION_FILE + "' not found.");
    std::cerr
      << "   Please create this file with your Advent of Code session cookie:\n"
      << "   echo 'your_cookie_here' > " << SESSION_FILE << "\n";
    return 1;
  }
  if (!file_exists(ARGS_FILE)) {
    log_error("Args file '" + ARGS_FILE + "' not found.");
    std::cerr
      << "   Please create this file the Advent of Code you want to run:\n"
      << "   echo 'year day part' > " << ARGS_FILE << "\n";
    return 1;
  }

  AOC_SESSION = read_file(SESSION_FILE);
  AOC_SESSION.erase(AOC_SESSION.find_last_not_of(" \n\r\t") + 1);

  if (argc < 2) {
    show_help();
    return 1;
  }
  std::string command = argv[1];

  std::ifstream in(ARGS_FILE);
  std::string year, part;
  int day_int;

  std::string line1, line2;
  std::getline(in, line1);
  std::getline(in, line2);

  std::istringstream args_stream(line1);
  args_stream >> year >> day_int >> part;
  std::string day = (day_int < 10 ? "0" : "") + std::to_string(day_int);

  if (command == "input") {
    get_input(year, day);
  } else if (command == "prompt") {
    get_prompt(year, day);
  } else if (command == "submit") {
    if (line2.rfind("ANSWER: ", 0) == 0) {
      std::string answer = line2.substr(8);
      submit_answer(year, day, part, answer);
    } else
      log_error("Invalid input format. Expected line starting with 'ANSWER: '");
  } else {
    log_error("Unknown command: " + command);
    show_help();
  }

  return 0;
}
