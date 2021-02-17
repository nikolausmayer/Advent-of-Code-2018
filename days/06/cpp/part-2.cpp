#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  int minx = 9999;
  int maxx = 0;
  int miny = 9999;
  int maxy = 0;
  std::vector<std::pair<int,int>> seeds;
  while (true) {
    int x, y;
    char c;
    std::string s;
    infile >> x >> c >> y;
    std::getline(infile, s);
    if (infile.eof())
      break;
    seeds.push_back({x, y});
    minx = std::min(minx, x);
    maxx = std::max(maxx, x);
    miny = std::min(miny, y);
    maxy = std::max(maxy, y);
  }

  minx -= (10'000 / seeds.size());
  miny -= (10'000 / seeds.size());
  maxx += (10'000 / seeds.size());
  maxy += (10'000 / seeds.size());
  auto goodcount = 0;
  for (auto y = miny; y <= maxy; ++y) {
    for (auto x = minx; x <= maxx; ++x) {
      auto dist = 0;
      for (auto seed = 0u; seed < seeds.size(); ++seed) {
        dist += std::abs(x - seeds[seed].first) +
                std::abs(y - seeds[seed].second);
      }
      if (dist < 10'000)
        ++goodcount;
    }
  }

  std::cout << goodcount << std::endl;
  
  return EXIT_SUCCESS;
}

