#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

struct Node
{
  Node* parent = nullptr;
  std::vector<Node> children;
  std::vector<int> metadata;
};

int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::function<Node()> recurse;
  recurse = [&infile, &recurse]() -> Node {
    int num_children, num_metadata;
    infile >> num_children >> num_metadata;
    Node n;
    for (auto i = 0; i < num_children; ++i) {
      auto c = std::move(recurse());
      c.parent = &n;
      n.children.emplace_back(std::move(c));
    }
    for (auto i = 0; i < num_metadata; ++i) {
      int m;
      infile >> m;
      n.metadata.emplace_back(m);
    }
    return n;
  };
  Node root = recurse();

  std::function<int(Node)> value;
  value = [&value](const Node& n) -> int {
    auto result = 0;
    if (n.children.size() > 0) {
      for (auto c : n.metadata) {
        if (c <= (int)n.children.size()) {
          result += value(n.children[c-1]);
        }
      }
    } else {
      result = std::accumulate(n.metadata.cbegin(), n.metadata.cend(), 0);
    }

    return result;
  };
  
  std::cout << value(root) << std::endl;
  
  return EXIT_SUCCESS;
}

