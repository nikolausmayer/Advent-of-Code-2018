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

  std::unordered_set<char> left;
  std::map<char, std::unordered_set<char>> dependencies;
  std::deque<char> queue;
  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    dependencies[line[36]].insert(line[5]);
    left.insert(line[5]);
    left.insert(line[36]);
  }
  for (auto c : left)
    if (dependencies[c].size() == 0)
      queue.push_back(c);
  for (auto c : queue)
    left.erase(c);
  std::sort(std::begin(queue), std::end(queue));

  while (queue.size()) {
    auto next = queue.front();
    queue.pop_front();
    left.erase(next);

    std::cout << next;

    for (auto c : left) {
      dependencies[c].erase(next);
      if (dependencies[c].size() == 0 and
          std::find(queue.cbegin(), queue.cend(), c) == queue.cend())
        queue.push_back(c);
    }
    std::sort(std::begin(queue), std::end(queue));
  }
  std::cout << std::endl;
  
  return EXIT_SUCCESS;
}

