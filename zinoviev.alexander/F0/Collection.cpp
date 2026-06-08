#include <iostream>
#include <string>
#include <algorithm>
#include "Collection.hpp"
#include "Vector.hpp"
#include "Card.hpp"

namespace zinoviev
{
  void Collection::saveToFile(std::ostream& out) const
  {
    for (auto it = cards_.cbegin(); it != cards_.cend(); ++it)
    {
      const Card* card = &(it->second);

      out << card->id << " " << card->name << " " << card->type << " " << card->rarity
        << " " << card->power << " " << card->price << "\n";
    }
  }

  std::istream& Collection::loadFromStream(std::istream& input)
  {
    if (!input)
      return input;

    Card card;
    while (input >> card.id >> card.name >> card.type >> card.rarity >> card.power >> card.price)
    {
      addCard(card);
    }
    return input;
  }

  void Collection::addCard(const Card& card)
  {
    auto res = cards_.insert(card.id, card);

    if (res.second)
      typeIndex_[card.type].push_back(card.id);
  }

  void Collection::removeCard(size_t id)
  {
    Card* card = findCard(id);

    if (!card)
      return;

    auto& vec_id = typeIndex_[card->type];
    vec_id.erase(std::remove(vec_id.begin(), vec_id.end(), card->id), vec_id.end());

    if (vec_id.empty())
      typeIndex_.erase(card->type);

    cards_.erase(id);
  }

  Card* Collection::findCard(size_t id)
  {
    auto res = cards_.find(id);

    if (res != cards_.end())
      return &(res->second);

    return nullptr;
  }

  const Card* Collection::findCard(size_t id) const
  {
    auto res = cards_.find(id);

    if (res != cards_.cend())
      return &(res->second);

    return nullptr;
  }

  Vector< Card > Collection::filterByType(const std::string& type) const
  {
    Vector< Card > result;

    auto it = typeIndex_.find(type);
    if (it == typeIndex_.end())
      return result;

    const auto& vec_id = it->second;
    for (size_t i = 0; i < vec_id.size(); ++i)
    {
      const Card* card = findCard(vec_id[i]);
      if (card)
        result.push_back(*card);
    }

    return result;
  }

  void Collection::updatePrice(size_t id, size_t newPrice)
  {
    Card* card = findCard(id);

    if (card)
      card->price = newPrice;
  }

  Vector< Card > Collection::getAllCards() const
  {
    Vector<Card> result;
    for (auto it = cards_.cbegin(); it != cards_.cend(); ++it)
    {
      result.push_back(it->second);
    }
    return result;
  }

  void Collection::saveToStream(std::ostream& out) const
  {
    for (auto it = cards_.cbegin(); it != cards_.cend(); ++it)
    {
      const Card& card = it->second;
      out << card.id << ' '
        << card.name << ' '
        << card.type << ' '
        << card.rarity << ' '
        << card.price << ' '
        << card.power << '\n';
    }
  }
}
