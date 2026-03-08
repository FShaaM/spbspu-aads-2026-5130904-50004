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

    while (std::cin.peek() != '\n' && std::cin.peek() != std::char_traits<char>::eof())
    {
      if (std::cin >> x)
      {
        inner.push_back(x);
      }
      else
      {
        std::cerr << "Error: invalid number\n";
        return 1;
      }

      while (std::cin.peek() == ' ')
      {
        std::cin.get();
      }
    }

    if (std::cin.peek() == '\n')
    {
      std::cin.get();
    }

    outer.push_back(std::make_pair(name, std::move(inner)));
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

  BiList<unsigned long long> list_sum;

  while (list_iter_inner.size())
  {
    BiList<unsigned long long> current_row;
    for (auto it = list_iter_inner.cbegin(); it != list_iter_inner.cend(); ++it)
    {
      current_row.push_back(*(it->first));
    }

    unsigned long long test_sum = 0;
    bool overflow = false;
    for (auto it = current_row.cbegin(); it != current_row.cend(); ++it)
    {
      if (test_sum > std::numeric_limits<unsigned long long>::max() - *it)
      {
        overflow = true;
        break;
      }
      test_sum += *it;
    }

    if (overflow)
    {
      std::cerr << "Overflow\n";
      return 1;
    }

    bool first_in_row = true;
    for (auto it = current_row.cbegin(); it != current_row.cend(); ++it)
    {
      if (!first_in_row) std::cout << " ";
      std::cout << *it;
      first_in_row = false;
    }
    std::cout << "\n";

    list_sum.push_back(test_sum);

    auto it = list_iter_inner.begin();
    while (it != list_iter_inner.end())
    {
      ++(it->first);
      if (it->first == it->second)
      {
        it = list_iter_inner.erase(it);
      }
      else
      {
        ++it;
      }
    }
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

  return 0;
}
