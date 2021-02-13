#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
  if (argc < 2) {
    return EXIT_FAILURE;
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read file");

  int twos = 0;
  int threes = 0;

  while (true) {
    int count[256] = {0};

    std::string ID;
    infile >> ID;
    if (infile.eof())
      break;

    for (auto letter : ID)
      count[(std::size_t)letter]++;

    for (auto i = 0u; i < 256; ++i)
      if (count[i] == 2) {
        twos++;
        break;
      }
    for (auto i = 0u; i < 256; ++i)
      if (count[i] == 3) {
        threes++;
        break;
      }
  }

  std::cout << twos * threes << std::endl;

  return EXIT_SUCCESS;
}

