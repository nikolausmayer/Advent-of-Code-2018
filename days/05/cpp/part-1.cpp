#include <algorithm>
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

  std::string line;
  std::getline(infile, line);

  auto len = line.size();
  std::vector<bool> valid_map(len);
  std::fill(valid_map.begin(), valid_map.end(), true);
  auto p0 = 0, p1 = 1;
  while (p1 < (int)line.size()) {
    while (p1 < (int)line.size() and std::abs(line[p0] - line[p1]) == 32) {
      valid_map[p0] = false;
      valid_map[p1] = false;
      len -= 2;

      while (p0 >= 0 and not valid_map[p0]) 
        --p0;
      if (p0 < 0)
        p0 = p1;

      ++p1;
    } 

    p0 = p1;
    ++p1;
  }

  std::cout << len << std::endl;

  return EXIT_SUCCESS;
}

