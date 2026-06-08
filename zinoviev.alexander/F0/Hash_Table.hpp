#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include "Vector.hpp"
#include "Slot.hpp"
#include "Hash_Table_Iterators.hpp"

namespace zinoviev
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator;

  template < class Key, class Value, class Hash, class Equal >
  class HashTableIterator;

  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  private:
    Vector< Slot< Key, Value > > slots_;
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
    const Slot< Key, Value >* find_slot(const Key& k) const;
    Slot< Key, Value >* find_slot(const Key& k);
    void check_and_rehash();
    HashTableIterator< Key, Value, Hash, Equal > find_iterator(const Key& k);
    void add(const Key& k, const Value& val);
    void add(Key&& k, Value&& val);

  public:

    friend class HashTableConstIterator< Key, Value, Hash, Equal >;
    friend class HashTableIterator< Key, Value, Hash, Equal >;

    HashTable();
    HashTable(size_t buckets, size_t bucket_capacity, size_t overflow_capacity,
      const Hash& hash = Hash(), const Equal& equal = Equal());
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    HashTableIterator< Key, Value, Hash, Equal > begin() noexcept;
    HashTableConstIterator< Key, Value, Hash, Equal > begin() const noexcept;
    HashTableConstIterator< Key, Value, Hash, Equal > cbegin() const noexcept;
    HashTableIterator< Key, Value, Hash, Equal > end() noexcept;
    HashTableConstIterator< Key, Value, Hash, Equal > end() const noexcept;
    HashTableConstIterator< Key, Value, Hash, Equal > cend() const noexcept;

    void clear() noexcept;
    std::pair< HashTableIterator< Key, Value, Hash, Equal >, bool >
      insert(const std::pair<const Key, Value>& value);
    std::pair< HashTableIterator< Key, Value, Hash, Equal >, bool >
      insert(std::pair<const Key, Value>&& value);
    template < class P >
    std::pair< HashTableIterator< Key, Value, Hash, Equal >, bool >
      insert(P&& value);
    template < class... Args >
    std::pair< HashTableIterator< Key, Value, Hash, Equal >, bool >
      emplace(Args&&... args);

    HashTableIterator< Key, Value, Hash, Equal > erase(HashTableConstIterator< Key, Value, Hash, Equal > pos);
    HashTableIterator< Key, Value, Hash, Equal > erase(HashTableConstIterator< Key, Value, Hash, Equal > first,
      HashTableConstIterator< Key, Value, Hash, Equal > last);
    size_t erase(const Key& k);

    void swap(HashTable& other) noexcept;

    Value& operator[](const Key& k);
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    HashTableIterator< Key, Value, Hash, Equal > find(const Key& k);
    HashTableConstIterator< Key, Value, Hash, Equal > find(const Key& k) const;
    bool contains(const Key& k) const;
    size_t count(const Key& k) const;

    size_t bucket_count() const noexcept;
    float load_factor() const noexcept;
    float max_load_factor() const noexcept;
    void max_load_factor(float ml);
    void rehash(size_t n);
    void reserve(size_t n);
  };

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable() :
    HashTable(16, 4, 16)
  {
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t buckets, size_t bucket_capacity,
    size_t overflow_capacity, const Hash& hash, const Equal& equal) :
    buckets_(buckets),
    bucket_capacity_(bucket_capacity),
    overflow_capacity_(overflow_capacity),
    total_slots_(buckets* bucket_capacity + overflow_capacity),
    size_(0),
    max_load_factor_(0.75f),
    hasher_(hash),
    equal_(equal)
  {
    Vector< Slot< Key, Value > > tmp(total_slots_);
    slots_.swap(tmp);
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other) :
    slots_(other.slots_),
    buckets_(other.buckets_),
    bucket_capacity_(other.bucket_capacity_),
    overflow_capacity_(other.overflow_capacity_),
    total_slots_(other.total_slots_),
    size_(other.size_),
    max_load_factor_(other.max_load_factor_),
    hasher_(other.hasher_),
    equal_(other.equal_)
  {
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& other) noexcept :
    slots_(std::move(other.slots_)),
    buckets_(other.buckets_),
    bucket_capacity_(other.bucket_capacity_),
    overflow_capacity_(other.overflow_capacity_),
    total_slots_(other.total_slots_),
    size_(other.size_),
    max_load_factor_(other.max_load_factor_),
    hasher_(std::move(other.hasher_)),
    equal_(std::move(other.equal_))
  {
    other.buckets_ = 0;
    other.bucket_capacity_ = 0;
    other.overflow_capacity_ = 0;
    other.total_slots_ = 0;
    other.size_ = 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >&
    HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
  {
    if (this != &other)
    {
      HashTable tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >&
    HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& other) noexcept
  {
    if (this != &other)
    {
      slots_ = std::move(other.slots_);
      buckets_ = other.buckets_;
      bucket_capacity_ = other.bucket_capacity_;
      overflow_capacity_ = other.overflow_capacity_;
      total_slots_ = other.total_slots_;
      size_ = other.size_;
      max_load_factor_ = other.max_load_factor_;
      hasher_ = std::move(other.hasher_);
      equal_ = std::move(other.equal_);
      other.buckets_ = 0;
      other.bucket_capacity_ = 0;
      other.overflow_capacity_ = 0;
      other.total_slots_ = 0;
      other.size_ = 0;
    }
    return *this;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template < class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::begin() noexcept
  {
    size_t id = 0;
    while (id < slots_.getSize() && !slots_[id].occupied)
      ++id;
    return HashTableIterator< Key, Value, Hash, Equal >(this, id);
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::begin() const noexcept
  {
    return cbegin();
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
  {
    size_t id = 0;
    while (id < slots_.getSize() && !slots_[id].occupied)
      ++id;
    return HashTableConstIterator< Key, Value, Hash, Equal >(this, id);
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::end() noexcept
  {
    return HashTableIterator< Key, Value, Hash, Equal >(this, slots_.size());
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::end() const noexcept
  {
    return cend();
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::cend() const noexcept
  {
    return HashTableConstIterator< Key, Value, Hash, Equal >(this, slots_.size());
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear() noexcept
  {
    for (size_t i = 0; i < slots_.getSize(); ++i)
    {
      if (slots_[i].occupied)
      {
        Slot< Key, Value > empty;
        slots_[i].swap_slots(empty);
      }
    }
    size_ = 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::bucket_index(const Key& k) const
  {
    return hasher_(k) % buckets_;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::bucket_start(size_t bucket_idx) const
  {
    return bucket_idx * bucket_capacity_;
  }

  template < class Key, class Value, class Hash, class Equal >
  const Slot< Key, Value >* HashTable< Key, Value, Hash, Equal >::find_slot(const Key& k) const
  {
    size_t id = bucket_start(bucket_index(k));
    for (size_t i = id; i < id + bucket_capacity_; ++i)
      if (slots_[i].occupied && equal_(slots_[i].pair.first, k))
        return &slots_[i];

    id = buckets_ * bucket_capacity_;
    for (size_t i = id; i < id + overflow_capacity_; ++i)
      if (slots_[i].occupied && equal_(slots_[i].pair.first, k))
        return &slots_[i];

    return nullptr;
  }

  template < class Key, class Value, class Hash, class Equal >
  Slot< Key, Value >* HashTable< Key, Value, Hash, Equal >::find_slot(const Key& k)
  {
    size_t id = bucket_start(bucket_index(k));
    for (size_t i = id; i < id + bucket_capacity_; ++i)
      if (slots_[i].occupied && equal_(slots_[i].pair.first, k))
        return &slots_[i];

    id = buckets_ * bucket_capacity_;
    for (size_t i = id; i < id + overflow_capacity_; ++i)
      if (slots_[i].occupied && equal_(slots_[i].pair.first, k))
        return &slots_[i];

    return nullptr;
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::check_and_rehash()
  {
    if (static_cast<float>(size_) > total_slots_ * max_load_factor_)
    {
      size_t new_buckets = buckets_ * 2;
      if (new_buckets == 0) new_buckets = 1;
      rehash(new_buckets);
    }
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::find_iterator(const Key& k)
  {
    Slot< Key, Value >* s = find_slot(k);
    if (!s) return end();
    size_t index = s - &slots_[0];
    return HashTableIterator< Key, Value, Hash, Equal >(this, index);
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& val)
  {
    Slot< Key, Value >* slot = find_slot(k);
    if (slot)
    {
      slot->pair.second = val;
      return;
    }

    size_t id = bucket_start(bucket_index(k));
    for (size_t i = id; i < id + bucket_capacity_; ++i)
    {
      if (!slots_[i].occupied)
      {
        Slot< Key, Value > new_slot(k, val);
        slots_[i].swap_slots(new_slot);
        ++size_;
        check_and_rehash();
        return;
      }
    }

    id = buckets_ * bucket_capacity_;
    for (size_t i = id; i < id + overflow_capacity_; ++i)
    {
      if (!slots_[i].occupied)
      {
        Slot< Key, Value > new_slot(k, val);
        slots_[i].swap_slots(new_slot);
        ++size_;
        check_and_rehash();
        return;
      }
    }

    throw std::overflow_error("HashTable overflow capacity exceeded");
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(Key&& k, Value&& val)
  {
    Slot< Key, Value >* slot = find_slot(k);
    if (slot)
    {
      slot->pair.second = std::move(val);
      return;
    }

    size_t id = bucket_start(bucket_index(k));
    for (size_t i = id; i < id + bucket_capacity_; ++i)
    {
      if (!slots_[i].occupied)
      {
        Slot< Key, Value > new_slot(std::move(k), std::move(val));
        slots_[i].swap_slots(new_slot);
        ++size_;
        check_and_rehash();
        return;
      }
    }

    id = buckets_ * bucket_capacity_;
    for (size_t i = id; i < id + overflow_capacity_; ++i)
    {
      if (!slots_[i].occupied)
      {
        Slot< Key, Value > new_slot(std::move(k), std::move(val));
        slots_[i].swap_slots(new_slot);
        ++size_;
        check_and_rehash();
        return;
      }
    }

    throw std::overflow_error("HashTable overflow capacity exceeded");
  }

  template < class Key, class Value, class Hash, class Equal >
  std::pair< HashTableIterator< Key, Value, Hash, Equal >, bool >
    HashTable< Key, Value, Hash, Equal >::insert(const std::pair<const Key, Value>& value)
  {
    HashTableIterator< Key, Value, Hash, Equal > it = find_iterator(value.first);
    if (it != end())
      return std::make_pair(it, false);
    add(value.first, value.second);
    return std::make_pair(find_iterator(value.first), true);
  }

  template < class Key, class Value, class Hash, class Equal >
  std::pair< HashTableIterator< Key, Value, Hash, Equal >, bool >
    HashTable< Key, Value, Hash, Equal >::insert(std::pair<const Key, Value>&& value)
  {
    HashTableIterator< Key, Value, Hash, Equal > it = find_iterator(value.first);
    if (it != end())
      return std::make_pair(it, false);
    add(std::move(value.first), std::move(value.second));
    return std::make_pair(find_iterator(value.first), true);
  }

  template < class Key, class Value, class Hash, class Equal >
  template < class P >
  std::pair< HashTableIterator< Key, Value, Hash, Equal >, bool >
    HashTable< Key, Value, Hash, Equal >::insert(P&& value)
  {
    return insert(std::pair<const Key, Value>(std::forward<P>(value)));
  }

  template < class Key, class Value, class Hash, class Equal >
  template < class... Args >
  std::pair< HashTableIterator< Key, Value, Hash, Equal >, bool >
    HashTable< Key, Value, Hash, Equal >::emplace(Args&&... args)
  {
    return insert(std::pair<const Key, Value>(std::forward<Args>(args)...));
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::erase(HashTableConstIterator< Key, Value, Hash, Equal > pos)
  {
    size_t index = pos.index_;
    if (index >= slots_.getSize() || !slots_[index].occupied)
      return end();
    Slot< Key, Value > empty;
    slots_[index].swap_slots(empty);
    --size_;
    HashTableIterator< Key, Value, Hash, Equal > it(this, index);
    ++it;
    return it;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::erase(HashTableConstIterator< Key, Value, Hash, Equal > first,
      HashTableConstIterator< Key, Value, Hash, Equal > last)
  {
    HashTableIterator< Key, Value, Hash, Equal > it = const_cast<HashTableIterator< Key, Value, Hash, Equal >&>(first);
    while (it != const_cast<HashTableIterator< Key, Value, Hash, Equal >&>(last))
      it = erase(it);
    return it;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key& k)
  {
    Slot< Key, Value >* s = find_slot(k);
    if (!s) return 0;
    Slot< Key, Value > empty;
    s->swap_slots(empty);
    --size_;
    return 1;
  }

  template < class Key, class Value, class Hash, class Equal >
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

  template < class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::operator[](const Key& k)
  {
    HashTableIterator< Key, Value, Hash, Equal > it = find_iterator(k);
    if (it != end())
      return (*it).second;
    add(k, Value());
    return find(k)->second;
  }

  template < class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::at(const Key& k)
  {
    Value* v = find(k);
    if (!v)
      throw std::out_of_range("HashTable::at");
    return *v;
  }

  template < class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& k) const
  {
    const Value* v = find(k);
    if (!v)
      throw std::out_of_range("HashTable::at");
    return *v;
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::find(const Key& k)
  {
    Slot< Key, Value >* s = find_slot(k);
    if (!s) return end();
    size_t index = s - &slots_[0];
    return HashTableIterator< Key, Value, Hash, Equal >(this, index);
  }

  template < class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >
    HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
  {
    const Slot< Key, Value >* s = find_slot(k);
    if (!s) return cend();
    size_t index = s - &slots_[0];
    return HashTableConstIterator< Key, Value, Hash, Equal >(this, index);
  }

  template < class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::contains(const Key& k) const
  {
    return find_slot(k) != nullptr;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::count(const Key& k) const
  {
    return contains(k) ? 1 : 0;
  }

  template < class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::bucket_count() const noexcept
  {
    return buckets_;
  }

  template < class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::load_factor() const noexcept
  {
    return static_cast<float>(size_) / total_slots_;
  }

  template < class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::max_load_factor() const noexcept
  {
    return max_load_factor_;
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::max_load_factor(float ml)
  {
    max_load_factor_ = ml;
    check_and_rehash();
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t n)
  {
    if (n == 0) n = 1;
    HashTable new_table(n, bucket_capacity_, overflow_capacity_, hasher_, equal_);
    for (size_t i = 0; i < slots_.size(); ++i)
      if (slots_[i].occupied)
        new_table.add(slots_[i].pair.first, slots_[i].pair.second);
    swap(new_table);
  }

  template < class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::reserve(size_t n)
  {
    rehash(n);
  }
}

#endif
