#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <utility>


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
  auto eroLevel = [&](auto x, auto y) {
    if (x == tx and y == ty) {
      geoIndex[{x,y}] = 0;
    } else if (x == 0) {
      geoIndex[{x,y}] = y * 48271;
    } else if (y == 0) {
      geoIndex[{x,y}] = x * 16807;
    } else {
      auto a = (geoIndex[{x-1,y}] + depth) % 20183;
      auto b = (geoIndex[{x,y-1}] + depth) % 20183;
      geoIndex[{x,y}] = a * b;
    }
    return (geoIndex[{x,y}] + depth) % 20183;
  };

  auto riskLevelSum = 0;
  for (auto y = 0; y <= ty; y++) {
    for (auto x = 0; x <= tx; x++) {
      riskLevelSum += eroLevel(x, y) % 3;
      //auto lvl = eroLevel(x, y);
      //switch (lvl % 3) {
      //  case 0: std::cout << '.'; break;
      //  case 1: std::cout << '='; break;
      //  case 2: std::cout << '|'; break;
      //  default: std::cout << " " << lvl << " ";
      //}
    }
    //std::cout << std::endl;
  }
  std::cout << riskLevelSum << std::endl;

  return EXIT_SUCCESS;
}

