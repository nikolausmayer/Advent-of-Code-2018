#include <cstdlib>
#include <cstdio>
#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>


int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::vector<std::array<int,4>> coordinates;

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    
    int a,b,c,d;
    std::sscanf(line.c_str(), "%d,%d,%d,%d", &a, &b, &c, &d);
    coordinates.push_back({a,b,c,d});
  }

  std::vector<std::vector<std::size_t>> connections(coordinates.size());
  for (auto i = 0u; i < coordinates.size(); i++) {
    for (auto j = i+1; j < coordinates.size(); j++) {
      auto [x1,y1,z1,t1] = coordinates[i];
      auto [x2,y2,z2,t2] = coordinates[j];
      auto dist = std::abs(x1-x2) + std::abs(y1-y2) +
                  std::abs(z1-z2) + std::abs(t1-t2);
      if (dist <= 3) {
        connections[i].push_back(j);
        connections[j].push_back(i);
      }
    }
  }

  std::set<size_t> candidates;
  for (auto i = 0u; i < coordinates.size(); i++)
    candidates.insert(i);

  std::vector<std::set<size_t>> constellations;
  while (candidates.size()) {
    auto next = *candidates.begin();
    std::set<size_t> new_constellation{next};
    candidates.erase(next);

    bool extended = true;
    while (extended) {
      extended = false;
      for (auto star : new_constellation) {
        if (extended)
          break;
        for (auto neighbor : connections[star]) {
          if (candidates.contains(neighbor)) {
            new_constellation.insert(neighbor);
            candidates.erase(neighbor);
            extended = true;
            break;
          }
        }
      }
    }
    constellations.push_back(std::move(new_constellation));
  }

  std::cout << constellations.size() << std::endl;

  return EXIT_SUCCESS;
}

