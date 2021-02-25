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

  int num; infile >> num; infile.close();

  std::list<int> scores{3, 7};
  decltype(scores.begin()) elves[2];
  elves[0] = scores.begin();
  elves[1] = scores.begin();
  elves[1]++;

  auto resultstart = scores.cbegin();
  for (auto round = 0; round < num; round++) {
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
      for (auto i : tmp)
        scores.push_back(i);
    }

    for (auto& e : elves)
      for (auto i = *e; i >= 0; i--)
        if (++e == scores.end())
          e = scores.begin();

    //for (auto i = scores.cbegin(); i != scores.cend(); ++i)
    //  if (i == elves[0])
    //    std::cout << "(" << *i << ")";
    //  else if (i == elves[1])
    //    std::cout << "[" << *i << "]";
    //  else 
    //    std::cout << " " << *i << " ";
    //std::cout << std::endl;

    resultstart++;
  }

  for (auto i = 0; i < 10; ++i)
    std::cout << *(resultstart++);
  std::cout << std::endl;

  return EXIT_SUCCESS;
}

