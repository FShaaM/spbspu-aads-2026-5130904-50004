#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include <iterator>
#include "Node.hpp"

namespace zinoviev
{
  template < class T > class BiList;
  template < class T > class CBIter;

  template < class T >
  class BIter : public std::iterator< std::bidirectional_iterator_tag, T, std::ptrdiff_t, T*, T& >
  {
    Node< T >* node_;
    friend class BiList< T >;
    friend class CBIter< T >;

   public:
    BIter() :
      node_(nullptr)
    {}

    explicit BIter(Node< T >* n) :
      node_(n)
    {}

    BIter< T >& operator++()
    {
      node_ = node_->next;
      return *this;
    }

    BIter< T > operator++(int)
    {
      BIter< T > temp = *this;
      ++(*this);
      return temp;
    }

    BIter< T >& operator--()
    {
      node_ = node_->prev;
      return *this;
    }

    BIter< T > operator--(int)
    {
      BIter< T > temp = *this;
      --(*this);
      return temp;
    }

    T& operator*() const
    {
      return node_->val;
    }

    T* operator->() const
    {
      return &(node_->val);
    }

    bool operator==(const BIter< T >& x) const
    {
      return node_ == x.node_;
    }

    bool operator!=(const BIter< T >& x) const
    {
      return !(*this == x);
    }
  };

  template < class T >
  class CBIter : public std::iterator< std::bidirectional_iterator_tag, const T, std::ptrdiff_t, const T*, const T& >
  {
    const Node< T >* node_;
    friend class BiList< T >;

   public:
    CBIter() :
      node_(nullptr)
    {}

    explicit CBIter(const Node< T >* n) :
      node_(n)
    {}

    CBIter(const BIter< T >& it) :
      node_(it.node_)
    {}

    CBIter< T >& operator++()
    {
      node_ = node_->next;
      return *this;
    }

    CBIter< T > operator++(int)
    {
      CBIter< T > temp = *this;
      ++(*this);
      return temp;
    }

    CBIter< T >& operator--()
    {
      node_ = node_->prev;
      return *this;
    }

    CBIter< T > operator--(int)
    {
      CBIter< T > temp = *this;
      --(*this);
      return temp;
    }

    const T& operator*() const
    {
      return node_->val;
    }

    const T* operator->() const
    {
      return &(node_->val);
    }

    bool operator==(const CBIter< T >& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const CBIter< T >& other) const
    {
      return !(*this == other);
    }
  };
}

#endif
