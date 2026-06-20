#ifndef SLOT_HPP
#define SLOT_HPP

#include <utility>

namespace zinoviev
{
  template< class Key, class Value >
  struct Slot
  {
    Key key;
    Value value;
    bool occupied;

    Slot():
      occupied(false),
      key(),
      value()
    {
    }

    Slot(const Slot& other):
      key(other.key),
      value(other.value),
      occupied(other.occupied)
    {
    }

    Slot(const Key& k, const Value& v):
      key(k),
      value(v),
      occupied(true)
    {
    }

    void swap_slots(Slot& other) noexcept
    {
      std::swap(key, other.key);
      std::swap(value, other.value);
      std::swap(occupied, other.occupied);
    }
  };
}

#endif
