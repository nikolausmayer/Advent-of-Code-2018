#include <cstdlib>
#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>

typedef std::array<int, 4> I4;

std::ostream& operator<<(std::ostream& oss, const I4& a) {
  oss << "[" << a[0] << " " 
             << a[1] << " " 
             << a[2] << " " 
             << a[3] << "]";
  return oss;
}


int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::array<std::function<I4(const I4&, I4)>,16> OPs{
    /*ADDR*/ [](const auto& op, auto regs) {
      regs[op[3]] = regs[op[1]] + regs[op[2]]; return regs; },
    /*ADDI*/ [](const auto& op, auto regs) {
      regs[op[3]] = regs[op[1]] + op[2]; return regs; },

    /*MULR*/ [](const auto& op, auto regs) {
      regs[op[3]] = regs[op[1]] * regs[op[2]]; return regs; },
    /*MULI*/ [](const auto& op, auto regs) {
      regs[op[3]] = regs[op[1]] * op[2]; return regs; },

    /*BANR*/ [](const auto& op, auto regs) {
      regs[op[3]] = regs[op[1]] & regs[op[2]]; return regs; },
    /*BANI*/ [](const auto& op, auto regs) {
      regs[op[3]] = regs[op[1]] & op[2]; return regs; },

    /*BORR*/ [](const auto& op, auto regs) {
      regs[op[3]] = regs[op[1]] | regs[op[2]]; return regs; },
    /*BORI*/ [](const auto& op, auto regs) {
      regs[op[3]] = regs[op[1]] | op[2]; return regs; },

    /*SETR*/ [](const auto& op, auto regs) {
      regs[op[3]] = regs[op[1]]; return regs; },
    /*SETI*/ [](const auto& op, auto regs) {
      regs[op[3]] = op[1]; return regs; },

    /*GTIR*/ [](const auto& op, auto regs) {
      regs[op[3]] = (op[1] > regs[op[2]]); return regs; },
    /*GTRI*/ [](const auto& op, auto regs) {
      regs[op[3]] = (regs[op[1]] > op[2]); return regs; },
    /*GTRR*/ [](const auto& op, auto regs) {
      regs[op[3]] = (regs[op[1]] > regs[op[2]]); return regs; },

    /*EQIR*/ [](const auto& op, auto regs) {
      regs[op[3]] = (op[1] == regs[op[2]]); return regs; },
    /*EQRI*/ [](const auto& op, auto regs) {
      regs[op[3]] = (regs[op[1]] == op[2]); return regs; },
    /*EQRR*/ [](const auto& op, auto regs) {
      regs[op[3]] = (regs[op[1]] == regs[op[2]]); return regs; }
  };

  auto results = 0;
  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof() or line == "")
      break;

    I4 in;
    for (auto i = 0; i < 4; i++)
      in[i] = line[3*i+9] - '0';

    I4 op;
    infile >> op[0] >> op[1] >> op[2] >> op[3];
    std::getline(infile, line);

    std::getline(infile, line);
    I4 out;
    for (auto i = 0; i < 4; i++)
      out[i] = line[3*i+9] - '0';

    std::getline(infile, line);

    //std::cout << in  << std::endl
    //          << op  << std::endl
    //          << out << std::endl;

    auto matches =  std::count_if(OPs.cbegin(), 
                                  OPs.cend(), 
                                  [&in, &op, &out](auto& f) {
                                    return f(op, in) == out;
                                  });
    if (matches >= 3)
      results++;
  }

  std::cout << results << std::endl;

  return EXIT_SUCCESS;
}

