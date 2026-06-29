#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
#include <string>
#include <cstddef>

namespace zinoviev
{
  struct Card
  {
    size_t id;
    std::string name;
    std::string type;
    size_t rarity;
    size_t price;
    size_t power;
  };

  bool operator<(const Card& lhs, const Card& rhs);
  std::ostream& operator<<(std::ostream& out, const Card& card);
  std::istream& operator>>(std::istream& in, Card& card);
}

#endif
