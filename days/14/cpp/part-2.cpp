#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <stdexcept>


int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::list<int> target;
  {
    int num; infile >> num;
    while (num >= 10) {
      target.push_front(num % 10);
      num /= 10;
    }
    target.push_front(num);
  }
  infile.close();

  std::list<int> scores{3, 7};
  decltype(scores.begin()) elves[2];
  elves[0] = scores.begin();
  elves[1] = scores.begin();
  elves[1]++;

  while (true) {
    auto sum = 0;
    for (const auto& e : elves)
      sum += *e;

    {
      std::list<int> tmp;
      while (sum >= 10) {
        tmp.push_front(sum % 10);
        sum /= 10;
      }
      tmp.push_front(sum);

      for (auto i : tmp) {
        scores.push_back(i);

        if (scores.size() >= target.size()) [[likely]] {
          auto here = scores.cend();
          here--;
          auto there = target.cend();
          there--;

          bool match = true;
          for (auto j = 0u; j < target.size(); j++) {
            if (*here != *there) {
              match = false;
              break;
            }
            here--;
            there--;
          }
          if (match) {
            std::cout << scores.size()-target.size() << std::endl;
            return EXIT_SUCCESS;
          }
        }
      }
    }

    for (auto& e : elves)
      for (auto i = *e; i >= 0; i--)
        if (++e == scores.end())
          e = scores.begin();
  }

  return EXIT_SUCCESS;
}

