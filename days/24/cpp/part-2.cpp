#include <cstdlib>
#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <stdexcept>
#include <vector>


struct Group
{
  bool affiliation;
  int count;
  int hit_points;
  std::uint8_t weaknesses;
  std::uint8_t immunities;
  int attack_damage;
  std::uint8_t attack_type;
  int initiative;

  Group* target;
  Group* targeted_by;

  friend auto operator<=>(const Group& a, const Group& b) = default;
};

std::ostream& operator<<(std::ostream& os, const Group& g)
{
  os << (g.affiliation ? 'O' : 'X') << ": "
     << g.count << "x" << g.hit_points << " "
     << "w:" << std::bitset<8>{g.weaknesses} << " "
     << "i:" << std::bitset<8>{g.immunities} << " "
     << g.attack_damage << "x" << std::bitset<8>{g.attack_type} << " "
     << g.initiative;
  return os;
}


int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  const auto re1 = std::regex("([0-9]+) units each with ([0-9]+) hit points (?:\\(([^;]*)(?:;? ?(.*))\\) )?with an attack that does ([0-9]+) ([a-z]+) damage at initiative ([0-9]+)");
  const auto re2 = std::regex("(weak|immune) to (.*)");
  const auto re3 = std::regex("[a-z]+");

  std::vector<Group> groups;
  std::map<std::string, int> type_map;

  std::string line;
  std::getline(infile, line);

  bool affiliation = true;
  while (true) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    if (line == "") {
      affiliation = false;
      std::getline(infile, line);
      continue;
    }

    Group g{};
    g.affiliation = affiliation;

    std::smatch match1;
    std::regex_search(line, match1, re1);
    const auto count = std::stoi(match1[1]);
    g.count = count;
    const auto hp = std::stoi(match1[2]);
    g.hit_points = hp;
    if (match1[3].str().size()) {
      std::smatch match2;
      const auto substr = match1[3].str();
      std::regex_match(substr, match2, re2);
      const auto mode = match2[1].str();
      const auto types = match2[2].str();

      std::sregex_iterator next{types.begin(), types.end(), re3};
      std::sregex_iterator end;
      while (next != end) {
        std::smatch match3 = *next;
        const auto type = match3.str();
        next++;

        if (not type_map.contains(type))
          type_map[type] = type_map.size();
        (mode == "weak" ? g.weaknesses : g.immunities) |= (1 << type_map[type]);
      }
    }
    if (match1[4].str().size()) {
      std::smatch match2;
      const auto substr = match1[4].str();
      std::regex_match(substr, match2, re2);
      const auto mode = match2[1].str();
      const auto types = match2[2].str();

      std::sregex_iterator next{types.begin(), types.end(), re3};
      std::sregex_iterator end;
      while (next != end) {
        std::smatch match3 = *next;
        const auto type = match3.str();
        next++;

        if (not type_map.contains(type))
          type_map[type] = type_map.size();
        (mode == "weak" ? g.weaknesses : g.immunities) |= (1 << type_map[type]);
      }
    }
    const auto attack_damage = std::stoi(match1[5]);
    g.attack_damage = attack_damage;
    const auto attack_type = match1[6].str();
    if (not type_map.contains(attack_type))
      type_map[attack_type] = type_map.size();
    g.attack_type = (1 << type_map[attack_type]);
    const auto initiative = std::stoi(match1[7]);
    g.initiative = initiative;

    groups.push_back(std::move(g));
  }

  auto reserve = groups;

  for (auto boost = 0;; boost++) {
    groups = reserve;
    for (auto& g : groups)
      if (g.affiliation)
        g.attack_damage += boost;

    std::vector<std::size_t> order(groups.size());
    std::iota(order.begin(), order.end(), 0);
    
    bool its_a_tie = false;
    auto prev_state = groups;
    while (true) {
      /// Bring groups into target-selection order
      std::sort(order.begin(), order.end(), 
        [&](const auto& a, const auto& b) {
          const auto ap = groups[a].count * groups[a].attack_damage;
          const auto bp = groups[b].count * groups[b].attack_damage;
          return ap > bp or
                (ap == bp and 
                 groups[a].initiative > groups[b].initiative);
        });

      for (auto& g : groups) {
        g.target      = nullptr;
        g.targeted_by = nullptr;
      }

      /// Select targets
      for (auto idx : order) {
        auto& g = groups[idx];
        auto maxdamage = 0;
        Group* target = nullptr;
        for (auto& other : groups) {
          if (g.affiliation == other.affiliation or
              other.count == 0 or
              other.targeted_by != nullptr)
            continue;
          
          auto damage = g.count * g.attack_damage;
          if (g.attack_type & other.weaknesses)
            damage *= 2;
          else if (g.attack_type & other.immunities)
            damage = 0;
          if (damage > maxdamage or
              (damage == maxdamage and
               target != nullptr and
               (other.count * other.attack_damage > target->count * target->attack_damage or
                (other.count * other.attack_damage == target->count * target->attack_damage and
                 other.initiative > target->initiative)))) {
            maxdamage = damage;
            target = &other;
          }
        }
        if (target != nullptr) {
          g.target = target;
          target->targeted_by = &g;
        }
      }

      /// Bring groups into attacking order
      std::sort(order.begin(), order.end(), 
        [&](const auto& a, const auto& b) {
          return groups[a].initiative > groups[b].initiative;
        });

      /// Attack
      for (auto idx : order) {
        auto& g = groups[idx];
        if (not g.target or 
            not g.count or 
            not g.target->count)
          continue;

        auto damage = g.count * g.attack_damage;
        if (g.attack_type & g.target->weaknesses)
          damage *= 2;
        else if (g.attack_type & g.target->immunities)
          damage = 0;

        auto dead = std::min(g.target->count, 
                             damage / g.target->hit_points);
        g.target->count -= dead;
      }

      if (std::all_of(groups.cbegin(), groups.cend(), 
          [](auto &g) { return g.count == 0 or g.affiliation; }) or
          std::all_of(groups.cbegin(), groups.cend(), 
          [](auto &g) { return g.count == 0 or !g.affiliation; }))
        break;

      /// Oh for fuck's sake, seriously?! You wonderful bastard.
      if (prev_state == groups) {
        its_a_tie = true;
        break;
      }
      prev_state = groups;
    }
    
    if (its_a_tie)
      continue;

    auto remaining_units = 0;
    for (const auto& g : groups)
      if (g.affiliation)
        remaining_units += g.count;
    if (remaining_units > 0) {
      std::cout << remaining_units << std::endl;
      break;
    }
  }

  return EXIT_SUCCESS;
}

