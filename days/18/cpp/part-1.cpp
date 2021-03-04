#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>


int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::vector<std::vector<char>> map;
  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    std::vector<char> row;
    for (auto c : line)
      row.push_back(c);
    map.push_back(std::move(row));
  }
  
  auto print_map = [&]{
    for (const auto& line : map) {
      for (auto c : line)
        std::cout << c;
      std::cout << std::endl;
    }
  };

  for (auto round = 0; round < 10; round++) {
    auto newmap = map;
    for (auto y = 0; y < map.size(); y++) {
      for (auto x = 0; x < map[0].size(); x++) {

        auto trees = 0;
        auto yards = 0;
        for (auto dy = -1; dy <= 1; dy++) {
          for (auto dx = -1; dx <= 1; dx++) {
            if (dx == 0 and dy == 0)
              continue;
            const auto tx = x+dx;
            const auto ty = y+dy;
            if (tx < 0 or tx >= map[0].size() or
                ty < 0 or ty >= map.size())
              continue;
            trees += (map[ty][tx] == '|');
            yards += (map[ty][tx] == '#');
          }
        }

        const auto m = map[y][x];
        if (m == '.' and trees >= 3)
          newmap[y][x] = '|';
        else if (m == '|' and yards >= 3)
          newmap[y][x] = '#';
        else if (m == '#' and (trees < 1 or yards < 1))
          newmap[y][x] = '.';
      }
    }
    map = newmap;
  }

  auto trees = 0;
  auto yards = 0;
  for (const auto& line : map) {
    for (auto c : line) {
      trees += (c == '|');
      yards += (c == '#');
    }
  }
  std::cout << trees * yards << std::endl;

  return EXIT_SUCCESS;
}

