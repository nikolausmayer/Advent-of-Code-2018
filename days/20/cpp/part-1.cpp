#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <tuple>
#include <vector>

struct Node
{
  Node(Node* parent=nullptr)
    : parent(parent)
  { 
    if (parent) {
      x = parent->x;
      y = parent->y;
    } else {
      x = 0;
      y = 0;
    }
  }

  ~Node() {
    for (auto c : children)
      delete(c);
  }

  int x, y;
  Node* const parent = nullptr;
  std::vector<Node*> children;
};

int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::set<std::tuple<int,int,bool>> open_doors;
  //auto print_map = [&open_doors] {
  //  auto minx = 0;
  //  auto maxx = 0;
  //  auto miny = 0;
  //  auto maxy = 0;
  //  for (auto [x,y,_] : open_doors) {
  //    minx = std::min(x, minx);
  //    maxx = std::max(x, maxx);
  //    miny = std::min(y, miny);
  //    maxy = std::max(y, maxy);
  //  }

  //  for (auto i = 0; i < 2*(maxx-minx+1)+1; i++)
  //    std::cout << "#";
  //  std::cout << std::endl;
  //  for (auto y = miny; y <= maxy; y++) {
  //    std::cout << "#";
  //    for (auto x = minx; x <= maxx; x++) {
  //      std::cout << ((x == 0 and y == 0) ? "X" : ".")
  //                << (open_doors.contains({x,y,true}) ? "|" : "#");
  //    }
  //    std::cout << std::endl;
  //    std::cout << "#";
  //    for (auto x = minx; x <= maxx; x++) {
  //      std::cout << (open_doors.contains({x,y,false}) ? "-" : "#")
  //                << "#";
  //    }
  //    std::cout << std::endl;
  //  }
  //};

  {
    auto tree = Node{};
    Node* current = &tree;

    char c = infile.get();
    for (c = infile.get(); c != '$'; c = infile.get()) {
      switch (c) {
        case 'N':
          current->y--;
          open_doors.insert({current->x, current->y, false});
          break;
        case 'S':
          open_doors.insert({current->x, current->y, false});
          current->y++;
          break;
        case 'W':
          current->x--;
          open_doors.insert({current->x, current->y, true});
          break;
        case 'E':
          open_doors.insert({current->x, current->y, true});
          current->x++;
          break;
        case '(':
          current->children.push_back(new Node{current});
          current = current->children.back();
          break;
        case ')':
          current = current->parent;
          break;
        case '|':
          current = current->parent;
          current->children.push_back(new Node{current});
          current = current->children.back();
          break;
        default:
          std::cerr << "Unknown symbol '" << c << "'\n";
          return EXIT_FAILURE;
      }
    }
  }

  //print_map();


  typedef std::array<int,3> ANode;
  std::map<std::array<int,2>, int> distances;
  distances[{0,0}] = 0;
  std::set<std::array<int,2>> visited;
  auto cmp = [](const ANode& a, const ANode& b) {
    return a[0] > b[0];
  };
  std::priority_queue<ANode, 
                      std::vector<ANode>,
                      decltype(cmp)> queue(cmp);
  queue.push({0,0,0});
  while (queue.size()) {
    auto [c,x,y] = queue.top();
    queue.pop();

    if (visited.contains({x,y}))
      continue;
    visited.insert({x,y});

    if (open_doors.contains({x-1,y,true})) {
      auto& nd = distances[{x-1,y}];
      nd = (nd > 0 ? std::min(c+1, nd) : c+1);
      queue.push({c+1,x-1,y});
    }
    if (open_doors.contains({x,y,true})) {
      auto& nd = distances[{x+1,y}];
      nd = (nd > 0 ? std::min(c+1, nd) : c+1);
      queue.push({c+1,x+1,y});
    }
    if (open_doors.contains({x,y-1,false})) {
      auto& nd = distances[{x,y-1}];
      nd = (nd > 0 ? std::min(c+1, nd) : c+1);
      queue.push({c+1,x,y-1});
    }
    if (open_doors.contains({x,y,false})) {
      auto& nd = distances[{x,y+1}];
      nd = (nd > 0 ? std::min(c+1, nd) : c+1);
      queue.push({c+1,x,y+1});
    }
  }

  auto maxdist = 0;
  for (auto [_, d] : distances)
    maxdist = std::max(d, maxdist);

  std::cout << maxdist << std::endl;

  return EXIT_SUCCESS;
}

