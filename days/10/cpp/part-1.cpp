#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <utility>
#include <vector>

struct Point
{
  int x, y, dx, dy;
};

int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");
  
  const auto re = std::regex("position=< *(-?[0-9]+), *(-?[0-9]+)> velocity=< *(-?[0-9]+), *(-?[0-9]+)>");

  std::vector<Point> points;

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    std::smatch match;
    std::regex_match(line, match, re);
    const auto  x = std::stoi(match[1]);
    const auto  y = std::stoi(match[2]);
    const auto dx = std::stoi(match[3]);
    const auto dy = std::stoi(match[4]);
    points.emplace_back(x, y, dx, dy);
  }

  std::map<std::pair<int,int>, bool> map1, map2;
  auto* pm1 = &map1;
  auto* pm2 = &map2;
  auto prevW = 1e6;
  auto prevH = 1e6;

  for (auto i = 0; i < 100000; ++i) {
    pm1->clear();

    int minx =  1e6;
    int maxx = -1e6;
    int miny =  1e6;
    int maxy = -1e6;
    for (auto& p : points) {
      p.x += p.dx;
      p.y += p.dy;
      (*pm1)[{p.x, p.y}] = true;
      minx = std::min(minx, p.x);
      maxx = std::max(maxx, p.x);
      miny = std::min(miny, p.y);
      maxy = std::max(maxy, p.y);
    }

    auto W = maxx - minx + 1;
    auto H = maxy - miny + 1;
    if (W > prevW && H > prevH) {
      for (auto y = miny; y <= maxy; ++y) {
        for (auto x = minx; x <= maxx; ++x)
          std::cout << (((*pm2)[{x, y}]) ? '#' : '.');
        std::cout << std::endl;
      }
      break;
    }
    prevW = W;
    prevH = H;

    std::swap(pm1, pm2);
  }

  return EXIT_SUCCESS;
}

