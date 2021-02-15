#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>

int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  const std::regex re_guard{"#([0-9]+)"};
  const std::regex re_sleep{":([0-9]+).*falls asleep"};
  const std::regex re_wake{":([0-9]+).*wakes up"};

  std::map<int, int> guard_sleep_minutes;
  std::map<int, std::map<int, int>> sleep_minutes_map;

  std::vector<std::string> lines;

  int guard;
  int begin_sleep;
  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    lines.emplace_back(std::move(line));
  }

  std::sort(lines.begin(), lines.end());

  for (auto line : lines) {
    std::smatch match;
    if (std::regex_search(line, match, re_guard)) {
      guard = std::stoi(match[1]);
    } else if (std::regex_search(line, match, re_sleep)) {
      begin_sleep = std::stoi(match[1]);
    } else {
      std::regex_search(line, match, re_wake);
      for (auto i = begin_sleep; i < std::stoi(match[1]); ++i) {
        guard_sleep_minutes[guard]++;
        sleep_minutes_map[guard][i]++;
      }
    }
  }

  auto most_sleepy_guard = 0;
  auto most_sleepy_guard_most_sleepy_minute = 0;
  auto most_sleepy_guard_most_sleepy_slept = 0;
  for (auto [guard, _] : guard_sleep_minutes) {
    for (auto [minute, slept] : sleep_minutes_map[guard])
      if (slept > most_sleepy_guard_most_sleepy_slept) {
        most_sleepy_guard = guard;
        most_sleepy_guard_most_sleepy_minute = minute;
        most_sleepy_guard_most_sleepy_slept = slept;
      }
  }

  std::cout << most_sleepy_guard * most_sleepy_guard_most_sleepy_minute << std::endl;
  
  
  return EXIT_SUCCESS;
}

