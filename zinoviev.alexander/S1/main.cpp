#include "BiList.hpp"
#include <iostream>
#include <string>
#include <limits>

int main()
{
  using namespace zinoviev;

  BiList<std::pair<std::string, BiList<unsigned long long>>> outer;
  std::string name{};
  unsigned long long x = 0;

  while (std::cin >> name)
  {
    BiList<unsigned long long> inner;

    while (std::cin >> x)
    {
      if (x > static_cast<unsigned long long>(std::numeric_limits<unsigned long long>::max()))
      {
        std::cerr << "Error: number out of range\n";
        return 1;
      }
      inner.push_back(static_cast<unsigned long long>(x));
    }

    outer.push_back(std::make_pair(name, std::move(inner)));

    if (!std::cin.eof())
    {
      std::cin.clear();
    }
  }

  if (outer.size() == 0)
  {
    std::cout << "0\n";
    return 0;
  }

  CBIter<std::pair<std::string, BiList<unsigned long long>>> c_iter_outer = outer.cbegin();

  std::cout << c_iter_outer->first;
  ++c_iter_outer;

  while (c_iter_outer != outer.cend())
  {
    std::cout << " " << c_iter_outer->first;
    ++c_iter_outer;
  }
  std::cout << "\n";

  BIter<std::pair<std::string, BiList<unsigned long long>>> iter_outer = outer.begin();

  BiList<std::pair<BIter<unsigned long long>, BIter<unsigned long long>>> list_iter_inner;
  while (iter_outer != outer.end())
  {
    if (iter_outer->second.size() > 0)
    {
      list_iter_inner.push_back(std::make_pair(iter_outer->second.begin(), iter_outer->second.end()));
    }
    ++iter_outer;
  }

  if (list_iter_inner.size() == 0)
  {
    std::cout << "0\n";
    return 0;
  }

  BIter<std::pair<BIter<unsigned long long>, BIter<unsigned long long>>> iter_l_i_inner = list_iter_inner.begin();

  BiList<unsigned long long> list_sum;
  unsigned long long sum = 0;

  while (list_iter_inner.size())
  {
    std::cout << *(iter_l_i_inner->first);

    sum = 0;
    sum += *(iter_l_i_inner->first);

    ++(iter_l_i_inner->first);

    for (BIter<std::pair<BIter<unsigned long long>, BIter<unsigned long long>>> i = ++(list_iter_inner.begin()); i != list_iter_inner.end(); )
    {
      std::cout << " " << *(i->first);

      if (sum > std::numeric_limits<unsigned long long>::max() - *(i->first))
      {
        std::cerr << "Error: overflow\n";
        return 1;
      }
      sum += *(i->first);

      ++(i->first);

      if (i->first == i->second)
      {
        i = list_iter_inner.erase(i);
      }
      else
      {
        ++i;
      }
    }

    std::cout << "\n";

    if (iter_l_i_inner->first == iter_l_i_inner->second)
    {
      BIter<std::pair<BIter<unsigned long long>, BIter<unsigned long long>>> tmp = iter_l_i_inner;
      ++iter_l_i_inner;
      list_iter_inner.erase(tmp);
    }

    list_sum.push_back(sum);
  }

  CBIter<unsigned long long> iter_sum = list_sum.cbegin();

  std::cout << *iter_sum;
  ++iter_sum;

  while (iter_sum != list_sum.cend())
  {
    std::cout << " " << *iter_sum;
    ++iter_sum;
  }
  std::cout << "\n";
}
