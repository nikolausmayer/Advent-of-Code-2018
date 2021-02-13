#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char* argv[])
{
  if (argc < 2) {
    return EXIT_FAILURE;
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read file");

  std::vector<std::string> IDs;
  while (true) {
    std::string ID;
    infile >> ID;
    if (infile.eof())
      break;
    IDs.emplace_back(std::move(ID));
  }

  for (auto i = 0u; i < IDs.size()-1; ++i) {
    for (auto j = i+1; j < IDs.size(); ++j) {
      auto diff = 0;
      for (auto c = 0u; c < IDs[i].size(); ++c)
        diff += (IDs[i][c] != IDs[j][c]);
      if (diff == 1) {
        for (auto c = 0u; c < IDs[i].size(); ++c)
          if (IDs[i][c] == IDs[j][c])
            std::cout << IDs[i][c];
        std::cout << std::endl;
        return EXIT_SUCCESS;
      }
    }
  }

  return EXIT_SUCCESS;
}

