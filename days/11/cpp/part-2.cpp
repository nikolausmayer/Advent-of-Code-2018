#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <tuple>


int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");
  
  int serial;
  infile >> serial;

  int grid[300][300];
  for (auto y = 0; y < 300; ++y)
    for (auto x = 0; x < 300; ++x)
      grid[y][x] = (((x+1+10) * (y+1) + serial) * (x+1+10) % 1000) / 100 - 5;

  int integral_grid[300][300];
  for (auto y = 0; y < 300; ++y)
    for (auto x = 0; x < 300; ++x)
      integral_grid[y][x] = grid[y][x] +
        (y > 0 ? integral_grid[y-1][x] : 0) +
        (x > 0 ? integral_grid[y][x-1] : 0) -
        (x > 0 and y > 0 ? integral_grid[y-1][x-1] : 0);

  auto max = -99999;
  auto mx = 0;
  auto my = 0;
  auto ms = 0;
  for (auto y = 0; y < 300; ++y)
    for (auto x = 0; x < 300; ++x)
      for (auto s = 0; s < std::min(x,y); ++s)
        if (auto score = integral_grid[y  ][x  ] -
                         integral_grid[y-s][x  ] -
                         integral_grid[y  ][x-s] +
                         integral_grid[y-s][x-s]; score > max)
          std::tie(max, mx, my, ms) = std::tuple(score, x-s+2, y-s+2, s);

  std::cout << mx << "," << my << "," << ms << std::endl;

  return EXIT_SUCCESS;
}

