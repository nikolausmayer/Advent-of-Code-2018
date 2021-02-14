#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>

int main(int argc, char* argv[])
{
  if (argc < 2) {
    return EXIT_FAILURE;
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read file");

  const auto re = std::regex("#([0-9]+) @ ([0-9]+),([0-9]+): ([0-9]+)x([0-9]+)");

  std::uint8_t map[1'000'000] = {0};

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;

    std::smatch match;
    if (not std::regex_match(line, match, re))
      continue;

    const auto left = std::stoi(match[2]);
    const auto top  = std::stoi(match[3]);
    const auto w    = std::stoi(match[4]);
    const auto h    = std::stoi(match[5]);

    for (auto y = top; y < top+h; ++y)
      for (auto x = left; x < left+w; ++x)
        map[1000*y+x]++;
  }

  infile.clear();
  infile.seekg(0);

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;

    std::smatch match;
    if (not std::regex_match(line, match, re))
      continue;

    const auto ID   = std::stoi(match[1]);
    const auto left = std::stoi(match[2]);
    const auto top  = std::stoi(match[3]);
    const auto w    = std::stoi(match[4]);
    const auto h    = std::stoi(match[5]);

    bool ok = true;
    for (auto y = top; y < top+h and ok; ++y)
      for (auto x = left; x < left+w and ok; ++x)
        if (map[1000*y+x] > 1)
          ok = false;
    if (ok) {
      std::cout << ID << std::endl;
      break;
    }
  }

  return EXIT_SUCCESS;
}

