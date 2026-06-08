#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include <cstddef>
#include <unordered_map>

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

    bool operator<(const Card& other) const
    {
      return id < other.id;
    }
  };
}

#endif
