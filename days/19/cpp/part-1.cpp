#include <cstdlib>
#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>

typedef std::array<int, 4> I4;
typedef std::array<int, 6> I6;


int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::array<std::function<I6(const I4&, I6)>,16> OPs{
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

  I6 regs{0, 0, 0, 0, 0, 0};
  std::string dummy;
  int iptr_target;
  infile >> dummy >> iptr_target;
  int& iptr = regs[iptr_target];

  std::vector<I4> instructions;

  std::map<std::string, std::size_t> opmap;
  {
    int i = 0;
    opmap["addr"] = i++;
    opmap["addi"] = i++;
    opmap["mulr"] = i++;
    opmap["muli"] = i++;
    opmap["banr"] = i++;
    opmap["bani"] = i++;
    opmap["borr"] = i++;
    opmap["bori"] = i++;
    opmap["setr"] = i++;
    opmap["seti"] = i++;
    opmap["gtir"] = i++;
    opmap["gtri"] = i++;
    opmap["gtrr"] = i++;
    opmap["eqir"] = i++;
    opmap["eqri"] = i++;
    opmap["eqrr"] = i++;
  }

  while (true) {
    std::string op;
    infile >> op;
    if (infile.eof())
      break;
    I4 opargs;
    opargs[0] = opmap[op];
    for (auto i = 1; i < 4; i++)
      infile >> opargs[i];
    instructions.push_back(std::move(opargs));
  }

  while (iptr >= 0 and iptr < (int)instructions.size()) {
    regs = OPs[instructions[iptr][0]](instructions[iptr], regs);
    iptr++;
  }
  std::cout << regs[0] << std::endl;

  return EXIT_SUCCESS;
}

