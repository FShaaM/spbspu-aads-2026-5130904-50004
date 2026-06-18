#ifndef BILIST_HPP
#define BILIST_HPP

#include <cstddef>
#include <algorithm>
#include "Node.hpp"
#include "iterators.hpp"

namespace zinoviev
{
  template< class T >
  class BiList
  {
  public:
    BiList();
    BiList(const BiList< T >& other);
    BiList(BiList< T >&& other) noexcept;
    ~BiList();

    BiList& operator=(const BiList< T >& other);
    BiList& operator=(BiList< T >&& other) noexcept;

    void push_front(const T& d);
    void push_back(const T& d);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void clear() noexcept;
    void swap(BiList< T >& other) noexcept;
    BIter< T > erase(BIter< T > pos) noexcept;

    size_t size() const noexcept;
    BIter< T > begin();
    BIter< T > end();
    CBIter< T > cbegin() const;
    CBIter< T > cend() const;

  private:
    Node< T >* head_;
    Node< T >* tail_;
    size_t size_;

    friend class BIter< T >;
    friend class CBIter< T >;
  };

  template< class T >
  BiList< T >::BiList():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}

  template< class T >
  BiList< T >::BiList(const BiList< T >& other):
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {
    Node< T >* cur = other.head_;
    while (cur)
    {
      push_back(cur->val);
      cur = cur->next;
    }
  }

  template< class T >
  BiList< T >::BiList(BiList< T >&& other) noexcept:
    head_(other.head_),
    tail_(other.tail_),
    size_(other.size_)
  {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  template< class T >
  BiList< T >::~BiList()
  {
    clear();
  }

  template< class T >
  BiList< T >& BiList< T >::operator=(const BiList< T >& other)
  {
    if (this != &other)
    {
      BiList< T > tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class T >
  BiList< T >& BiList< T >::operator=(BiList< T >&& other) noexcept
  {
    if (this != &other)
    {
      swap(other);
    }
    return *this;
  }

  template< class T >
  void BiList< T >::push_front(const T& d)
  {
    Node< T >* const newNode = new Node< T >(d);
    ++size_;

    if (!head_)
    {
      tail_ = newNode;
      head_ = newNode;
    }
    else
    {
      newNode->next = head_;
      head_->prev = newNode;
      head_ = newNode;
    }
  }

  template< class T >
  void BiList< T >::push_back(const T& d)
  {
    Node< T >* const newNode = new Node< T >(d);
    ++size_;

    if (!tail_)
    {
      tail_ = newNode;
      head_ = newNode;
    }
    else
    {
      newNode->prev = tail_;
      tail_->next = newNode;
      tail_ = newNode;
    }
  }

  template< class T >
  void BiList< T >::pop_front() noexcept
  {
    if (!head_)
    {
      return;
    }

    Node< T >* const temp = head_;
    head_ = head_->next;
    delete temp;
    --size_;

    if (head_)
    {
      head_->prev = nullptr;
    }
    else
    {
      tail_ = nullptr;
    }
  }

  template< class T >
  void BiList< T >::pop_back() noexcept
  {
    if (!tail_)
    {
      return;
    }

    Node< T >* const temp = tail_;
    tail_ = tail_->prev;
    delete temp;
    --size_;

    if (tail_)
    {
      tail_->next = nullptr;
    }
    else
    {
      head_ = nullptr;
    }
  }

  template< class T >
  void BiList< T >::clear() noexcept
  {
    Node< T >* temp = head_;
    while (head_)
    {
      head_ = head_->next;
      delete temp;
      temp = head_;
    }
    size_ = 0;
    tail_ = nullptr;
  }

  template< class T >
  void BiList< T >::swap(BiList< T >& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  template< class T >
  BIter< T > BiList< T >::erase(BIter< T > pos) noexcept
  {
    if (pos == end())
    {
      return pos;
    }

    Node< T >* const node = pos.node_;
    Node< T >* const pr_node = node->prev;
    Node< T >* const nx_node = node->next;

    if (node == head_)
    {
      pop_front();
      return BIter< T >(head_);
    }
    else if (node == tail_)
    {
      pop_back();
      return BIter< T >(nullptr);
    }
    else
    {
      pr_node->next = nx_node;
      nx_node->prev = pr_node;
      delete node;
      --size_;
      return BIter< T >(nx_node);
    }
  }

  template< class T >
  size_t BiList< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  BIter< T > BiList< T >::begin()
  {
    return BIter< T >(head_);
  }

  template< class T >
  BIter< T > BiList< T >::end()
  {
    return BIter< T >(nullptr);
  }

  template< class T >
  CBIter< T > BiList< T >::cbegin() const
  {
    return CBIter< T >(head_);
  }

  template< class T >
  CBIter< T > BiList< T >::cend() const
  {
    return CBIter< T >(nullptr);
  }
}

#endif
