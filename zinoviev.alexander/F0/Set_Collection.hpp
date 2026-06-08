#ifndef SET_COLLECTION_HPP
#define SET_COLLECTION_HPP

#include <string>
#include <iostream>
#include "Vector.hpp"
#include "Card.hpp"
#include "RBTree.hpp"
#include "Iterators.hpp"

namespace zinoviev
{
  class SetCollection
  {
    RBTree< std::string, Vector< size_t > > sets_;

  public:

    void addSet(const std::string& name, const Vector< size_t >& ids);
    bool hasSet(const std::string& name) const;
    const Vector<size_t>* getSet(const std::string& name) const;
    void removeSet(const std::string& name);

    std::istream& loadFromStream(std::istream& in);
    void saveToStream(std::ostream& out) const;

    CIterator< std::string, Vector< size_t > > cbegin() const;
    CIterator< std::string, Vector< size_t > > cend() const;
    Iterator< std::string, Vector< size_t > > begin();
    Iterator< std::string, Vector< size_t > > end();
  };
}

#endif
