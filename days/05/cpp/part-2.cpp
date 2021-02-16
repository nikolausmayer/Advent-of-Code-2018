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

  auto minlen = line.size();
  for (auto polymer = 0; polymer < 26; ++polymer) {
    auto len = line.size();
    std::vector<bool> valid_map(len);
    std::fill(valid_map.begin(), valid_map.end(), true);

    for (auto i = 0u; i < line.size(); ++i)
      if (line[i] == polymer + 'A' or line[i] == polymer + 'a') {
          valid_map[i] = false;
          --len;
      }

    auto p0 = 0, p1 = 1;
    while (true) {
      if (std::abs(line[p0] - line[p1]) == 32) {
        valid_map[p0] = false;
        valid_map[p1] = false;
        len -= 2;
        --p0;
        ++p1;
      } else {
        p0 = p1;
        p1 = p1 + 1;
      }

      while (p1 < (int)line.size() and not valid_map[p1])
        ++p1;
      if (p1 >= (int)line.size())
        break;

      while (p0 >= 0 and not valid_map[p0])
        --p0;
      if (p0 < 0) {
        p0 = p1;
        p1 = p1 + 1;

        while (p1 < (int)line.size() and not valid_map[p1])
          ++p1;
        if (p1 == (int)line.size())
          break;
      }
    }

    minlen = std::min(minlen, len);
  }

  std::cout << minlen << std::endl;

  return EXIT_SUCCESS;
}

