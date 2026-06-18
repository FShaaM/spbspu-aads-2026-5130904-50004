#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include <iterator>
#include "Node.hpp"

namespace zinoviev
{
  template< class T >
  class BiList;

  template< class T >
  class CBIter;

  template< class T >
  class BIter: public std::iterator< std::bidirectional_iterator_tag,
                                      T,
                                      std::ptrdiff_t,
                                      T*,
                                      T& >
  {
  public:
    BIter();
    explicit BIter(Node< T >* n);
    BIter< T >& operator++();
    BIter< T > operator++(int);
    BIter< T >& operator--();
    BIter< T > operator--(int);
    T& operator*() const;
    T* operator->() const;
    bool operator==(const BIter< T >& x) const;
    bool operator!=(const BIter< T >& x) const;

  private:
    Node< T >* node_;
    friend class BiList< T >;
    friend class CBIter< T >;
  };

  template< class T >
  class CBIter: public std::iterator< std::bidirectional_iterator_tag,
                                       const T,
                                       std::ptrdiff_t,
                                       const T*,
                                       const T& >
  {
  public:
    CBIter();
    explicit CBIter(const Node< T >* n);
    CBIter(const BIter< T >& it);
    CBIter< T >& operator++();
    CBIter< T > operator++(int);
    CBIter< T >& operator--();
    CBIter< T > operator--(int);
    const T& operator*() const;
    const T* operator->() const;
    bool operator==(const CBIter< T >& other) const;
    bool operator!=(const CBIter< T >& other) const;

  private:
    const Node< T >* node_;
    friend class BiList< T >;
  };

  template< class T >
  BIter< T >::BIter():
    node_(nullptr)
  {}

  template< class T >
  BIter< T >::BIter(Node< T >* n):
    node_(n)
  {}

  template< class T >
  BIter< T >& BIter< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  BIter< T > BIter< T >::operator++(int)
  {
    BIter< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< class T >
  BIter< T >& BIter< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  BIter< T > BIter< T >::operator--(int)
  {
    BIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  T& BIter< T >::operator*() const
  {
    return node_->val;
  }

  template< class T >
  T* BIter< T >::operator->() const
  {
    return &(node_->val);
  }

  template< class T >
  bool BIter< T >::operator==(const BIter< T >& x) const
  {
    return node_ == x.node_;
  }

  template< class T >
  bool BIter< T >::operator!=(const BIter< T >& x) const
  {
    return !(*this == x);
  }

  template< class T >
  CBIter< T >::CBIter():
    node_(nullptr)
  {}

  template< class T >
  CBIter< T >::CBIter(const Node< T >* n):
    node_(n)
  {}

  template< class T >
  CBIter< T >::CBIter(const BIter< T >& it):
    node_(it.node_)
  {}

  template< class T >
  CBIter< T >& CBIter< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  CBIter< T > CBIter< T >::operator++(int)
  {
    CBIter< T > temp = *this;
    ++(*this);
    return temp;
  }

  template< class T >
  CBIter< T >& CBIter< T >::operator--()
  {
    node_ = node_->prev;
    return *this;
  }

  template< class T >
  CBIter< T > CBIter< T >::operator--(int)
  {
    CBIter< T > temp = *this;
    --(*this);
    return temp;
  }

  template< class T >
  const T& CBIter< T >::operator*() const
  {
    return node_->val;
  }

  template< class T >
  const T* CBIter< T >::operator->() const
  {
    return &(node_->val);
  }

  template< class T >
  bool CBIter< T >::operator==(const CBIter< T >& other) const
  {
    return node_ == other.node_;
  }

  template< class T >
  bool CBIter< T >::operator!=(const CBIter< T >& other) const
  {
    return !(*this == other);
  }
}

#endif
