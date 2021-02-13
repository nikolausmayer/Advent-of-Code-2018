#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <stdexcept>
#include <vector>

int main(int argc, char* argv[]){
  if (argc < 2)
    return EXIT_FAILURE;

  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read file");

  std::vector<int> deltas;
  while (true) {
    auto delta = 0;
    infile >> delta;
    if (infile.eof())
      break;
    deltas.push_back(delta);
  }

  std::unordered_set<int> log;
  auto frequency = 0;
  auto i = 0u;
  while (true) {
    frequency += deltas[i++];
    if (i == deltas.size())
      i = 0;
    if (log.contains(frequency)) {
      std::cout << frequency << std::endl;
      break;
    }
    log.insert(frequency);
  }

  return EXIT_SUCCESS;
}

