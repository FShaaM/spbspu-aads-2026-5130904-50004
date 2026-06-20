#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <string>
#include "vector.hpp"
#include "slot.hpp"
#include "hash_table_iterators.hpp"

namespace zinoviev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    class Iterator;
    class ConstIterator;

    friend class Iterator;
    friend class ConstIterator;

    HashTable();
    HashTable(size_t buckets,
              size_t bucket_capacity,
              size_t overflow_capacity,
              const Hash& hash = Hash(),
              const Equal& equal = Equal());
    HashTable(const HashTable& other);

    size_t size() const;
    bool empty() const;
    void clear();
    Value* find(const Key& k);
    const Value* find(const Key& k) const;
    bool has(const Key& k) const;
    void add(const Key& k, const Value& val);
    void drop(const Key& k, Value& out);
    void rehash(size_t new_buckets);
    void erase(const Key& k) noexcept;

    void swap(HashTable& other) noexcept;

    Iterator begin();
    Iterator end();
    ConstIterator cbegin() const;
    ConstIterator cend() const;

  private:
    Vector< Slot< Key, Value > > slots_;
    size_t buckets_;
    size_t bucket_capacity_;
    size_t overflow_capacity_;
    size_t size_;

    Hash hasher_;
    Equal equal_;

    size_t bucket_index(const Key& k) const;
    size_t bucket_start(size_t bucket_idx) const;

    const Slot< Key, Value >* find_slot(const Key& k) const;
    Slot< Key, Value >* find_slot(const Key& k);
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    HashTable(16, 4, 16)
  {
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t buckets,
                                                  size_t bucket_capacity,
                                                  size_t overflow_capacity,
                                                  const Hash& hash,
                                                  const Equal& equal):
    buckets_(buckets),
    bucket_capacity_(bucket_capacity),
    overflow_capacity_(overflow_capacity),
    size_(0),
    hasher_(hash),
    equal_(equal)
  {
    const size_t total_slots = buckets_ * bucket_capacity_ + overflow_capacity_;
    Vector< Slot< Key, Value > > tmp(total_slots);
    slots_.swap(tmp);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
    slots_(other.slots_),
    buckets_(other.buckets_),
    bucket_capacity_(other.bucket_capacity_),
    overflow_capacity_(other.overflow_capacity_),
    size_(other.size_),
    hasher_(other.hasher_),
    equal_(other.equal_)
  {
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::bucket_index(const Key& k) const
  {
    return hasher_(k) % buckets_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::bucket_start(size_t bucket_idx) const
  {
    return bucket_idx * bucket_capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Slot< Key, Value >*
  HashTable< Key, Value, Hash, Equal >::find_slot(const Key& k) const
  {
    size_t id = bucket_start(bucket_index(k));
    const size_t bucket_end = id + bucket_capacity_;
    for (; id < bucket_end; ++id)
    {
      if (slots_[id].occupied && equal_(slots_[id].key, k))
      {
        return &slots_[id];
      }
    }

    id = buckets_ * bucket_capacity_;
    const size_t overflow_end = id + overflow_capacity_;
    for (; id < overflow_end; ++id)
    {
      if (slots_[id].occupied && equal_(slots_[id].key, k))
      {
        return &slots_[id];
      }
    }

    return nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  Slot< Key, Value >*
  HashTable< Key, Value, Hash, Equal >::find_slot(const Key& k)
  {
    size_t id = bucket_start(bucket_index(k));
    const size_t bucket_end = id + bucket_capacity_;
    for (; id < bucket_end; ++id)
    {
      if (slots_[id].occupied && equal_(slots_[id].key, k))
      {
        return &slots_[id];
      }
    }

    id = buckets_ * bucket_capacity_;
    const size_t overflow_end = id + overflow_capacity_;
    for (; id < overflow_end; ++id)
    {
      if (slots_[id].occupied && equal_(slots_[id].key, k))
      {
        return &slots_[id];
      }
    }

    return nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear()
  {
    const size_t total = slots_.getSize();
    for (size_t i = 0; i < total; ++i)
    {
      if (slots_[i].occupied)
      {
        Slot< Key, Value > empty;
        slots_[i].swap_slots(empty);
      }
    }
    size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value* HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
  {
    const Slot< Key, Value >* s = find_slot(k);
    if (s)
    {
      return &s->value;
    }
    return nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value* HashTable< Key, Value, Hash, Equal >::find(const Key& k)
  {
    Slot< Key, Value >* s = find_slot(k);
    if (s)
    {
      return &s->value;
    }
    return nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key& k) const
  {
    return find_slot(k) != nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& val)
  {
    Slot< Key, Value >* slot = find_slot(k);
    if (slot)
    {
      slot->value = val;
      return;
    }

    size_t id = bucket_start(bucket_index(k));
    const size_t bucket_end = id + bucket_capacity_;
    for (; id < bucket_end; ++id)
    {
      if (!slots_[id].occupied)
      {
        Slot< Key, Value > new_slot(k, val);
        slots_[id].swap_slots(new_slot);
        ++size_;
        return;
      }
    }

    id = buckets_ * bucket_capacity_;
    const size_t overflow_end = id + overflow_capacity_;
    for (; id < overflow_end; ++id)
    {
      if (!slots_[id].occupied)
      {
        Slot< Key, Value > new_slot(k, val);
        slots_[id].swap_slots(new_slot);
        ++size_;
        return;
      }
    }

    throw std::overflow_error("HashTable overflow capacity exceeded");
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::drop(const Key& k, Value& out)
  {
    Slot< Key, Value >* slot = find_slot(k);
    if (!slot)
    {
      throw std::out_of_range("Key not found");
    }

    out = slot->value;
    Slot< Key, Value > empty;
    slot->swap_slots(empty);
    --size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t new_buckets)
  {
    HashTable< Key, Value, Hash, Equal > new_table(new_buckets,
                                                   bucket_capacity_,
                                                   overflow_capacity_,
                                                   hasher_,
                                                   equal_);
    const size_t total = slots_.getSize();
    for (size_t i = 0; i < total; ++i)
    {
      if (slots_[i].occupied)
      {
        new_table.add(slots_[i].key, slots_[i].value);
      }
    }
    swap(new_table);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::erase(const Key& k) noexcept
  {
    Slot< Key, Value >* s = find_slot(k);
    if (!s)
    {
      return;
    }

    Slot< Key, Value > empty;
    s->swap_slots(empty);
    --size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
  {
    slots_.swap(other.slots_);
    std::swap(buckets_, other.buckets_);
    std::swap(bucket_capacity_, other.bucket_capacity_);
    std::swap(overflow_capacity_, other.overflow_capacity_);
    std::swap(size_, other.size_);
    std::swap(hasher_, other.hasher_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iterator
  HashTable< Key, Value, Hash, Equal >::begin()
  {
    size_t id = 0;
    const size_t total = slots_.getSize();
    while (id < total && !slots_[id].occupied)
    {
      ++id;
    }
    return Iterator(this, id);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::Iterator
  HashTable< Key, Value, Hash, Equal >::end()
  {
    return Iterator(this, slots_.getSize());
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::ConstIterator
  HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    size_t id = 0;
    const size_t total = slots_.getSize();
    while (id < total && !slots_[id].occupied)
    {
      ++id;
    }
    return ConstIterator(this, id);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::ConstIterator
  HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return ConstIterator(this, slots_.getSize());
  }
}

#endif
