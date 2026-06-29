#ifndef HASH_TABLE_ITERATOR_HPP
#define HASH_TABLE_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <utility>

namespace zinoviev
{

  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator;

  template< class Key, class Value, class Hash, class Equal >
  class HashTableIterator:
    public std::iterator< std::forward_iterator_tag, std::pair< const Key, Value > >
  {
  public:
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashTableConstIterator< Key, Value, Hash, Equal >;

    HashTableIterator();
    HashTableIterator(HashTable< Key, Value, Hash, Equal >* table, size_t index);

    std::pair< const Key, Value >& operator*() const;
    std::pair< const Key, Value >* operator->() const;
    HashTableIterator& operator++();
    HashTableIterator operator++(int);
    bool operator==(const HashTableIterator& other) const;
    bool operator!=(const HashTableIterator& other) const;

  private:
    HashTable< Key, Value, Hash, Equal >* table_;
    size_t index_;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator:
    public std::iterator< std::forward_iterator_tag, const std::pair< const Key, Value > >
  {
  public:
    friend class HashTable< Key, Value, Hash, Equal >;

    HashTableConstIterator();
    HashTableConstIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t index);
    HashTableConstIterator(const HashTableIterator< Key, Value, Hash, Equal >& other);

    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;
    HashTableConstIterator& operator++();
    HashTableConstIterator operator++(int);
    bool operator==(const HashTableConstIterator& other) const;
    bool operator!=(const HashTableConstIterator& other) const;

  private:
    const HashTable< Key, Value, Hash, Equal >* table_;
    size_t index_;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator():
    table_(nullptr),
    index_(0)
  {
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >::HashTableIterator(
      HashTable< Key, Value, Hash, Equal >* table,
      size_t index):
    table_(table),
    index_(index)
  {
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< const Key, Value >&
  HashTableIterator< Key, Value, Hash, Equal >::operator*() const
  {
    return table_->slots_[index_].pair;
  }

  template< class Key, class Value, class Hash, class Equal >
  std::pair< const Key, Value >*
  HashTableIterator< Key, Value, Hash, Equal >::operator->() const
  {
    return &operator*();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >&
  HashTableIterator< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    while (index_ < table_->slots_.size() && !table_->slots_[index_].occupied)
    {
      ++index_;
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableIterator< Key, Value, Hash, Equal >
  HashTableIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    HashTableIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool
  HashTableIterator< Key, Value, Hash, Equal >::operator==(const HashTableIterator& other) const
  {
    return table_ == other.table_ && index_ == other.index_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool
  HashTableIterator< Key, Value, Hash, Equal >::operator!=(const HashTableIterator& other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator():
    table_(nullptr),
    index_(0)
  {
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator(
      const HashTable< Key, Value, Hash, Equal >* table,
      size_t index):
    table_(table),
    index_(index)
  {
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >::HashTableConstIterator(
      const HashTableIterator< Key, Value, Hash, Equal >& other):
    table_(other.table_),
    index_(other.index_)
  {
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< const Key, Value >&
  HashTableConstIterator< Key, Value, Hash, Equal >::operator*() const
  {
    return table_->slots_[index_].pair;
  }

  template< class Key, class Value, class Hash, class Equal >
  const std::pair< const Key, Value >*
  HashTableConstIterator< Key, Value, Hash, Equal >::operator->() const
  {
    return &operator*();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >&
  HashTableConstIterator< Key, Value, Hash, Equal >::operator++()
  {
    ++index_;
    while (index_ < table_->slots_.getSize() && !table_->slots_[index_].occupied)
    {
      ++index_;
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >
  HashTableConstIterator< Key, Value, Hash, Equal >::operator++(int)
  {
    HashTableConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool
  HashTableConstIterator< Key, Value, Hash, Equal >::operator==(
      const HashTableConstIterator& other) const
  {
    return table_ == other.table_ && index_ == other.index_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool
  HashTableConstIterator< Key, Value, Hash, Equal >::operator!=(
      const HashTableConstIterator& other) const
  {
    return !(*this == other);
  }

}

#endif
