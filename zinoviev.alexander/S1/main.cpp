#include <iostream>
#include <string>
#include <limits>
#include "BiList.hpp"

int main()
{
  using namespace zinoviev;
  using ULL = unsigned long long;

  BiList< std::pair< std::string, BiList< ULL > > > outer;
  std::string name{};
  ULL x = 0;

  while (std::cin >> name)
  {
    BiList< ULL > inner;

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

  if (outer.size() == 0)
  {
    std::cout << "0\n";
    return 0;
  }

  CBIter< std::pair< std::string, BiList< ULL > > > cItOuter =
      outer.cbegin();
  std::cout << cItOuter->first;
  ++cItOuter;

  for (; cItOuter != outer.cend(); ++cItOuter)
  {
    std::cout << " " << cItOuter->first;
  }
  std::cout << "\n";

  BIter< std::pair< std::string, BiList< ULL > > > itOuter =
      outer.begin();
  BiList< std::pair< BIter< ULL >, BIter< ULL > > > pairList;

  while (itOuter != outer.end())
  {
    if (itOuter->second.size() > 0)
    {
      pairList.push_back(
          std::make_pair(itOuter->second.begin(), itOuter->second.end())
      );
    }
    ++itOuter;
  }

  if (pairList.size() == 0)
  {
    std::cout << "0\n";
    return 0;
  }

  BIter< std::pair< BIter< ULL >, BIter< ULL > > > itPair =
      pairList.begin();
  BiList< ULL > listSum;

  while (pairList.size())
  {
    std::cout << *(itPair->first);
    ULL sum = *(itPair->first);
    ++(itPair->first);

    for (BIter< std::pair< BIter< ULL >, BIter< ULL > > >
        i = ++(pairList.begin());
        i != pairList.end(); )
    {
      std::cout << " " << *(i->first);

      if (sum > std::numeric_limits< ULL >::max() - *(i->first))
      {
        std::cout << "\n";
        std::cerr << "Overflow\n";
        return 1;
      }

      sum += *(i->first);
      ++(i->first);

      if (i->first == i->second)
      {
        i = pairList.erase(i);
      }
      else
      {
        ++i;
      }
    }

    std::cout << "\n";
    listSum.push_back(sum);

    if (itPair->first == itPair->second)
    {
      BIter< std::pair< BIter< ULL >, BIter< ULL > > > tmp =
          itPair;
      ++itPair;
      pairList.erase(tmp);
    }
  }

  CBIter< ULL > iterSum = listSum.cbegin();
  std::cout << *iterSum;
  ++iterSum;

  while (iterSum != listSum.cend())
  {
    std::cout << " " << *iterSum;
    ++iterSum;
  }
  std::cout << "\n";
}
