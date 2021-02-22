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

  auto max3x3 = -99999;
  auto maxx = 0;
  auto maxy = 0;
  for (auto y = 1; y < 299; ++y)
    for (auto x = 1; x < 299; ++x)
      if (auto score = grid[y-1][x-1] +
                       grid[y-1][x  ] +
                       grid[y-1][x+1] +
                       grid[y  ][x-1] +
                       grid[y  ][x  ] +
                       grid[y  ][x+1] +
                       grid[y+1][x-1] +
                       grid[y+1][x  ] +
                       grid[y+1][x+1]; score > max3x3)
        std::tie(max3x3, maxx, maxy) = std::tuple(score, x, y);

  std::cout << maxx << "," << maxy << std::endl;

  return EXIT_SUCCESS;
}

