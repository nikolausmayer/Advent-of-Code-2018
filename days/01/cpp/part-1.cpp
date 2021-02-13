#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]){
  if (argc < 2)
    return EXIT_FAILURE;

  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read file");

  auto frequency = 0;
  while (true) {
    auto delta = 0;
    infile >> delta;
    if (infile.eof())
      break;
    frequency += delta;
  }

  std::cout << frequency << std::endl;

  return EXIT_SUCCESS;
}

