#ifndef SLOT_HPP
#define SLOT_HPP

#include <utility>

namespace zinoviev
{
  template < class Key, class Value >
  struct Slot
  {
    std::pair< const Key, Value > pair;
    bool occupied;

    Slot() :
      occupied(false),
      pair()
    {
    }

    Slot(const Slot& other) :
      pair(other.pair),
      occupied(other.occupied)
    {
    }

    Slot(const Key& k, const Value& v) :
      pair(k, v),
      occupied(true)
    {
    }

    Slot(Key&& k, Value&& v) :
      pair(std::move(k), std::move(v)),
      occupied(true)
    {
    }

    void swap_slots(Slot& other) noexcept
    {
      std::swap(const_cast<Key&>(pair.first), const_cast<Key&>(other.pair.first));
      std::swap(pair.second, other.pair.second);
      std::swap(occupied, other.occupied);
    }
  };
}

#endif
