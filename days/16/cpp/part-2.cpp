#include <cstdlib>
#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

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

  std::vector<std::pair<int, std::vector<int>>> matching_ops;

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

    std::vector<int> matches;
    for (auto opidx = 0u; opidx < OPs.size(); opidx++)
      if (OPs[opidx](op, in) == out)
        matches.push_back(opidx);
    matching_ops.emplace_back(op[0], std::move(matches));
  }

  std::array<int, 16> op_map;
  while (matching_ops.size()) {
    int resolved;
    for (auto& m : matching_ops) {
      if (m.second.size() == 1) {
        resolved = m.second[0];
        op_map[m.first] = resolved;
        //std::cout << m.first << " -> " << resolved << std::endl;
        break;
      }
    }

    for (auto& m : matching_ops)
      if (auto pos = std::find(m.second.cbegin(), 
                               m.second.cend(), 
                               resolved); 
          pos != m.second.cend())
        m.second.erase(pos);

    decltype(matching_ops) n;
    std::copy_if(matching_ops.cbegin(), matching_ops.cend(),
                 std::back_inserter(n),
                 [](auto& m) { return m.second.size() > 0; });
    matching_ops = n;
  }

  I4 data{0, 0, 0, 0};
  {
    std::string line;
    std::getline(infile, line);
  }
  while (true) {
    I4 op;
    infile >> op[0] >> op[1] >> op[2] >> op[3];
    if (infile.eof())
      break;

    data = OPs[op_map[op[0]]](op, data);
  }
  std::cout << data[0] << std::endl;

  return EXIT_SUCCESS;
}

