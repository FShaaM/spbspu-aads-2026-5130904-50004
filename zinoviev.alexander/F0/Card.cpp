#include "Card.hpp"
#include <iostream>
#include <string>
#include "Functions_for_Collections.hpp"

namespace zinoviev
{
  bool operator<(const Card& lhs, const Card& rhs)
  {
    return lhs.id < rhs.id;
  }

  std::ostream& operator<<(std::ostream& out, const Card& card)
  {
    std::ostream::sentry sentry(out);
    if (sentry)
    {
      IOGuard guard(out);
      out << card.id << " "
          << card.name << " "
          << card.type << " "
          << card.rarity << " "
          << card.power << " "
          << card.price;
    }
    return out;
  }

  std::istream& operator>>(std::istream& in, Card& card)
  {
    std::istream::sentry sentry(in);
    if (sentry)
    {
      IOGuard guard(in);
      in >> card.id
         >> card.name
         >> card.type
         >> card.rarity
         >> card.power
         >> card.price;
    }
    return in;
  }
}
