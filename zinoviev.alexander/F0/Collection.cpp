#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include "Collection.hpp"
#include "Card.hpp"
#include "Functions_for_Collections.hpp"
#include "Vector.hpp"

namespace zinoviev
{

  Collection::Collection(const Collection& other):
    cards_(other.cards_),
    typeIndex_(other.typeIndex_)
  {
  }

  Collection::Collection(Collection&& other) noexcept:
    cards_(std::move(other.cards_)),
    typeIndex_(std::move(other.typeIndex_))
  {
  }

  Collection& Collection::operator=(const Collection& other)
  {
    if (this != std::addressof(other))
    {
      cards_ = other.cards_;
      typeIndex_ = other.typeIndex_;
    }
    return *this;
  }

  void Collection::saveToFile(std::ostream& out) const
  {
    for (auto it = cards_.cbegin(); it != cards_.cend(); ++it)
    {
      const Card* card = &(it->second);
      out << *card << "\n";
    }
  }

  std::istream& Collection::loadFromStream(std::istream& input)
  {
    if (!input)
    {
      return input;
    }

    Collection tmp;
    Card card;
    while (input >> card)
    {
      tmp.addCard(card);
    }

    std::swap(cards_, tmp.cards_);
    std::swap(typeIndex_, tmp.typeIndex_);

    return input;
  }

  void Collection::addCard(const Card& card)
  {
    auto res = cards_.insert(card.id, card);
    if (res.second)
    {
      try
      {
        typeIndex_[card.type].push_back(card.id);
      }
      catch (...)
      {
        cards_.erase(card.id);
        throw;
      }
    }
  }

  void Collection::removeCard(size_t id)
  {
    Card* card = findCard(id);
    if (!card)
    {
      return;
    }

    const std::string card_type = card->type;
    auto& vec_id = typeIndex_[card_type];
    vec_id.erase(std::remove(vec_id.begin(), vec_id.end(), id), vec_id.end());

    if (vec_id.empty())
    {
      typeIndex_.erase(card_type);
    }

    cards_.erase(id);
  }

  Card* Collection::findCard(size_t id)
  {
    auto res = cards_.find(id);
    if (res != cards_.end())
    {
      return &(res->second);
    }
    return nullptr;
  }

  const Card* Collection::findCard(size_t id) const
  {
    const auto res = cards_.find(id);
    if (res != cards_.cend())
    {
      return &(res->second);
    }
    return nullptr;
  }

  Vector< Card > Collection::filterByType(const std::string& type) const
  {
    Vector< Card > result;
    const auto it = typeIndex_.find(type);
    if (it == typeIndex_.end())
    {
      return result;
    }

    const auto& vec_id = it->second;
    for (size_t i = 0; i < vec_id.size(); ++i)
    {
      const Card* card = findCard(vec_id[i]);
      if (card)
      {
        result.push_back(*card);
      }
    }
    return result;
  }

  void Collection::updatePrice(size_t id, size_t newPrice)
  {
    Card* card = findCard(id);
    if (card)
    {
      card->price = newPrice;
    }
  }

  Vector< Card > Collection::getAllCards() const
  {
    Vector< Card > result;
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
      out << card << "\n";
    }
  }

}
