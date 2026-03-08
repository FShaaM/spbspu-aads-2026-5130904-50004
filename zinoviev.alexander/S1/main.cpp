#include "BiList.hpp"
#include <iostream>
#include <string>
#include <utility>

using namespace zinoviev;
int main()
{
  BiList<std::pair<std::string, BiList<int>>> outer;
  std::string name{};
  int x = 0;

  while (std::cin >> name)
  {
    BiList<int> inner;

    while (std::cin >> x)
    {
      inner.push_back(x);
    }

    outer.push_back(std::make_pair(name, std::move(inner)));

    if (!std::cin.eof())
    {
      std::cin.clear();
    }
  }

  if (!outer.size())
  {
    std::cout << "0\n";
    return 0;
  }

  CBIter<std::pair<std::string, BiList<int>>> c_iter_outer = outer.cbegin();

  std::cout << c_iter_outer->first;
  ++c_iter_outer;

  while (c_iter_outer != outer.cend())
  {
    std::cout << " " << c_iter_outer->first;
    ++c_iter_outer;
  }
  std::cout << "\n";
}
