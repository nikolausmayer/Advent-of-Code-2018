#include <array>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>


int main(int argc, char *argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    throw std::runtime_error("Could not read from file");

  std::vector<std::array<int,4>> nanobots;

  while (true) {
    int a, b, c, d;
    infile.ignore(5); infile >> a;
    if (infile.eof())
      break;
    infile.ignore(1); infile >> b;
    infile.ignore(1); infile >> c;
    infile.ignore(5); infile >> d;
    infile.ignore(10, '\n');
    nanobots.push_back({a, b, c, d});
  }

  const std::size_t num = nanobots.size();
  std::vector<std::set<int>> neighbors(num);
  for (auto idx = 0u; idx < num; idx++) {
    for (auto jdx = idx+1; jdx < num; jdx++) {
      auto [x1,y1,z1,d1] = nanobots[idx];
      auto [x2,y2,z2,d2] = nanobots[jdx];
      if ((std::abs(x1-x2) + std::abs(y1-y2) + std::abs(z1-z2))
          <= d1+d2) {
        neighbors[idx].insert(jdx);
        neighbors[jdx].insert(idx);
      }
    }
  }

  typedef std::set<int> SI;

  /// Find all maximal cliques using Pivoting Bron-Kerbosch
  std::vector<SI> max_cliques;
  std::function<void(SI,SI,SI)> BronKerbosch;
  BronKerbosch = [&](SI R, SI P, SI X) {
    if (P.size() == 0 and X.size() == 0)
      max_cliques.push_back(R);

    auto pivot = *P.begin();
    SI Px = P;
    for (auto n : neighbors[pivot]) Px.erase(n);

    for (auto v : Px) {
      SI Rc = R; Rc.insert(v);
      SI Pc;
      SI Xc;
      for (auto n : neighbors[v]) {
        if (P.contains(n)) Pc.insert(n);
        if (X.contains(n)) Xc.insert(n);
      }

      BronKerbosch(Rc,Pc,Xc);

      P.erase(v);
      X.insert(v);
    }
  };
  SI all; for (auto i = 0u; i < num; i++) all.insert(i);
  BronKerbosch(SI{}, all, SI{});

  /// Find largest clique size
  auto max_clique_size = (*max_cliques.begin()).size();
  decltype(max_cliques) largest_cliques;

  /// Keep all cliques of this largest size
  for (auto clique : max_cliques) {
    if (clique.size() > max_clique_size) {
      max_clique_size = clique.size();
      largest_cliques.clear();
    }
    if (clique.size() >= max_clique_size)
      largest_cliques.push_back(clique);
  }

  /// Find *any* point in range of all nanobots
  for (auto clique : largest_cliques) {
    /// Find largest possible area of all-in-range points
    auto minx =  999999999;
    auto miny =  999999999;
    auto minz =  999999999;
    auto maxx = -999999999;
    auto maxy = -999999999;
    auto maxz = -999999999;
    for (auto node : clique) {
      auto [x,y,z,d] = nanobots[node];
      minx = std::min(minx, x+d);
      miny = std::min(miny, y+d);
      minz = std::min(minz, z+d);
      maxx = std::max(maxx, x-d);
      maxy = std::max(maxy, y-d);
      maxz = std::max(maxz, z-d);
    }

    /// Start at center of area and walk towards out-of-range bots
    auto x = (minx+maxx)/2;
    auto y = (miny+maxy)/2;
    auto z = (minz+maxz)/2;
    while (true) {
      bool all_in_range = true;
      for (auto node : clique) {
        auto [nx,ny,nz,d] = nanobots[node];
        auto dist = std::abs(x-nx) + std::abs(y-ny) + std::abs(z-nz);
        if (dist > d) {
          all_in_range = false;
          if      (x < nx) x++;
          else if (x > nx) x--;
          if      (y < ny) y++;
          else if (y > ny) y--;
          if      (z < nz) z++;
          else if (z > nz) z--;
        }
      }
      if (all_in_range)
        break;
    }

    /// Walk towards origin. The area of closest points must be
    /// compact which makes it possible to just straight-walk
    /// in (+-1,+-1,+-1) towards the target. All candidate
    /// points MUST be on a plane tilted towards the origin,
    /// this is THE KEY INSIGHT from the problem's "Manhattan
    /// distance" property.
    auto dx = (x < 0 ? 1 : -1);
    auto dy = (y < 0 ? 1 : -1);
    auto dz = (z < 0 ? 1 : -1);
    while (true) {
      bool all_in_range = true;
      for (auto node : clique) {
        auto [nx,ny,nz,d] = nanobots[node];
        auto dist = std::abs(x+dx-nx) + std::abs(y+dy-ny) + std::abs(z+dz-nz);
        if (dist > d) {
          all_in_range = false;
          break;
        }
      }
      if (not all_in_range) {
        break;
      }
      x += dx;
      y += dy;
      z += dz;
    }
    std::cout << std::abs(x) + std::abs(y) + std::abs(z) << std::endl;

    //for (auto x = maxx; x <= minx; x++)
    //for (auto y = maxy; y <= miny; y++)
    //for (auto z = maxz; z <= minz; z++) {
    //  bool ok = true;
    //  for (auto node : clique) {
    //    auto [nx,ny,nz,d] = nanobots[node];
    //    auto dist = std::abs(x-nx) + std::abs(y-ny) + std::abs(z-nz);
    //    if (dist > d) {
    //      ok = false;
    //      break;
    //    }
    //  }
    //  if (ok) {
    //    auto dist = std::abs(x) + std::abs(y) + std::abs(z);
    //    mindistance = std::min(mindistance, dist);
    //    std::cout << mindistance << std::endl;
    //  }
    //}
  }

  return EXIT_SUCCESS;
}

