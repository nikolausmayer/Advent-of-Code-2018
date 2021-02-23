#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");
  
  std::string line;
  std::getline(infile, line);
  line = line.substr(15);

  int left = 0;
  int right = line.size()-1;
  std::map<int, bool> state;
  for (auto i = 0u; i < line.size(); ++i)
    state[i] = (line[i] == '#');

  std::getline(infile, line);
  std::vector<std::array<bool, 6>> rules;
  while (true) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    std::array<bool, 6> rule;
    for (auto i = 0u; i < 5; ++i)
      rule[i] = (line[i] == '#');
    rule[5] = (line[line.size()-1] == '#');
    rules.emplace_back(std::move(rule));
  }

  for (auto round = 0; round < 300; ++round) {
    auto next_state = state;
    for (auto i = left-2; i <= right+2; ++i) {
      next_state[i] = false;
      for (const auto& rule : rules) {
        bool match = true;
        for (auto j = 0; j < 5; ++j) {
          if (state[i+j-2] != rule[j]) {
            match = false;
            break;
          }
        }
        if (match) {
          next_state[i] = rule[5];
          break;
        }
      }
    }

    state = next_state;
    left -= 2;
    right += 2;

    /// Looking at the output, we can see that it becomes static
    /// after ca 200 iterations. The rest can be extrapolated!

    //std::cout << round << ": ";
    //for (auto i = -5; i <= 300; ++i)
    //  std::cout << (state[i] ? '#' : '.');
    //std::cout << std::endl;
  }

  auto sum = 0ull;
  for (auto i = left; i <= right; ++i)
    if (state[i])
      sum += (50'000'000'000 - 300) + i;
  std::cout << sum << std::endl;

  return EXIT_SUCCESS;
}

