#ifndef BILIST_HPP
#define BILIST_HPP

#include <cstddef>
#include <algorithm>
#include "Node.hpp"
#include "iterators.hpp"

namespace zinoviev
{
  template < class T >
  class BiList
  {
    Node< T >* head_;
    Node< T >* tail_;
    size_t size_;

   public:
    friend class BIter< T >;
    friend class CBIter< T >;

    BiList() :
      head_(nullptr),
      tail_(nullptr),
      size_(0)
    {}

    BiList(const BiList< T >& other) :
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

    BiList(BiList< T >&& other) noexcept :
      head_(other.head_),
      tail_(other.tail_),
      size_(other.size_)
    {
      other.head_ = other.tail_ = nullptr;
      other.size_ = 0;
    }

    BiList& operator=(const BiList< T >& other)
    {
      if (this != &other)
      {
        BiList< T > tmp(other);
        swap(tmp);
      }
      return *this;
    }

    BiList& operator=(BiList< T >&& other) noexcept
    {
      if (this != &other)
      {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
      }
      return *this;
    }

    ~BiList()
    {
      clear();
    }

    void push_front(const T& d);
    void push_back(const T& d);
    void clear() noexcept;
    size_t size() const noexcept
    {
      return size_;
    }

    BIter< T > erase(BIter< T > pos) noexcept;
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(BiList< T >& other) noexcept;

    BIter< T > begin()
    {
      return BIter< T >(head_);
    }

    BIter< T > end()
    {
      return BIter< T >(nullptr);
    }

    CBIter< T > cbegin() const
    {
      return CBIter< T >(head_);
    }

    CBIter< T > cend() const
    {
      return CBIter< T >(nullptr);
    }
  };

  template < class T >
  void BiList< T >::push_front(const T& d)
  {
    Node< T >* newNode = new Node< T >(d);
    ++size_;

    if (!head_)
    {
      tail_ = head_ = newNode;
    }
    else
    {
      newNode->next = head_;
      head_->prev = newNode;
      head_ = newNode;
    }
  }

  template < class T >
  void BiList< T >::push_back(const T& d)
  {
    Node< T >* newNode = new Node< T >(d);
    ++size_;

    if (!tail_)
    {
      tail_ = head_ = newNode;
    }
    else
    {
      newNode->prev = tail_;
      tail_->next = newNode;
      tail_ = newNode;
    }
  }

  template < class T >
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

  template < class T >
  void BiList< T >::pop_front() noexcept
  {
    if (!head_)
    {
      return;
    }

    Node< T >* temp = head_;
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

  template < class T >
  void BiList< T >::pop_back() noexcept
  {
    if (!tail_)
    {
      return;
    }

    Node< T >* temp = tail_;
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

  template < class T >
  void BiList< T >::swap(BiList< T >& other) noexcept
  {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  template < class T >
  BIter< T > BiList< T >::erase(BIter< T > x) noexcept
  {
    if (x == end())
    {
      return x;
    }

    Node< T >* node = x.node_;
    Node< T >* pr_node = node->prev;
    Node< T >* nx_node = node->next;

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
}

#endif
