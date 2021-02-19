#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
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

  auto metasum = 0;

  std::function<Node()> recurse;
  recurse = [&infile, &recurse, &metasum]() -> Node {
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
      metasum += m;
    }
    return n;
  };
  Node root = recurse();
  
  std::cout << metasum << std::endl;
  
  return EXIT_SUCCESS;
}

