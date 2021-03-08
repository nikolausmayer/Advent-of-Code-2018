#include <array>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
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

  int depth, tx, ty;
  {
    std::string s;
    char c;
    infile >> s >> depth;
    infile >> s >> tx >> c >> ty;
  }


  std::map<std::pair<int,int>, long long> geoIndex;
  std::function<int(int,int)> eroLevel;
  eroLevel = [&](auto x, auto y) {
    if (x == tx and y == ty) {
      geoIndex[{x,y}] = 0;
    } else if (x == 0) {
      geoIndex[{x,y}] = y * 48271;
    } else if (y == 0) {
      geoIndex[{x,y}] = x * 16807;
    } else {
      auto a = geoIndex.contains({x-1,y})
                ? (geoIndex[{x-1,y}] + depth) % 20183
                : eroLevel(x-1,y);
      auto b = geoIndex.contains({x,y-1})
                ? (geoIndex[{x,y-1}] + depth) % 20183
                : eroLevel(x,y-1);
      geoIndex[{x,y}] = a * b;
    }
    return (geoIndex[{x,y}] + depth) % 20183;
  };


  /// cost, heuristic, x, y, equipment
  /// equipment:
  ///   0: none
  ///   1: torch
  ///   2: climbing_gear
  typedef std::array<int,5> T;
  auto cmp = [](const T& a, const T& b) {
    return (a[0] + a[1]) > (b[0] + b[1]);
  };

  std::priority_queue<T, std::vector<T>, decltype(cmp)> queue(cmp);
  std::set<std::array<int,3>> visited;

  ////std::map<T,T> history;

  auto heu = [tx, ty](int x, int y, int e) {
    return std::abs(tx - x) + std::abs(ty - y) + 7 * (e != 1);
  };

  queue.push({0, heu(0, 0, 1), 0, 0, 1});

  while (queue.size()) {
    auto [c, h, x, y, e] = queue.top();
    queue.pop();

    if (visited.contains({x, y, e}))
      continue;
    visited.insert({x, y, e});

    if (x == tx and y == ty and e == 1) {
      std::cout << c << std::endl;
      //while (history.contains({c, h, x, y, e})) {
      //  std::cout << c << " " << h << " " << x << " " << y << " " << e << std::endl;
      //  auto t = history[{c, h, x, y, e}];
      //  c = t[0]; h = t[1]; x = t[2]; y = t[3]; e = t[4];
      //}
      //std::cout << c << " " << h << " " << x << " " << y << " " << e << std::endl;
      break;
    }

    auto lvl = eroLevel(x, y) % 3;
    if (lvl == 0) {
      queue.push({c+7, heu(x,y,3-e), x, y, 3-e}); // 2->1 or 1->2
      //history[{c+7, heu(x,y,3-e), x, y, 3-e}] = {c, h, x, y, e};
    } else if (lvl == 1) {
      queue.push({c+7, heu(x,y,2-e), x, y, 2-e});
      //history[{c+7, heu(x,y,2-e), x, y, 2-e}] = {c, h, x, y, e};
    } else if (lvl == 2) {
      queue.push({c+7, heu(x,y,1-e), x, y, 1-e});
      //history[{c+7, heu(x,y,1-e), x, y, 1-e}] = {c, h, x, y, e};
    }

    for (auto dy : {-1, 0, 1}) {
      if (y+dy < 0)
        continue;
      for (auto dx : {-1, 0, 1}) {
        if (x+dx < 0)
          continue;
        if ((dx == 0) == (dy == 0))
          continue;
        auto nlvl = eroLevel(x+dx, y+dy) % 3;

        if ((lvl == nlvl) or
            (lvl == 0 and nlvl == 1 and e == 2) or
            (lvl == 1 and nlvl == 0 and e == 2) or
            (lvl == 1 and nlvl == 2 and e == 0) or
            (lvl == 2 and nlvl == 1 and e == 0) or
            (lvl == 2 and nlvl == 0 and e == 1) or
            (lvl == 0 and nlvl == 2 and e == 1)) {
          queue.push({c+1, heu(x+dx,y+dy,e), x+dx, y+dy, e});
          //history[{c+1, heu(x+dx,y+dy,e), x+dx, y+dy, e}] = {c, h, x, y, e};
        }
      }
    }
  }


  return EXIT_SUCCESS;
}

