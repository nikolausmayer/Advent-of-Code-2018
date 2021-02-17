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

  std::vector<int> areas(seeds.size());
  std::vector<bool> infinite(seeds.size());
  std::fill(infinite.begin(), infinite.end(), false);
  for (auto y = miny; y <= maxy; ++y) {
    for (auto x = minx; x <= maxx; ++x) {
      auto mindist = 9999;
      auto minseed = 0;
      auto samecount = 0;
      for (auto seed = 0u; seed < seeds.size(); ++seed) {
        auto dist = std::abs(x - seeds[seed].first) +
                    std::abs(y - seeds[seed].second);
        if (dist == mindist) {
          ++samecount;
        } else if (dist < mindist) {
          mindist = dist;
          minseed = seed;
          samecount = 0;
        }
      }
      if (samecount == 0)
        areas[minseed]++;
      if (x == minx or x == maxx or y == miny or y == maxy)
        infinite[minseed] = true;
    }
  }

  auto max_finite_area = 0;
  for (auto seed = 0u; seed < seeds.size(); ++seed)
    if (not infinite[seed])
      max_finite_area = std::max(max_finite_area, areas[seed]);
  std::cout << max_finite_area << std::endl;
  
  return EXIT_SUCCESS;
}

