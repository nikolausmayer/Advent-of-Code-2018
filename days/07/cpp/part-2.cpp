#include <algorithm>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_set>
#include <stdexcept>

int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::map<char, int> left;
  std::map<char, std::unordered_set<char>> dependencies;
  std::deque<char> queue;
  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    dependencies[line[36]].insert(line[5]);
    left[line[5]] = line[5] - 'A' + 61;
    left[line[36]] = line[36] - 'A' + 61;
  }
  for (auto [c, _] : left)
    if (dependencies[c].size() == 0)
      queue.push_back(c);
  std::sort(std::begin(queue), std::end(queue));

  std::unordered_set<char> in_flight;
  auto free_workers = 5;

  auto tick = 0;
  while (std::any_of(left.cbegin(), left.cend(), 
         [](auto& p){ return p.second > 0; })) {
    for (auto [c, i] : left) {
      if (not in_flight.contains(c))
        continue;

      if (i == 0) {
        in_flight.erase(c);
        free_workers++;
        for (auto [c2, _] : left) {
          dependencies[c2].erase(c);
          if (dependencies[c2].size() == 0 and
              left[c2] > 0 and 
              not in_flight.contains(c2) and
              std::find(queue.cbegin(), queue.cend(), c2) == queue.cend()) {
            queue.push_back(c2);
          }
        }
      }
    }
    std::sort(std::begin(queue), std::end(queue));

    while (free_workers > 0 and queue.size()) {
      auto next = queue.front();
      queue.pop_front();
      in_flight.insert(next);
      free_workers--;
    }

    for (auto c : in_flight)
      left[c]--;

    tick++;
  }

  std::cout << tick << std::endl;
  
  return EXIT_SUCCESS;
}

