#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

struct Cart
{
  int ID, x, y;
  char heading;
  int state;
};

int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::vector<Cart> carts;
  std::vector<std::vector<char>> map;
  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;

    std::vector<char> mapped;
    for (auto c : line) {
      switch (c) {
        case '^':
        case 'v':
          carts.emplace_back(carts.size(), mapped.size(), map.size(), c, 0);
          mapped.push_back('|');
          break;
        case '>':
        case '<':
          carts.emplace_back(carts.size(), mapped.size(), map.size(), c, 0);
          mapped.push_back('-');
          break;
        default: 
          mapped.push_back(c);
      }
    }
    map.emplace_back(std::move(mapped));
  }

  while (true) {
    std::sort(carts.begin(), carts.end(), 
              [](auto c1, auto c2) {
                if (c1.y < c2.y)
                  return true;
                else if (c1.y == c2.y)
                  return c1.x < c2.x;
                else
                  return false;
              });

    for (auto& c : carts) {
      const auto m = map[c.y][c.x];
      switch (m) {
        case '+':
          switch (c.state) {
            case 0:
              switch (c.heading) {
                case '^': c.heading = '<'; c.x--; break;
                case '>': c.heading = '^'; c.y--; break;
                case 'v': c.heading = '>'; c.x++; break;
                case '<': c.heading = 'v'; c.y++; break;
              }
              break;
            case 1:
              switch (c.heading) {
                case '^': c.y--; break;
                case '>': c.x++; break;
                case 'v': c.y++; break;
                case '<': c.x--; break;
              }
              break;
            case 2:
              switch (c.heading) {
                case '^': c.heading = '>'; c.x++; break;
                case '>': c.heading = 'v'; c.y++; break;
                case 'v': c.heading = '<'; c.x--; break;
                case '<': c.heading = '^'; c.y--; break;
              }
              break;
          }
          c.state = (c.state + 1) % 3;
          break;
        case '-':
          switch (c.heading) {
            case '>': c.x++; break;
            case '<': c.x--; break;
          }
          break;
        case '|':
          switch (c.heading) {
            case 'v': c.y++; break;
            case '^': c.y--; break;
          }
          break;
        case '\\':
          switch (c.heading) {
            case '^': c.x--; c.heading = '<'; break;
            case '>': c.y++; c.heading = 'v'; break;
            case 'v': c.x++; c.heading = '>'; break;
            case '<': c.y--; c.heading = '^'; break;
          }
          break;
        case '/':
          switch (c.heading) {
            case '^': c.x++; c.heading = '>'; break;
            case '>': c.y--; c.heading = '^'; break;
            case 'v': c.x--; c.heading = '<'; break;
            case '<': c.y++; c.heading = 'v'; break;
          }
          break;
      }

      for (const auto& c2 : carts)
        if (c.ID != c2.ID and c.x == c2.x and c.y == c2.y) {
          std::cout << c.x << "," << c.y << std::endl;
          return EXIT_SUCCESS;
        }
    }

    //for (const auto& c : carts)
    //  std::cout << "Car " << c.ID << " "  << c.heading << " at (" << c.x << ", " << c.y << ")" << std::endl;
    //std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}

