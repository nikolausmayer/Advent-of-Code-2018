#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <stdexcept>
#include <utility>
#include <type_traits>
#include <vector>


struct Unit 
{
  int ID, x, y, attack, health;
  bool is_goblin;
};

constexpr bool operator==(const Unit& a, const Unit& b)
{
  return a.ID == b.ID;
}
constexpr bool unitCompare(const Unit& a, const Unit& b)
{
  return (a.y < b.y) or (a.y == b.y and a.x < b.x);
}

constexpr bool reading_order(const std::pair<int,int>& p1,
                             const std::pair<int,int>& p2)
{
  return (p1.second < p2.second) or 
         (p1.second == p2.second and p1.first < p2.first);
}


struct AstarNode
{
  int cost, heuristic, x, y;
};
constexpr int AstarHeuristic(int x, int y, const auto& target)
{
  return std::abs(x - target.first) + std::abs(y - target.second);
}
int ASTAR(const auto& map, const auto& start, const auto& target)
{
  std::set<std::pair<int,int>> visited;
  auto cmp = [](const AstarNode& a, const AstarNode& b) {
    return (a.cost + a.heuristic) > (b.cost + b.heuristic);
  };
  std::priority_queue<AstarNode, 
                      std::vector<AstarNode>,
                      decltype(cmp)> queue(cmp);
  queue.push({0, AstarHeuristic(start.first, start.second, target), 
             start.first, start.second});
  while (queue.size()) {
    auto node = queue.top();
    queue.pop();
    if (node.heuristic == 0)
      return node.cost;
    if (visited.contains({node.x, node.y})) {
      continue;
    }
    visited.insert({node.x, node.y});
    if (map[node.y-1][node.x] == '.') {
      queue.push({node.cost+1, 
                  AstarHeuristic(node.x, node.y-1, target),
                  node.x, node.y-1});
    }
    if (map[node.y+1][node.x] == '.') {
      queue.push({node.cost+1, 
                  AstarHeuristic(node.x, node.y+1, target),
                  node.x, node.y+1});
    }
    if (map[node.y][node.x-1] == '.') {
      queue.push({node.cost+1, 
                  AstarHeuristic(node.x-1, node.y, target),
                  node.x-1, node.y});
    }
    if (map[node.y][node.x+1] == '.') {
      queue.push({node.cost+1, 
                  AstarHeuristic(node.x+1, node.y, target),
                  node.x+1, node.y});
    }
  }
  return -1;
}


int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::vector<Unit> units;

  std::vector<std::vector<char>> map;
  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    std::vector<char> row;
    for (auto c : line) {
      switch (c) {
        case '#': 
          break;
        case '.': 
          break;
        case 'G': 
          units.emplace_back(units.size(),
                             row.size(), map.size(), 
                             3, 200, true);
          break;
        case 'E': 
          units.emplace_back(units.size(),
                             row.size(), map.size(), 
                             3, 200, false);
          break;
      }
      row.push_back(c); 
    }
    map.emplace_back(std::move(row));
  }

  auto printMap = [&map]{
    for (const auto& row : map) {
      for (auto c : row)
        std::cout << c;
      std::cout << std::endl;
    }
    std::cout << std::endl;
  };

  bool game_is_running = true;
  auto round = 0;
  while (game_is_running) {
    std::cout << "---ROUND " << round << " STARTS---\n";
    printMap();
    /// Bring all units into reading order
    std::sort(units.begin(), units.end(), unitCompare);

    for (auto& active_unit : units) {

      std::cout << active_unit.x << "," << active_unit.y << " (" << (active_unit.is_goblin ? "G" : "E") << ")\n";

      if (active_unit.health <= 0) {
        std::cout << "  omae wa, mou shindeiru.\n";
        continue;
      }

      /// Check terminating condition
      if (not std::any_of(units.cbegin(), units.cend(), 
                          [&active_unit](const Unit& u) {
                            return u.is_goblin != active_unit.is_goblin;
                          })) {
        std::cout << "All foes vanquished before round " << round << " ended\n";
        auto battle_outcome = 0;
        for (const auto& u : units)
          if (u.health > 0)
            battle_outcome += u.health;
        battle_outcome *= round-1;
        std::cout << "BATTLE OUTCOME: " << battle_outcome << std::endl;
        game_is_running = false;
        break;
      }

      /// Check whether a target is in range...
      auto is_in_range = [&active_unit](const Unit& other) {
        return (other != active_unit) and 
               (other.is_goblin != active_unit.is_goblin) and
               (other.health > 0) and
               (std::abs(active_unit.x - other.x) +
                std::abs(active_unit.y - other.y) == 1);
      };
      bool targets_in_range = std::any_of(units.cbegin(),
                                          units.cend(),
                                          is_in_range);
      /// ...else move towards one
      if (not targets_in_range)
      {
        std::cout << "  no targets in range, planning a move...\n";

        std::set<std::pair<int,int>> targets;
        /// Gather target tiles
        for (const auto& u : units) {
          /// This also avoids u == active_unit
          if (u.is_goblin == active_unit.is_goblin or
              u.health <= 0)
            continue;
          std::cout << "  foe at " << u.x << "," << u.y << std::endl;
          if (map[u.y-1][u.x] == '.')
            targets.insert({u.x, u.y-1});
          if (map[u.y+1][u.x] == '.')
            targets.insert({u.x, u.y+1});
          if (map[u.y][u.x-1] == '.')
            targets.insert({u.x-1, u.y});
          if (map[u.y][u.x+1] == '.')
            targets.insert({u.x+1, u.y});
        }

        if (targets.size() == 0) {
          std::cout << "  alas, all foes are beyond approach.\n";
        } else {
          /// Calculate distances for target tiles
          int mindist = 99999;
          std::vector<std::pair<int,int>> closest_targets;
          for (const auto& target : targets) {
            int distance = ASTAR(map, 
                                 std::make_pair(active_unit.x, 
                                                active_unit.y),
                                 target);
            std::cout << "  tile " << target.first << "," << target.second << " has distance " << distance << std::endl;
            if (distance == -1)
              continue;
            if (distance <= mindist) {
              if (distance < mindist)
                closest_targets.clear();
              closest_targets.push_back(target);
              mindist = distance;
            }
          }
          if (mindist == 99999) {
            std::cout << "  curses, the way is shut.\n";
          } else {
            /// Sort closest target tiles in reading order
            std::sort(closest_targets.begin(), closest_targets.end(), reading_order);
            const auto& target_tile = closest_targets.front();
            /// Get shortest reading-order-first path to selected target tile
            std::pair<int,int> step{active_unit.x, active_unit.y};
            mindist = 99999;
            for (auto [dx,dy] : std::vector<std::pair<int,int>>{{0,-1},{-1,0},{1,0},{0,1}}) {
              if (map[active_unit.y+dy][active_unit.x+dx] == '.') {
                auto distance = ASTAR(map, 
                                      std::make_pair(active_unit.x+dx, 
                                                     active_unit.y+dy), 
                                      target_tile);
                //std::cout << "    " << dx << ":" << dy << " " << distance << std::endl;
                if (distance != -1 and distance < mindist) {
                  mindist = distance;
                  step = {active_unit.x+dx, active_unit.y+dy};
                }
              }
            }
            std::cout << "  unit will move to " << step.first << "," << step.second << ", towards " << target_tile.first << "," << target_tile.second << std::endl;
            /// Move unit and update map
            map[active_unit.y][active_unit.x] = '.';
            map[step.second][step.first] = (active_unit.is_goblin ? 'G' : 'E');
            active_unit.x = step.first;
            active_unit.y = step.second;
          }
        }
      }
      printMap();

      /// Combat if target is in range
      targets_in_range = std::any_of(units.cbegin(),
                                     units.cend(),
                                     is_in_range);
      if (targets_in_range) {
        std::cout << "  COMBAT\n";
        /// Choose weakest target, in reading order
        std::pair<int,int> minpos;
        Unit* targeted_foe = nullptr;
        auto minhealth = 99999;
        for (auto [dx,dy] : std::vector<std::pair<int,int>>{{0,-1},{-1,0},{1,0},{0,1}}) {
          for (auto& u : units) {
            if (u.is_goblin == active_unit.is_goblin or
                u.health <= 0 or
                u.x != active_unit.x+dx or
                u.y != active_unit.y+dy)
              continue;
            auto health = u.health;
            /// First condition will always fire; uninitialized minpos is ok
            if (health < minhealth or
                (health == minhealth and
                 (u.y < minpos.second or
                  (u.y == minpos.second and u.x < minpos.first)))) {
              minhealth = health;
              minpos = {u.x, u.y};
              targeted_foe = &u;
            }
          }
        }
        std::cout << "  combat with " << minpos.first << "," << minpos.second << std::endl;
        /// Combat
        std::cout << "    health " << targeted_foe->health << "->" << targeted_foe->health-active_unit.attack << std::endl;
        targeted_foe->health -= active_unit.attack;
        /// Aftermath: delete dead target's map tile
        if (targeted_foe->health <= 0) {
          std::cout << "  the foe has been vanquished\n";
          map[targeted_foe->y][targeted_foe->x] = '.';
        }
      }
    }
    /// Cleanup: remove dead units
    decltype(units) alive_units;
    std::copy_if(units.cbegin(), units.cend(), std::back_inserter(alive_units),
                 [](const auto& u) { return u.health > 0; });
    units = alive_units;

    round++;
  }

  return EXIT_SUCCESS;
}

