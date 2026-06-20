#ifndef HASH_TABLE_ITERATORS_HPP
#define HASH_TABLE_ITERATORS_HPP

#include <iterator>
#include <utility>

namespace zinoviev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable< Key, Value, Hash, Equal >::Iterator:
    public std::iterator< std::forward_iterator_tag,
                          std::pair< const Key, Value >,
                          std::ptrdiff_t,
                          std::pair< const Key, Value >*,
                          std::pair< const Key&, Value& > >
  {
    HashTable* ptr_;
    size_t id_;

  public:
    Iterator():
      ptr_(nullptr),
      id_(0)
    {
    }

    Iterator(HashTable* x, size_t i):
      ptr_(x),
      id_(i)
    {
    }

    std::pair< const Key&, Value& > operator*() const
    {
      return std::pair< const Key&, Value& >(ptr_->slots_[id_].key,
                                             ptr_->slots_[id_].value);
    }

    Iterator& operator++()
    {
      ++id_;
      while (id_ < ptr_->slots_.getSize() && !ptr_->slots_[id_].occupied)
      {
        ++id_;
      }
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const Iterator& other) const
    {
      return ptr_ == other.ptr_ && id_ == other.id_;
    }

    bool operator!=(const Iterator& other) const
    {
      return !(*this == other);
    }
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable< Key, Value, Hash, Equal >::ConstIterator:
    public std::iterator< std::forward_iterator_tag,
                          std::pair< const Key, Value >,
                          std::ptrdiff_t,
                          std::pair< const Key, Value >*,
                          std::pair< const Key&, const Value& > >
  {
    const HashTable* ptr_;
    size_t id_;

  public:
    ConstIterator():
      ptr_(nullptr),
      id_(0)
    {
    }

    ConstIterator(const HashTable* x, size_t i):
      ptr_(x),
      id_(i)
    {
    }

    std::pair< const Key&, const Value& > operator*() const
    {
      return std::pair< const Key&, const Value& >(ptr_->slots_[id_].key,
                                                   ptr_->slots_[id_].value);
    }

    ConstIterator& operator++()
    {
      ++id_;
      while (id_ < ptr_->slots_.getSize() && !ptr_->slots_[id_].occupied)
      {
        ++id_;
      }
      return *this;
    }

    ConstIterator operator++(int)
    {
      ConstIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const ConstIterator& other) const
    {
      return ptr_ == other.ptr_ && id_ == other.id_;
    }

    bool operator!=(const ConstIterator& other) const
    {
      return !(*this == other);
    }
  };
}

#endif
