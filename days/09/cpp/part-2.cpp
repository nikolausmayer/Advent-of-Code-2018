#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>

int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  int players, marbles;
  std::string d;
  infile >> players >> d >> d >> d >> d >> d >> marbles;
  marbles *= 100;

  std::vector<unsigned long> scores(players);
  std::list<int> circle;
  circle.push_back(0);
  auto pos = circle.begin();
  int player = -1;
  
  for (auto next = 1; next <= marbles; ++next) {
    player = (player + 1) % players;

    if (next % 23 == 0) {
      scores[player] += next;
      for (auto i = 0; i < 6; ++i) {
        if (pos == circle.begin())
          pos = circle.end();
        --pos;
      }
      if (pos == circle.begin())
        pos = circle.end();
      auto tmp = pos;
      --tmp;
      scores[player] += *tmp;
      circle.erase(tmp);
    } else {
      ++pos; 
      if (pos == circle.end()) pos = circle.begin();
      ++pos; 
      circle.insert(pos, next);
      if (pos == circle.begin())
        pos = circle.end();
      --pos;
    }
  }

  std::cout << *std::max_element(scores.cbegin(), scores.cend()) << std::endl;
  
  return EXIT_SUCCESS;
}

