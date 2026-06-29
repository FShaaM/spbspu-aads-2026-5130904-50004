#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include <string>
#include "RBTree.hpp"
#include "Vector.hpp"
#include "Hash_Table.hpp"
#include "Card.hpp"
#include "Functions_for_Collections.hpp"

namespace zinoviev
{
  class Collection
  {
  public:
    Collection() = default;
    Collection(const Collection& other);
    Collection(Collection&& other) noexcept;
    Collection& operator=(const Collection& other);

    void saveToFile(std::ostream& out) const;
    std::istream& loadFromStream(std::istream& in);

    void addCard(const Card& card);
    void removeCard(size_t id);
    Card* findCard(size_t id);
    const Card* findCard(size_t id) const;
    Vector< Card > filterByType(const std::string& type) const;
    void updatePrice(size_t id, size_t newPrice);
    Vector< Card > getAllCards() const;
    void saveToStream(std::ostream& out) const;

  private:
    RBTree< size_t, Card > cards_;
    HashTable< std::string,
               Vector< size_t >,
               std::hash< std::string >,
               std::equal_to< std::string > > typeIndex_;
  };
}

#endif
