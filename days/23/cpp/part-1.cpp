#include <array>
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

  std::vector<std::array<int,4>> nanobots;
  std::size_t strongest_bot_idx = 0;

  while (true) {
    int a, b, c, d;
    infile.ignore(5); infile >> a;
    if (infile.eof())
      break;
    infile.ignore(1); infile >> b;
    infile.ignore(1); infile >> c;
    infile.ignore(5); infile >> d;
    infile.ignore(10, '\n');
    nanobots.push_back({a, b, c, d});
    if (d > nanobots[strongest_bot_idx][3])
      strongest_bot_idx = nanobots.size()-1;
  }

  auto in_range = 0;
  for (auto [x,y,z,_] : nanobots)
    if (std::abs(x - nanobots[strongest_bot_idx][0]) +
        std::abs(y - nanobots[strongest_bot_idx][1]) +
        std::abs(z - nanobots[strongest_bot_idx][2]) <=
        nanobots[strongest_bot_idx][3])
      ++in_range;

  std::cout << in_range << std::endl;

  return EXIT_SUCCESS;
}

