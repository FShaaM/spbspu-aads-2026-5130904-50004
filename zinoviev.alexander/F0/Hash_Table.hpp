#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <memory>
#include "Vector.hpp"
#include "Hash_Table_Iterators.hpp"

namespace zinoviev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableIterator;

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    using iterator = HashTableIterator< Key, Value, Hash, Equal >;
    using const_iterator = HashTableConstIterator< Key, Value, Hash, Equal >;

    friend class HashTableConstIterator< Key, Value, Hash, Equal >;
    friend class HashTableIterator< Key, Value, Hash, Equal >;

    HashTable();

    HashTable(size_t buckets, size_t bucket_capacity,
      size_t overflow_capacity, const Hash& hash = Hash(), const Equal& equal = Equal());

    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    void clear() noexcept;
    std::pair< iterator, bool > insert(const std::pair< const Key, Value >& value);
    std::pair< iterator, bool > insert(std::pair< const Key, Value >&& value);

    iterator erase(const_iterator pos);
    size_t erase(const Key& k);

    void swap(HashTable& other) noexcept;

    Value& operator[](const Key& k);
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    iterator find(const Key& k);
    const_iterator find(const Key& k) const;
    bool contains(const Key& k) const;
    size_t count(const Key& k) const;

    size_t bucket_count() const noexcept;
    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float ml);
    void rehash(size_t n);
    void reserve(size_t n);

  private:
    struct Slot
    {
      std::pair< const Key, Value > pair;
      bool occupied;

      Slot():
        occupied(false),
        pair()
      {
      }

      Slot(const Slot& other):
        pair(other.pair),
        occupied(other.occupied)
      {
      }

      Slot(const Key& k, const Value& v):
        pair(k, v),
        occupied(true)
      {
      }

      Slot(Key&& k, Value&& val):
        pair(std::move(k), std::move(val)),
        occupied(true)
      {
      }

      void swap_slots(Slot& other) noexcept
      {
        std::swap(const_cast< Key& >(pair.first), const_cast< Key& >(other.pair.first));
        std::swap(pair.second, other.pair.second);
        std::swap(occupied, other.occupied);
      }
    };

    using SlotType = Slot;

    Vector< SlotType > slots_;
    size_t buckets_;
    size_t bucket_capacity_;
    size_t overflow_capacity_;
    size_t total_slots_;
    size_t size_;
    float max_load_factor_;

    Hash hasher_;
    Equal equal_;

    size_t bucket_index(const Key& k) const;
    size_t bucket_start(size_t bucket_idx) const;
    const SlotType* find_slot(const Key& k) const;
    SlotType* find_slot(const Key& k);
    void check_and_rehash();
    HashTableIterator< Key, Value, Hash, Equal > find_iterator(const Key& k);
    void add(const Key& k, const Value& val);
    void add(Key&& k, Value&& val);
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    HashTable(16, 4, 16)
  {
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(
    size_t buckets,
    size_t bucket_capacity,
    size_t overflow_capacity,
    const Hash& hash,
    const Equal& equal
    ):
    buckets_(buckets),
    bucket_capacity_(bucket_capacity),
    overflow_capacity_(overflow_capacity),
    total_slots_(buckets * bucket_capacity + overflow_capacity),
    size_(0),
    max_load_factor_(0.75f),
    hasher_(hash),
    equal_(equal)
  {
    Vector< SlotType > tmp(total_slots_);
    slots_.swap(tmp);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
    HashTable(other.buckets_, other.bucket_capacity_, other.overflow_capacity_, other.hasher_, other.equal_)
  {
    slots_ = other.slots_;
    size_ = other.size_;
    max_load_factor_ = other.max_load_factor_;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& other) noexcept:
    HashTable()
  {
    this->swap(other);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >&
  HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
  {
    if (this != std::addressof(other))
    {
      HashTable tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >&
  HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      swap(other);
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::begin() noexcept
  {
    size_t id = 0;
    while (id < slots_.size() && !slots_[id].occupied)
    {
      ++id;
    }
    return iterator(this, id);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::begin() const noexcept
  {
    return cbegin();
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
  {
    size_t id = 0;
    while (id < slots_.size() && !slots_[id].occupied)
    {
      ++id;
    }
    return const_iterator(this, id);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::end() noexcept
  {
    return iterator(this, slots_.size());
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::end() const noexcept
  {
    return cend();
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::cend() const noexcept
  {
    return const_iterator(this, slots_.size());
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear() noexcept
  {
    const size_t total = slots_.size();
    for (size_t i = 0; i < total; ++i)
    {
      if (slots_[i].occupied)
      {
        SlotType empty;
        slots_[i].swap_slots(empty);
      }
    }
    size_ = 0;
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
  const typename HashTable< Key, Value, Hash, Equal >::SlotType*
  HashTable< Key, Value, Hash, Equal >::find_slot(const Key& k) const
  {
    size_t id = bucket_start(bucket_index(k));
    const size_t bucket_end = id + bucket_capacity_;
    for (; id < bucket_end; ++id)
    {
      if (slots_[id].occupied && equal_(slots_[id].pair.first, k))
      {
        return &slots_[id];
      }
    }

    id = buckets_ * bucket_capacity_;
    const size_t overflow_end = id + overflow_capacity_;
    for (; id < overflow_end; ++id)
    {
      if (slots_[id].occupied && equal_(slots_[id].pair.first, k))
      {
        return &slots_[id];
      }
    }

    return nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::SlotType*
  HashTable< Key, Value, Hash, Equal >::find_slot(const Key& k)
  {
    size_t id = bucket_start(bucket_index(k));
    const size_t bucket_end = id + bucket_capacity_;
    for (; id < bucket_end; ++id)
    {
      if (slots_[id].occupied && equal_(slots_[id].pair.first, k))
      {
        return std::addressof(slots_[id]);
      }
    }

    id = buckets_ * bucket_capacity_;
    const size_t overflow_end = id + overflow_capacity_;
    for (; id < overflow_end; ++id)
    {
      if (slots_[id].occupied && equal_(slots_[id].pair.first, k))
      {
        return std::addressof(slots_[id]);
      }
    }

    return nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::check_and_rehash()
  {
    if (static_cast< float >(size_) > total_slots_ * max_load_factor_)
    {
      size_t new_buckets = buckets_ * 2;
      if (new_buckets == 0)
      {
        new_buckets = 1;
      }
      rehash(new_buckets);
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::find_iterator(const Key& k)
  {
    SlotType* s = find_slot(k);
    if (!s)
    {
      return end();
    }
    const size_t index = s - std::addressof(*slots_.begin());
    return iterator(this, index);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& val)
  {
    SlotType* slot = find_slot(k);
    if (slot)
    {
      slot->pair.second = val;
      return;
    }

    size_t id = bucket_start(bucket_index(k));
    const size_t bucket_end = id + bucket_capacity_;
    for (; id < bucket_end; ++id)
    {
      if (!slots_[id].occupied)
      {
        SlotType new_slot(k, val);
        slots_[id].swap_slots(new_slot);
        ++size_;
        check_and_rehash();
        return;
      }
    }

    id = buckets_ * bucket_capacity_;
    const size_t overflow_end = id + overflow_capacity_;
    for (; id < overflow_end; ++id)
    {
      if (!slots_[id].occupied)
      {
        SlotType new_slot(k, val);
        slots_[id].swap_slots(new_slot);
        ++size_;
        check_and_rehash();
        return;
      }
    }

    throw std::overflow_error("HashTable overflow capacity exceeded");
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(Key&& k, Value&& val)
  {
    SlotType* slot = find_slot(k);
    if (slot)
    {
      slot->pair.second = std::move(val);
      return;
    }

    size_t id = bucket_start(bucket_index(k));
    const size_t bucket_end = id + bucket_capacity_;
    for (; id < bucket_end; ++id)
    {
      if (!slots_[id].occupied)
      {
        SlotType new_slot(std::move(k), std::move(val));
        slots_[id].swap_slots(new_slot);
        ++size_;
        check_and_rehash();
        return;
      }
    }

    id = buckets_ * bucket_capacity_;
    const size_t overflow_end = id + overflow_capacity_;
    for (; id < overflow_end; ++id)
    {
      if (!slots_[id].occupied)
      {
        SlotType new_slot(std::move(k), std::move(val));
        slots_[id].swap_slots(new_slot);
        ++size_;
        check_and_rehash();
        return;
      }
    }

    throw std::overflow_error("HashTable overflow capacity exceeded");
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::iterator, bool >
  HashTable< Key, Value, Hash, Equal >::insert(const std::pair< const Key, Value >& value)
  {
    iterator it = find_iterator(value.first);
    if (it != end())
    {
      return std::make_pair(it, false);
    }
    add(value.first, value.second);
    return std::make_pair(find_iterator(value.first), true);
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< typename HashTable< Key, Value, Hash, Equal >::iterator, bool >
  HashTable< Key, Value, Hash, Equal >::insert(std::pair< const Key, Value >&& value)
  {
    iterator it = find_iterator(value.first);
    if (it != end())
    {
      return std::make_pair(it, false);
    }
    add(std::move(value.first), std::move(value.second));
    return std::make_pair(find_iterator(value.first), true);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::erase(const_iterator pos)
  {
    const size_t index = pos.index_;
    if (index >= slots_.size() || !slots_[index].occupied)
    {
      return end();
    }
    SlotType empty;
    slots_[index].swap_slots(empty);
    --size_;
    iterator it(this, index);
    ++it;
    return it;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& k)
  {
    SlotType* s = find_slot(k);
    if (!s)
    {
      return 0;
    }
    SlotType empty;
    s->swap_slots(empty);
    --size_;
    return 1;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
  {
    slots_.swap(other.slots_);
    std::swap(buckets_, other.buckets_);
    std::swap(bucket_capacity_, other.bucket_capacity_);
    std::swap(overflow_capacity_, other.overflow_capacity_);
    std::swap(total_slots_, other.total_slots_);
    std::swap(size_, other.size_);
    std::swap(max_load_factor_, other.max_load_factor_);
    std::swap(hasher_, other.hasher_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& k)
  {
    iterator it = find_iterator(k);
    if (it != end())
    {
      return (*it).second;
    }
    add(k, Value());
    return find(k)->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& k)
  {
    iterator it = find_iterator(k);
    if (it == end())
    {
      throw std::out_of_range("HashTable::at");
    }
    return (*it).second;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& k) const
  {
    const SlotType* s = find_slot(k);
    if (!s)
    {
      throw std::out_of_range("HashTable::at");
    }
    return s->pair.second;
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::iterator
  HashTable< Key, Value, Hash, Equal >::find(const Key& k)
  {
    SlotType* s = find_slot(k);
    if (!s)
    {
      return end();
    }
    const size_t index = s - std::addressof(*slots_.begin());
    return iterator(this, index);
  }

  template< class Key, class Value, class Hash, class Equal >
  typename HashTable< Key, Value, Hash, Equal >::const_iterator
  HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
  {
    const SlotType* s = find_slot(k);
    if (!s)
    {
      return cend();
    }
    const size_t index = s - std::addressof(*slots_.begin());
    return const_iterator(this, index);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::contains(const Key& k) const
  {
    return find_slot(k) != nullptr;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::count(const Key& k) const
  {
    return contains(k) ? 1 : 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::bucket_count() const noexcept
  {
    return buckets_;
  }

  template< class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::load_factor() const noexcept
  {
    return static_cast< float >(size_) / total_slots_;
  }

  template< class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::max_load_factor(float ml)
  {
    max_load_factor_ = ml;
    check_and_rehash();
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t n)
  {
    if (n == 0)
    {
      n = 1;
    }
    HashTable new_table(n, bucket_capacity_, overflow_capacity_, hasher_, equal_);
    const size_t total = slots_.size();
    for (size_t i = 0; i < total; ++i)
    {
      if (slots_[i].occupied)
      {
        new_table.add(slots_[i].pair.first, slots_[i].pair.second);
      }
    }
    swap(new_table);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::reserve(size_t n)
  {
    rehash(n);
  }
}

#endif
