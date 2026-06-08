#ifndef HASH_TABLE_ITERATOR_HPP
#define HASH_TABLE_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <utility>

namespace zinoviev
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable;

  template < class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator;

  template < class Key, class Value, class Hash, class Equal >
  class HashTableIterator :
    public std::iterator< std::forward_iterator_tag, std::pair<const Key, Value> >
  {
  private:
    HashTable< Key, Value, Hash, Equal >* table_;
    size_t index_;

  public:

    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashTableConstIterator< Key, Value, Hash, Equal >;

    HashTableIterator() :
      table_(nullptr),
      index_(0)
    {
    }

    HashTableIterator(HashTable< Key, Value, Hash, Equal >* table, size_t index) :
      table_(table),
      index_(index)
    {
    }

    std::pair<const Key, Value>& operator*() const
    {
      return table_->slots_[index_].pair;
    }

    std::pair<const Key, Value>* operator->() const
    {
      return &operator*();
    }

    HashTableIterator& operator++()
    {
      ++index_;
      while (index_ < table_->slots_.getSize() && !table_->slots_[index_].occupied)
        ++index_;
      return *this;
    }

    HashTableIterator operator++(int)
    {
      HashTableIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const HashTableIterator& other) const
    {
      return table_ == other.table_ && index_ == other.index_;
    }

    bool operator!=(const HashTableIterator& other) const
    {
      return !(*this == other);
    }
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashTableConstIterator :
    public std::iterator< std::forward_iterator_tag, const std::pair<const Key, Value> >
  {
  private:
    const HashTable< Key, Value, Hash, Equal >* table_;
    size_t index_;

  public:

    friend class HashTable< Key, Value, Hash, Equal >;

    HashTableConstIterator() :
      table_(nullptr),
      index_(0)
    {
    }

    HashTableConstIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t index) :
      table_(table),
      index_(index)
    {
    }

    HashTableConstIterator(const HashTableIterator< Key, Value, Hash, Equal >& other) :
      table_(other.table_),
      index_(other.index_)
    {
    }

    const std::pair<const Key, Value>& operator*() const
    {
      return table_->slots_[index_].pair;
    }

    const std::pair<const Key, Value>* operator->() const
    {
      return &operator*();
    }

    HashTableConstIterator& operator++()
    {
      ++index_;
      while (index_ < table_->slots_.getSize() && !table_->slots_[index_].occupied)
        ++index_;
      return *this;
    }

    HashTableConstIterator operator++(int)
    {
      HashTableConstIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const HashTableConstIterator& other) const
    {
      return table_ == other.table_ && index_ == other.index_;
    }

    bool operator!=(const HashTableConstIterator& other) const
    {
      return !(*this == other);
    }
  };
}

#endif
