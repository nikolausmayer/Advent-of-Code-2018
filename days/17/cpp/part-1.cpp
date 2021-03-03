#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <stdexcept>
#include <utility>
#include <vector>


struct Head
{
  Head(int x, int y, Head* parent=nullptr)
    : x(x), y(y),
      ID(running_ID++), 
      parent(parent)
  { }

  int x, y, ID;
  static int running_ID;
  Head* parent = nullptr;
  std::list<Head*> children;
  bool stopped = false;
  bool terminal = false;
};
int Head::running_ID = 0;

std::ostream& operator<<(std::ostream& oss, const Head& h) 
{
  oss << h.ID << " (" << h.x  << "," << h.y  << ")";
  return oss;
}


int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::map<std::pair<int,int>, int> map;
  int minx = 9999;
  int maxx = 0;
  int miny = 9999;
  int maxy = 0;

  while (true) {
    char c = infile.get();
    if (infile.eof())
      break;
    infile.get();
    if (c == 'x') {
      int x, y1, y2;
      infile >> x;
      infile.get(); infile.get(); infile.get(); infile.get();
      infile >> y1;
      infile.get(); infile.get();
      infile >> y2;
      for (auto y = y1; y <= y2; y++)
        map[{x,y}] = 1;
      minx = std::min(minx, x);
      miny = std::min(miny, y1);
      maxx = std::max(maxx, x);
      maxy = std::max(maxy, y2);
    } else {
      int y, x1, x2;
      infile >> y;
      infile.get(); infile.get(); infile.get(); infile.get();
      infile >> x1;
      infile.get(); infile.get();
      infile >> x2;
      for (auto x = x1; x <= x2; x++)
        map[{x,y}] = 1;
      minx = std::min(minx, x1);
      miny = std::min(miny, y);
      maxx = std::max(maxx, x2);
      maxy = std::max(maxy, y);
    }
    infile.get();
  }

  /// Water can overflow the clay bounds by 1 on each side
  minx--;
  maxx++;

  std::vector<Head*> all_heads;
  std::list<Head*> active_heads;
  all_heads.emplace_back(new Head(500, 0));
  active_heads.push_back(all_heads.back());
  map[{500,0}] = 2;

  auto print_map = [&]() {
    for (auto y = miny; y <= maxy; y++) {
      for (auto x = minx; x <= maxx; x++) {
        auto c = map[{x,y}];
        std::cout << (c == 0 ? '.' :
                      c == 1 ? '#' :
                      c == 2 ? 'v' :
                      c == 3 ? '<' :
                      c == 4 ? '>' :
                      c == 5 ? '~' :
                               'X');
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  };

  auto make_all_children_terminal = [&](Head* h) {
    std::list<Head*> remaining;
    remaining.push_back(h);
    while (remaining.size()) {
      auto n = remaining.back();
      remaining.pop_back();
      n->terminal = true;
      map[{n->x,n->y}] = 2;
      for (auto c : n->children)
        if (c->y <= n->y and not c->terminal)
          remaining.push_back(c);
    }
  };

  while (active_heads.size()) {
    Head* h = active_heads.back();
    while (h->stopped) {
      active_heads.pop_back();
      if (active_heads.size() == 0)
        break;
      h = active_heads.back();
    }
    if (active_heads.size() == 0)
      break;

    auto& below = map[{h->x, h->y+1}];
    auto& left  = map[{h->x-1, h->y}];
    auto& right = map[{h->x+1, h->y}];

    if (below >= 2 and below <= 4) {
      h->terminal = true;
      make_all_children_terminal(h);
      active_heads.pop_back();
      if (active_heads.size() == 0)
        break;
      h = active_heads.back();
    } else if (below == 0) {
      if (h->y == maxy) {
        h->stopped = true;
        h->terminal = true;
        make_all_children_terminal(h);
        active_heads.pop_back();
        h = h->parent;
        while (std::all_of(h->children.cbegin(),
                           h->children.cend(),
                           [](auto hp) {
                             return hp->terminal;
                           })) {
          h->stopped = true;
          h->terminal = true;
          make_all_children_terminal(h);
          active_heads.erase(std::find(active_heads.begin(),
                                       active_heads.end(),
                                       h));
          h = h->parent;
        }
        continue;
      } else {
        map[{h->x, h->y+1}] = 2;
        all_heads.emplace_back(new Head(h->x, h->y+1, h));
        active_heads.push_back(all_heads.back());
        h->children.push_back(active_heads.back());
      }
    } else if (left == 0 or right == 0) {
      if (left == 0) {
        map[{h->x-1, h->y}] = 3;
        all_heads.emplace_back(new Head(h->x-1, h->y, h));
        active_heads.push_back(all_heads.back());
        h->children.push_back(active_heads.back());
      }
      if (right == 0) {
        map[{h->x+1, h->y}] = 4;
        all_heads.emplace_back(new Head(h->x+1, h->y, h));
        active_heads.push_back(all_heads.back());
        h->children.push_back(active_heads.back());
      } 
    } else {
      if (std::any_of(h->children.cbegin(),
                      h->children.cend(),
                      [](auto hp) {
                        return hp->terminal;
                      })) {
        while (std::any_of(h->children.cbegin(),
                           h->children.cend(),
                           [](auto hp) {
                             return hp->terminal;
                           })) {
          h->terminal = true;
          make_all_children_terminal(h);
          active_heads.pop_back();
          if (active_heads.size() == 0)
            break;
          h = active_heads.back();
        }
      } else {
        h->stopped = true;
        while (h->stopped) {
          map[{h->x,h->y}] = 5;
          active_heads.pop_back();
          if (active_heads.size() == 0)
            break;
          h = active_heads.back();
        }
      }
    }
  }

  //print_map();

  auto result = 0;
  for (auto y = miny; y <= maxy; y++) {
    for (auto x = minx; x <= maxx; x++) {
      result += (map[{x,y}] > 1);
    }
  }
  std::cout << result << std::endl;

  return EXIT_SUCCESS;
}

